#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <math.h>

using namespace std;
class Expression
{
public:
	enum class priority
	{
		number, 
		brecket,
		div_or_mul,
		add_or_sub
	};

	enum class statusCheck
	{
		start,
		erorr,
		w_operator,
		w_number,
		w_operator_or_close_bracket,
		w_number_or_open_bracket
	};

	enum class nstatusCheck
	{
		erorr,
		w_number,
		w_number_or_point,
		w_number_a_point
	};

	bool checkDoubValue(const string& num)
	{
		nstatusCheck status = nstatusCheck::w_number;
		for (int i = 0; i <= num.size(); i++)
		{
			switch (status)
			{
			case nstatusCheck::erorr:
				return false;
			case nstatusCheck::w_number:
				if (num[i] == '.')
				{
					status = nstatusCheck::erorr;
				}
				else
				{
					status = nstatusCheck::w_number_or_point;
				}
				break;
			case nstatusCheck::w_number_or_point:
				if (num[i] == '.')
				{
					status = nstatusCheck::w_number_a_point;
				}
				break;
			case nstatusCheck::w_number_a_point:
				if (num[i] == '.')
				{
					status = nstatusCheck::erorr;
				}
				break;
			default:
				break;
			}
		}
		return true;
	}

	vector<pair<string, priority>> data;

	Expression() = default;
	
	Expression(const string& str)
	{
		for (int i = 0; i < str.size(); i++)
		{
			pair<string, priority> tmp;
			tmp.first = "";
			if (str[i] == '+' || str[i] == '-')
			{
				tmp.first = str[i];
				tmp.second = priority::add_or_sub;
				data.push_back(tmp);
			}
			else if (str[i] == '*' || str[i] == '/')
			{
				tmp.first = str[i];
				tmp.second = priority::div_or_mul;
				data.push_back(tmp);
			}
			else if (str[i] == '(' || str[i] == ')')
			{
				tmp.first = str[i];
				tmp.second = priority::brecket;
				data.push_back(tmp);
			}
			else if (((str[i] >= 48) && (str[i] <= 57)) || str[i] == '.')
			{
				tmp.second = priority::number;
				do
				{
					tmp.first += str[i];
					i++;
				} while ((((str[i] >= 48) && (str[i] <= 57)) || str[i] == '.') && i < str.size());
				if (checkDoubValue(tmp.first) == true)
				{
					data.push_back(tmp);
					i--;
				}
				else
				{
					throw ("Error: wrong real number!");
				}
			}
			else
			{
				throw ("Error: unidentified symbol!");
			}
		}
	}

	Expression& operator=(const Expression& expr)
	{
		if (this == &expr)
		{
			return *this;
		}

		data = expr.data;
		return *this;
	}

	bool check()
	{
		statusCheck status = statusCheck::start;
		int breck_count = 0;
		for (int i = 0; i < data.size(); ++i)
		{
			switch (status)
			{
			case Expression::statusCheck::start:
				if (data[i].first == ")" || data[i].second == priority::div_or_mul || data[i].second == priority::add_or_sub)
				{
					status = statusCheck::erorr;
				}
				else if (data[i].second == priority::number)
				{
					status = statusCheck::w_operator;
				}
				else if (data[i].first == "(") 
				{
					status = statusCheck::w_number;
					breck_count++;
				}
				break;
			case Expression::statusCheck::w_number:
				if (data[i].second == priority::number)
				{
					status = statusCheck::w_operator_or_close_bracket; 
				}
				else
				{
					status = statusCheck::erorr;
				}
				break;
			case Expression::statusCheck::w_operator:
				if (data[i].second == priority::div_or_mul || data[i].second == priority::add_or_sub)
				{
					status = statusCheck::w_number_or_open_bracket;
				}
				else
				{
					status = statusCheck::erorr;
				}
				break;
			case Expression::statusCheck::w_number_or_open_bracket:
				if (data[i].second == priority::number)
				{
					status = statusCheck::w_operator_or_close_bracket; 
				}
				else if (data[i].first == "(")
				{
					status = statusCheck::w_number;
					breck_count++;
				}
				else
				{
					status = statusCheck::erorr;
				}
				break;
			case Expression::statusCheck::w_operator_or_close_bracket:
				if (data[i].first == ")")
				{
					status = statusCheck::w_operator;
					breck_count--;
				}
				else if (data[i].second == priority::add_or_sub || data[i].second == priority::div_or_mul)
				{
					status = statusCheck::w_number_or_open_bracket;
				}
				else
				{
					status = statusCheck::erorr;
				}
				break;
			case Expression::statusCheck::erorr:
				return false;
			default:
				break;
			}
		}

		if (status == statusCheck::w_number_or_open_bracket)
		{
			return false;
		}
		if (breck_count == 0)
		{
			return true;
		}
		return false;
	}

