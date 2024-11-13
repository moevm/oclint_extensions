#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "clang/Basic/Specifiers.h"

namespace oclint {
    class StringCompareRule : public oclint::AbstractASTVisitorRule<StringCompareRule>
    {
    private:
        using Opcode = clang::BinaryOperator::Opcode;

    public:
        virtual void setUp() override {}
        virtual void tearDown() override {}

        virtual const std::string name() const override
        {
            return "string compare";
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
            return "In some cases, the '==' operator will give the correct result "
                   "(if a pointer to a constant string is written to a variable and "
                   "then compared with this constant string), but in a general "
                   "case it will inevitably lead to an error";
        }

        virtual const std::string example() const override
        {
            return R"rst(
    .. code-block:: cpp
        #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #define INITIAL_SIZE 8

    void example()
    {
        int arr_size = 0;
        char **arr = NULL;
        
        // ... (input arr and arr_size)

        int cnt = 1;
        for (int i = 0; i < arr_size; i++) {
            if (arr[0] == arr[i])               // bad
                cnt++;
        }

        printf("%d\n", cnt);
        
        // ... (free memory)
        
        return;
    })rst";
    #endif

        bool IsString(clang::Expr *expr)
        {
            clang::QualType type = expr->getType();
            if (!type->isPointerType())
                return false;

            type = type->getPointeeType();
            return type->isAnyCharacterType() || type.getAsString() == "wchar_t";
        }

        bool IsNull(clang::Expr *expr)
        {
            return expr->isNullPointerConstant(*this->_carrier->getASTContext(), clang::Expr::NPC_ValueDependentIsNotNull);
        }

        const char *OpcodeAsString(Opcode opcode)
        {
            switch (opcode) {
                case Opcode::BO_EQ:
                    return "'=='";
                case Opcode::BO_NE:
                    return "'!='";
                default:
                    return "???";
            }
        }

        bool VisitBinaryOperator(clang::BinaryOperator *binop)
        {
            Opcode opcode = binop->getOpcode();

            if (opcode != Opcode::BO_EQ && opcode != Opcode::BO_NE)
                return true;

            clang::Expr *left  = binop->getLHS();
            clang::Expr *right = binop->getRHS();

            if (!IsNull(left) && !IsNull(right) && (IsString(left) || IsString(right)))
                addViolation(binop, this, std::string("comparing 2 strings by using ") + OpcodeAsString(opcode) + ".");

            return true;
        }
    };
}

static oclint::RuleSet rules(new oclint::StringCompareRule());
