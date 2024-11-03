#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"

namespace oclint {
    class GlobalVariableRule : public oclint::AbstractASTVisitorRule<GlobalVariableRule>
    {
    public:
        virtual void tearDown() override {}

        virtual const std::string name() const override
        {
            return "global variable";
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
            return "...";
        }

        virtual const std::string example() const override
        {
            return R"rst(for example see examples/ex-global/main.c 
            )rst";
        }
    #endif

        bool VisitVarDecl(clang::VarDecl *var_decl)
        {
            if (var_decl->hasGlobalStorage())
                addViolation(var_decl, this, "using static local or global variable '" + var_decl->getNameAsString() + '\'');
            return true;
        }
    };
}

static oclint::RuleSet rules(new oclint::GlobalVariableRule());
