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
			else if ((str[i] >= 48) && (str[i] <= 57))
			{
				tmp.second = priority::number;
				do
				{
					tmp.first += str[i];
					i++;
				} while (((str[i] >= 48) && (str[i] <= 57)) && i < str.size());
				i--;
				data.push_back(tmp);
			}
			else
			{
				throw ("Error: unidentified symbol");
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
		for (int i = 0; i < data.size(); i++)
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
				break;
			default:
				break;
			}
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
				polish.push_back(make_pair(express.data[i].first, express.data[i].second));
			}
			else if (express.data[i].first == "(")
			{
				tmpStack.push(make_pair(express.data[i].first, express.data[i].second));
			}
			else if (express.data[i].first == ")")
			{
				while (tmpStack.top().first != "(")
				{
					polish.push_back(make_pair(tmpStack.top().first, tmpStack.top().second));
					tmpStack.pop();
				}
				tmpStack.pop();
			}
			else if (express.data[i].second == priority::add_or_sub)
			{
				if (tmpStack.empty())
				{
					tmpStack.push(make_pair(express.data[i].first, express.data[i].second));
				}
				else if (tmpStack.top().first == "+" || tmpStack.top().first == "-" || tmpStack.top().first == "/" || tmpStack.top().first == "*")
				{
					if ((express.data[i].first == "-") && (express.data[i + 1].second == priority::number))
					{
						polish.push_back(make_pair(express.data[i + 1].first, express.data[i + 1].second));
						i++;
					}
					else
					{
						polish.push_back(tmpStack.top());
						tmpStack.pop();
						tmpStack.push(make_pair(express.data[i].first, express.data[i].second));
					}
				}
			}
			else if (express.data[i].second == priority::div_or_mul)
			{
				if (tmpStack.empty())
				{						
					tmpStack.push(make_pair(express.data[i].first, express.data[i].second));
				}
				else if (tmpStack.top().first == "*" || tmpStack.top().first == "/")
				{
					polish.push_back(tmpStack.top());
					tmpStack.pop();
					tmpStack.push(make_pair(express.data[i].first, express.data[i].second));
				}
				else
				{
					tmpStack.push(make_pair(express.data[i].first, express.data[i].second));
				}
			}
		}

		while (!tmpStack.empty())
		{
			if (tmpStack.top().first == "(" || tmpStack.top().first == ")")
			{
				tmpStack.top();
			}
			else
			{
				polish.push_back(tmpStack.top());
				tmpStack.pop();
			}
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
			else
			{
				int left = stoi(tmpStack.top().first);
				tmpStack.pop();
				int right = stoi(tmpStack.top().first);
				tmpStack.pop();
				if (polish[i].first == "+")
				{
					tmpStack.push(make_pair(to_string(left + right), priority::add_or_sub));
				}
				else if (polish[i].first == "-")
				{
					tmpStack.push(make_pair(to_string(left - right), priority::add_or_sub));
				}
				else if (polish[i].first == "*")
				{
					tmpStack.push(make_pair(to_string(left * right), priority::div_or_mul));
				}
				else if (polish[i].first == "/")
				{
					if (left == 0)
					{
						throw ("Error: Division by zero");
					}
					tmpStack.push(make_pair(to_string(right / left), priority::div_or_mul));
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

