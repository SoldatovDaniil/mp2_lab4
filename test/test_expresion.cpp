#include <gtest.h>
#include "Ariphmetic.h"


using namespace std;
TEST(Expressions, can_make_exp)
{
	ASSERT_NO_THROW(Expression exp("1+3+4"));
}

TEST(Expressions, can_make_exp_right)
{
	Expression exp("1+3");
	vector<pair<string, Expression::priority>> tmp;
	tmp.push_back(make_pair("1", Expression::priority::number));
	tmp.push_back(make_pair("+", Expression::priority::add_or_sub));
	tmp.push_back(make_pair("3", Expression::priority::number));
	EXPECT_EQ(tmp, exp.data);
}

TEST(Expressions, can_throw_except_when_wrong_string) 
{
	ASSERT_ANY_THROW(Expression exp("1+3+4*6/9*12%2-0+2*3"));
}

TEST(Expressions, can_check_expr)
{
	Expression exp("1+3");
	ASSERT_NO_THROW(exp.check());
}

TEST(Expressions, can_check_return_true)
{
	Expression exp("1+3");
	EXPECT_EQ(true, exp.check());
}

TEST(Expressions, can_check_return_false)
{
	Expression exp("1+)3");
	EXPECT_EQ(false, exp.check());
}

TEST(Expressions, can_get_polish)
{
	Expression exp("1+3");
	Calculator calc(exp);
	calc.getPolish();
	ASSERT_NO_THROW(calc.getPolish());
}

TEST(Expressions, can_get_polish_right)
{
	Expression exp("2+3*4");
	string st1 = "234*+";
	Calculator calc(exp);
	calc.getPolish();
	string st2 = calc.polishToString();
	EXPECT_EQ(st1, st2);
}

TEST(Expressions, can_throw_except_when_div_by_zero)
{
	Expression exp("1+9/0");
	Calculator calc(exp);
	calc.getPolish();
	ASSERT_ANY_THROW(calc.perform());
}

TEST(Expressions, can_calculated) {

	Expression exp("1+9/3");
	Calculator calc(exp);
	calc.getPolish();
	cout << calc.perform();
	EXPECT_EQ(calc.perform(), "4");
}