	int size() 
	{
		return data.size();
	}

	friend ostream& operator<<(ostream& out, const Expression& expres)
	{
		cout << "\nExpression: \n";
		for (int i = 0; i < expres.data.size(); i++) 
		{
			pair<string, priority> tmp = expres.data[i];
			out << tmp.first;
		}
		return out;
	}

};


class Calculator : private Expression
{
private:
	Expression express;
	vector<pair<string, priority>> polish;
public:
	Calculator(const Expression& expr)
	{
		express = expr;
		if (express.check() == false)
		{
			throw ("Error: wrong expression");
		}
	}

	vector<pair<string, priority>> getPolish()
	{
		stack<pair<string, priority>> tmpStack;
		for (int i = 0; i < express.data.size(); i++)
		{
			if (express.data[i].second == priority::number)
			{
				polish.push_back(express.data[i]);
			}
			else if (express.data[i].first == "(")
			{
				tmpStack.push(express.data[i]);
			}
			else if (express.data[i].first == ")")
			{
				while (tmpStack.top().first != "(")
				{
					polish.push_back(tmpStack.top());
					tmpStack.pop();
				}
				tmpStack.pop();
			}
			else if (express.data[i].second == priority::add_or_sub || express.data[i].second == priority::div_or_mul) 
			{
				while (!tmpStack.empty() && ((express.data[i].second == priority::add_or_sub && (tmpStack.top().second == priority::add_or_sub || tmpStack.top().second == priority::div_or_mul)) || (express.data[i].second == priority::div_or_mul && tmpStack.top().second == priority::div_or_mul))) 
				{
					polish.push_back(tmpStack.top());
					tmpStack.pop();
				}
				tmpStack.push(express.data[i]);
			}
		}

		while (!tmpStack.empty())
		{
			polish.push_back(tmpStack.top());
			tmpStack.pop();
		}

		return polish;
	}

	string perform() 
	{	
		stack<pair<string, priority>> tmpStack;
		for (int i = 0; i < polish.size(); i++) 
		{
			if (polish[i].second == priority::number) 
			{
				tmpStack.push(polish[i]);
			}
			else if (polish[i].second == priority::add_or_sub || polish[i].second == priority::div_or_mul)
			{
				if (polish[i].first == "+")
				{
					double rOper = stod(tmpStack.top().first);
					tmpStack.pop();
					double lOper = stod(tmpStack.top().first);
					tmpStack.pop();
					tmpStack.push(make_pair(to_string(lOper + rOper), priority::add_or_sub));
				}
				else if (polish[i].first == "-")
				{
					double rOper = stod(tmpStack.top().first);
					tmpStack.pop();
					double lOper = stod(tmpStack.top().first);
					tmpStack.pop();
					tmpStack.push(make_pair(to_string(lOper - rOper), priority::add_or_sub));
				}
				else if (polish[i].first == "*")
				{
					double rOper = stod(tmpStack.top().first);
					tmpStack.pop();
					double lOper = stod(tmpStack.top().first);
					tmpStack.pop();
					tmpStack.push(make_pair(to_string(lOper * rOper), priority::add_or_sub));
				}
				else if (polish[i].first == "/")
				{
					double rOper = stod(tmpStack.top().first);
					tmpStack.pop();
					double lOper = stod(tmpStack.top().first);
					tmpStack.pop();
					if (rOper == 0)
					{
						throw ("Error: division by zero!!!");
					}
					tmpStack.push(make_pair(to_string(lOper / rOper), priority::add_or_sub));
				}
			}
			
		}
		return tmpStack.top().first;
	}

	string polishToString() 
	{
		string tmp = "";
		for (int i = 0; i < polish.size(); i++)
			tmp += polish[i].first;
		return tmp;
	}

	void printPolish() {
		cout << "\nPolish of this expression: \n";
		for (int i = 0; i < polish.size(); i++)
		{
			cout << polish[i].first;
		}
	}
};

string GetAnswer(const Expression& expr)
{
	Calculator calc(expr);
	calc.getPolish();
	return(calc.perform());
}

Expression GetExpr()
{
	string str;
	cout << "Input your expression: ";
	cin >> str;
	Expression expr(str);
	return expr;
}


