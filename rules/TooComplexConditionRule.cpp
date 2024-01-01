#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"
#include "clang/AST/Expr.h"

using namespace std;
using namespace clang;
using namespace oclint;

class TooComplexConditionRule : public AbstractASTVisitorRule<TooComplexConditionRule>
{
private:
    int max_condition_complexity;
    size_t getStmtComplexity(Stmt *stmt)
    {
        size_t result = 0;

        Stmt::StmtClass sclass = stmt->getStmtClass();   
        if (sclass == Stmt::BinaryOperatorClass) {
            BinaryOperator *binop = reinterpret_cast<BinaryOperator*>(stmt);
            result += binop->isLogicalOp();
        }

        for (Stmt::child_iterator child = stmt->child_begin(); child != stmt->child_end(); child++) {
            result += getStmtComplexity(*child);
        }

        return result;
    }

public:
    virtual void setUp() override
    {
        this->max_condition_complexity = RuleConfiguration::intForKey("MAX_CONDITION_COMPLEXITY", 4);
    }

    virtual void tearDown() override {}

    virtual const string name() const override
    {
        return "too complex condition";
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

    bool VisitIfStmt(IfStmt *node)
    {
        Expr *condition = node->getCond();
        size_t complexity = getStmtComplexity(reinterpret_cast<Stmt*>(condition));

        if (complexity > this->max_condition_complexity) {
            addViolation(node, this, toString<size_t>(complexity) + " logical operators is too complex.");
        }

        return true;
    }
};

static RuleSet rules(new TooComplexConditionRule());
