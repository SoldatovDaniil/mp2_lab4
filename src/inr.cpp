#include "../include/Ariphmetic.h"


int main()
{
	for (int i = 0; i < 20; i++)
	{
		cout << "-";
	}
	cout << "\nWelcome to Ariphmetic Expressions calculator\n";
	cout << "It support: integer numbers, operations '+', '-', '*', '/', and brackets\n";
	Expression exp = GetExpr();
	while (true)
	{
		if (exp.check())
		{
			cout << "\nYour expression is correct!\n";
			break;
		}
		else
		{
			cout << "\nYour expression is INCORRECT!\n";
			exp = GetExpr();
		}
	}

	cout << "\nANSWER\n" << exp << " = " << GetAnswer(exp);
	return 0;
}