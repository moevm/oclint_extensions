#include "TestRuleOnCode.h"
#include "rules/etu/TooManyConsecutiveIfStatementsRule.cpp"

TEST(TooManyConsecutiveIfStatementsRuleTest, PropertyTest)
{
    TooManyConsecutiveIfStatementsRule rule;
    EXPECT_EQ(2, rule.priority());
    EXPECT_EQ("too many consecutive if statements", rule.name());
    EXPECT_EQ("etu", rule.category());
}

TEST(TooManyConsecutiveIfStatementsRuleTest, FirstFailingTest)
{
    EXPECT_FALSE("Start writing a new test");
}
