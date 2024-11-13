#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "clang/AST/Decl.h"

namespace oclint {
    class OnlyMainFunctionRule : public oclint::AbstractASTVisitorRule<OnlyMainFunctionRule>
    {
    private:
        std::vector<clang::FunctionDecl*> decls;
    public:
        virtual void tearDown() override
        {
            if (decls.size() == 1) {
                addViolation(decls[0], this, "only main function");
            }
        }

        virtual const std::string name() const override
        {
            return "only main function";
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
            return "consider breaking your main function into pieces";
        }

        virtual const std::string example() const override
        {
            return R"rst(for example see examples/ex-only-main/main.c 
            )rst";
        }
    #endif

        bool VisitFunctionDecl(clang::FunctionDecl *func_decl)
        {
            decls.push_back(func_decl);
            return true;
        }
    };

}

static oclint::RuleSet rules(new oclint::OnlyMainFunctionRule());
