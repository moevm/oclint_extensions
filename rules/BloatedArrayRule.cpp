#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "clang/AST/Type.h"
#include <iostream>

namespace oclint {
    class BloatedArrayRule : public oclint::AbstractASTVisitorRule<BloatedArrayRule>
    {
    private:
        int threshold;
    public:
        virtual void setUp() override
        {
            threshold = RuleConfiguration::intForKey("MAX_DIMENSIONS", 2);
        }

        virtual void tearDown() override {}

        virtual const std::string name() const override
        {
            return "very bloated array";
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
            return "'Very' multidimentional arrays are hard to read and to use. Maybe you can rewrite your code without using so many dimensions.";
        }

        virtual const std::string example() const override
        {
            return "See example in 'examples/ex-bloated-array'";
        }
    #endif

        int GetDepth(const clang::Type *type)
        {
            int depth = 0;

            while (type->isArrayType() || type->isAnyPointerType())
            {
                type = type->isArrayType() ? type->getArrayElementTypeNoTypeQual() : type->getPointeeType().getTypePtr();
                depth++;
            }

            return depth;
        }

        void ReportBloated(clang::Decl *decl, int depth)
        {
            addViolation(decl, this, std::to_string(depth) + " dimensions is too much (threshold = " + std::to_string(threshold) + ")");
        }

        bool VisitFunctionDecl(clang::FunctionDecl *function_decl)
        {
            int depth = GetDepth(function_decl->getReturnType().getTypePtr());
            if (depth > threshold) {
                ReportBloated(clang::dyn_cast<clang::Decl>(function_decl), depth);
            }

            return true;
        }

        bool VisitVarDecl(clang::VarDecl *var_decl)
        {
            int depth = GetDepth(var_decl->getType().getTypePtr());
            if (depth > threshold) {
                ReportBloated(clang::dyn_cast<clang::Decl>(var_decl), depth);
            }

            return true;
        }
    };
}

static oclint::RuleSet rules(new oclint::BloatedArrayRule());
