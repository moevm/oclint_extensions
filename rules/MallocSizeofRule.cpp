#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"

namespace oclint {
    class MallocSizeofRule : public oclint::AbstractASTVisitorRule<MallocSizeofRule>
    {
    private:
    public:
        virtual void setUp() override {}
        virtual void tearDown() override {}

        virtual const std::string name() const override
        {
            return "no sizeof inside malloc/realloc";
        }

        virtual int priority() const override
        {
            return 2;
        }

        virtual const std::string category() const override
        {
            return "etu";
        }

    #ifdef DOCGEN
        virtual const std::string since() const override
        {
            return "22.02 etu";
        }

        virtual const std::string description() const override
        {
            return "You shouldn`t rely on fixed integers when allocating memory. sizeof(typename) is the right way to go.";
        }

        virtual const std::string example() const override
        {
            return R"rst(
    .. code-block:: cpp
        #include <stdio.h>
        #include <stdlib.h>
        
        void example()
        {
            int len = 0;
            int *arr = NULL;

            scanf("%d", &len);
            arr = malloc(4 * len); // this is bad

            for (int i = 0; i < len; i++) {
                scanf("%d", &arr[i]);
            }
            
            // ...

            free(arr);
            return 0;
        }
            )rst";
        }
    #endif

        void ReportSizeofViolation(clang::CallExpr *call_expr)
        {
            addViolation(call_expr, this, "The size of the allocated memory should be 'sizeof(typename) * SOMETHING'");
        }

        bool CheckSizeof(clang::Expr *expr)
        {
            if (expr->getStmtClass() == clang::Stmt::ImplicitCastExprClass) {
                expr = clang::dyn_cast<clang::ImplicitCastExpr>(expr)->getSubExpr();
            }

            if (expr->getStmtClass() != clang::Stmt::UnaryExprOrTypeTraitExprClass) {
                return false;
            }

            auto *unary = clang::dyn_cast<clang::UnaryExprOrTypeTraitExpr>(expr);
            clang::UnaryExprOrTypeTrait kind = unary->getKind();
            if (kind == clang::UnaryExprOrTypeTrait::UETT_SizeOf)
                return true;

            return false;
        }

        bool VisitCallExpr(clang::CallExpr *call_expr)
        {
            clang::Expr *node = call_expr->getCallee();
            
            if (node == nullptr) // is it even possible? anyway
                return true;
            
            if (node->getStmtClass() == clang::Stmt::ImplicitCastExprClass) {
                node = clang::dyn_cast<clang::ImplicitCastExpr>(node)->getSubExpr();
            }

            if (node->getStmtClass() != clang::Stmt::DeclRefExprClass)
                return true;

            auto *decl_ref = clang::dyn_cast<clang::DeclRefExpr>(node);
            std::string name = decl_ref->getNameInfo().getName().getAsString();

            int arg_index;
            if (name == "malloc")
                arg_index = 0;
            else if (name == "realloc")
                arg_index = 1;
            else
                return true;

            unsigned int n_args = call_expr->getNumArgs();
            if (arg_index >= n_args) // unlikely to happen. but check it anyway
                return true;

            clang::Expr *arg = call_expr->getArg(arg_index);
            
            if (CheckSizeof(arg)) {
                return true;
            }

            if (arg->getStmtClass() != clang::Stmt::BinaryOperatorClass) {
                ReportSizeofViolation(call_expr);
                return true;
            }
            
            auto binop = clang::dyn_cast<clang::BinaryOperator>(arg);
            clang::BinaryOperator::Opcode opcode = binop->getOpcode();
            if (opcode != clang::BinaryOperator::Opcode::BO_Mul) {
                ReportSizeofViolation(call_expr);
                return true;
            }

            if (!CheckSizeof(binop->getLHS()) && !CheckSizeof(binop->getRHS()))
                ReportSizeofViolation(call_expr);

            return true;
        }
    };
}

static oclint::RuleSet rules(new oclint::MallocSizeofRule());
