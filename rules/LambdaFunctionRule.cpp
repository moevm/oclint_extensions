#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"

class NestedFunctionRule : public oclint::AbstractASTVisitorRule<NestedFunctionRule>
{
private:
    std::vector<clang::FunctionDecl*> decls;
public:
    virtual const std::string name() const override
    {
        return "lambda function";
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
        return "consider removing these functions from your code";
    }

    virtual const std::string example() const override
    {
        return R"rst(for example see examples/ex-lambda/main.cpp 
        )rst";
    }
#endif

    bool VisitLambdaExpr(clang::LambdaExpr *expr)
    {
        addViolation(expr, this, "using lambda functions is bad");
        return true;
    }
};

static oclint::RuleSet rules(new NestedFunctionRule());
