#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "clang/AST/Expr.h"
#include <stack>

namespace oclint {
    class TooComplexConditionRule : public oclint::AbstractASTVisitorRule<TooComplexConditionRule>
    {
    private:
        int max_condition_complexity;
        size_t getStmtComplexity(clang::Stmt *stmt)
        {
        	std::stack<clang::Stmt*> stack;
            size_t result = 0;

            stack.push(stmt);

			while (!stack.empty()) {
				clang::Stmt *cur = stack.top();
				stack.pop();

				clang::Stmt::StmtClass sclass = cur->getStmtClass();
				if (sclass == clang::Stmt::BinaryOperatorClass) {
					auto *binop = clang::dyn_cast<clang::BinaryOperator>(cur);
					result += binop->isLogicalOp();
				}

				for (auto &child : cur->children()) {
					stack.push(child);
				}
			}

            return result;
        }

    public:
        virtual void setUp() override
        {
            this->max_condition_complexity = RuleConfiguration::intForKey("MAX_CONDITION_COMPLEXITY", 4);
        }

        virtual void tearDown() override {}

        virtual const std::string name() const override
        {
            return "too complex condition";
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
            return "Too complex conditions are hard to read.";
        }

        virtual const std::string example() const override
        {
            return R"rst(
    .. code-block:: cpp

        void example()
        {
            int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
            scanf("%d %d %d %d %d %d", &a, &b, &c, &d, &e, &f);

            if (((a == b || b == c) && (d == e || e == f) || !(d == c)) && b != e) {
                puts("Congrats! You found the secret code.");
            }
            else {
                puts("Better luck next time.");
            }

            return 0;
        }
            )rst";
        }
    #endif

        bool VisitIfStmt(clang::IfStmt *node)
        {
            clang::Expr *condition = node->getCond();
            size_t complexity = getStmtComplexity(clang::dyn_cast<clang::Stmt>(condition));

            if (complexity > this->max_condition_complexity) {
                addViolation(node, this, std::to_string(complexity) + " logical operators is too complex.");
            }

            return true;
        }
    };
}

static oclint::RuleSet rules(new oclint::TooComplexConditionRule());
