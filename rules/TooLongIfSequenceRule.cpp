#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;

class TooLongIfSequenceRule : public AbstractASTVisitorRule<TooLongIfSequenceRule>
{
private:
    int max_if_sequence_len;
    size_t getIfComplexity(IfStmt *if_stmt)
    {
        Stmt *else_stmt = if_stmt->getElse();

        if (else_stmt != nullptr && else_stmt->getStmtClass() == Stmt::IfStmtClass)
            return 1 + getIfComplexity(reinterpret_cast<IfStmt*>(else_stmt));

        return 1;
    }

public:
    virtual void setUp() override
    {
        this->max_if_sequence_len = RuleConfiguration::intForKey("MAX_IF_SEQUENCE_LEN", 5);
    }

    virtual void tearDown() override {}

    virtual const string name() const override
    {
        return "too long if sequence";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
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
        return "...";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example()
    {
        // TODO: modify the example for this rule.
    }
        )rst";
    }
#endif

    bool VisitCompoundStmt(CompoundStmt *node)
    {
        Stmt *first_if = nullptr;
        size_t cur_complexity = 0;

        for (Stmt::child_iterator it = node->child_begin(); it != node->child_end(); it++) {
            Stmt *stmt = (*it);

            if (stmt->getStmtClass() == Stmt::IfStmtClass) {
                if (first_if == nullptr)
                    first_if = stmt;

                cur_complexity += getIfComplexity(dyn_cast<IfStmt>(stmt));
            }
            else {
                if (first_if == nullptr) continue;

                if (cur_complexity > this->max_if_sequence_len) {
                    addViolation(first_if, this, toString<size_t>(cur_complexity) + " consecutive if statements is too many.");
                }

                first_if = nullptr;
                cur_complexity = 0;
            }
        }

        if (first_if != nullptr && cur_complexity > this->max_if_sequence_len) {
            addViolation(first_if, this, toString<size_t>(cur_complexity) + " consecutive if statements is too many.");
        }

        return true;
    }
};

static RuleSet rules(new TooLongIfSequenceRule());
