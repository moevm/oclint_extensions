#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include <stack>

namespace oclint {
    class TooLongIfSequenceRule : public oclint::AbstractASTVisitorRule<TooLongIfSequenceRule>
    {
    private:
        int max_if_sequence_len;
        clang::IfStmt *first_if;
        size_t cur_complexity;

        struct {
            bool binary_operator : 1;
            bool function_call : 1;
        } special_check_flags;

    public:
        virtual void setUp() override
        {
            this->max_if_sequence_len = oclint::RuleConfiguration::intForKey("MAX_IF_SEQUENCE_LEN", 5);
            this->special_check_flags.binary_operator = oclint::RuleConfiguration::intForKey("CHECK_BINARY_OPERATOR", 1);
            this->special_check_flags.function_call   = oclint::RuleConfiguration::intForKey("CHECK_FUNCTION_CALL", 1);
        }

        virtual void tearDown() override {}

        virtual const std::string name() const override
        {
            return "too long if sequence";
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
            return "Such if sequences are difficult to analyze and may be rewritten more efficiently.";
        }

        virtual const std::string example() const override
        {
            return R"rst(See example in examples/ex-if)rst";
        }
    #endif

        bool specialStatementCheck(clang::Stmt *first, clang::Stmt *second)
        {
            using clang::dyn_cast;

            switch (first->getStmtClass()) {
                case clang::Stmt::BinaryOperatorClass:
                    if (!this->special_check_flags.binary_operator) return true;
                    return dyn_cast<clang::BinaryOperator>(first)->getOpcode() == dyn_cast<clang::BinaryOperator>(second)->getOpcode();
                
                case clang::Stmt::CallExprClass:
                    if (!this->special_check_flags.function_call) return true;
                    return dyn_cast<clang::CallExpr>(first)->getCalleeDecl() == dyn_cast<clang::CallExpr>(second)->getCalleeDecl();
                default:
                    return true;
            }
        }

        bool areStatementsSimilar(clang::Stmt *first, clang::Stmt *second)
        {
            std::stack<clang::Stmt *> stack1;
            std::stack<clang::Stmt *> stack2;
            stack1.push(first);
            stack2.push(second);

            while (!stack1.empty() &&!stack2.empty())
            {
                clang::Stmt *stmt1 = stack1.top(); stack1.pop();
                clang::Stmt *stmt2 = stack2.top(); stack2.pop();

                if (stmt1->getStmtClass() != stmt2->getStmtClass())
                    return false;
                if (!specialStatementCheck(stmt1, stmt2))
                    return false;

                clang::Stmt::child_iterator it1 = stmt1->child_begin();
                clang::Stmt::child_iterator it2 = stmt2->child_begin();
                while (it1 != stmt1->child_end() && it2 != stmt2->child_end()) {
                    stack1.push(*it1);
                    stack2.push(*it2);
                    ++it1;
                    ++it2;
                }

                if (it1 != stmt1->child_end() || it2 != stmt2->child_end())
                    return false;
            }

            return true;
        }

        void endSequence()
        {
            if (first_if == nullptr)
                return;

            if (cur_complexity > this->max_if_sequence_len) {
                addViolation(
                    this->first_if,
                    this,
                    std::to_string(this->cur_complexity) + " consecutive if statements is too many."
                );
            }

            first_if = nullptr;
            cur_complexity = 0;
        }

        clang::IfStmt *getNext(clang::IfStmt *if_stmt)
        {
            clang::Stmt *stmt = if_stmt->getElse();

            if (stmt == nullptr) {
                return nullptr;
            }

            if (stmt->getStmtClass() == clang::Stmt::IfStmtClass) {
                return clang::dyn_cast<clang::IfStmt>(stmt);
            }
            
            // i guess there is no such thing as stmt->children.size() in oclint...
            if (stmt->getStmtClass() == clang::Stmt::CompoundStmtClass
                && (stmt->child_begin() != stmt->child_end() && ++stmt->child_begin() == stmt->child_end())
                && stmt->child_begin()->getStmtClass() == clang::Stmt::IfStmtClass
            ) {
                addViolation(
                    if_stmt,
                    this,
                    "using compound else statement for a single if"
                );
            }

            return nullptr;
        }

        void visitIfElseChain(clang::IfStmt *head)
        {
            if (first_if == nullptr)
                first_if = head;
            
            clang::IfStmt *cur = head;
            while (cur != nullptr) {
                if (!areStatementsSimilar(first_if->getCond(), cur->getCond())) {
                    this->endSequence();
                    first_if = cur;
                }
                
                this->cur_complexity++;
                cur = getNext(cur);
            }
        }

        bool VisitCompoundStmt(clang::CompoundStmt *node)
        {
            this->first_if = nullptr;
            this->cur_complexity = 0;

            for (auto &stmt : node->children()) {
                if (stmt->getStmtClass() == clang::Stmt::IfStmtClass) {
                    this->visitIfElseChain(clang::dyn_cast<clang::IfStmt>(stmt));
                }
                else {
                    this->endSequence();
                }
            }

            this->endSequence();
            return true;
        }
    };
}

static oclint::RuleSet rules(new oclint::TooLongIfSequenceRule());
