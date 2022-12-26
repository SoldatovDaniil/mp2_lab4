#include "../include/Ariphmetic.h"


void interface()
{
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
}


int main()
{
	for (int i = 0; i < 20; i++)
	{
		cout << "-";
	}
	cout << "\nWelcome to Ariphmetic Expressions calculator\n";
	cout << "It support: integer and real numbers, operations '+', '-', '*', '/', and brackets\n";
	int i = 0;
	while (i < 1)
	{
		try
		{
			interface();
		}
		catch (const char* message)
		{
			cout << message << "\nPlease, retry\n";
			--i;
		}
		i++;
	}
	return 0;
}