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
		w_open_bracket,
		w_close_bracket,
		w_operator_or_bracket,
		w_number_or_bracket
	};

	vector<pair<string, priority>> data;

	Expression() = default;
	
	Expression(const string& str)
	{
		pair<string, priority> tmp;
		for (int i = 0; i < str.size(); i++)
		{
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
			else if ((str[i] >= 49) && (str[i] <= 57))
			{
				tmp.second = priority::brecket;
				while (((str[i] >= 49) && (str[i] <= 57)) && i < str.size())
				{
					tmp.first += str[i];
					i++;
				}
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
					status = statusCheck::w_operator_or_bracket;
				}
				else if (data[i].first == "(") 
				{
					status = statusCheck::w_number;
					breck_count++;
				}
				else if (data[i].second == priority::number)
				{
					status = statusCheck::w_operator_or_bracket;
				}
				break;
			case Expression::statusCheck::w_number:
				if (data[i].second == priority::number)
				{
					status = statusCheck::w_operator_or_bracket; 
				}
				else
				{
					status = statusCheck::erorr;
				}
				break;
			case Expression::statusCheck::w_operator:
				if (data[i].second == priority::div_or_mul || data[i].second == priority::add_or_sub)
				{
					status = statusCheck::w_number_or_bracket;
				}
				else
				{
					status = statusCheck::erorr;
				}
				break;
			case Expression::statusCheck::w_open_bracket:
				if (data[i].first == "(")
				{
					status = statusCheck::w_number;
					breck_count++;
				}
				else
				{
					status = statusCheck::erorr;
				}
				break;
			case Expression::statusCheck::w_close_bracket:
				if (data[i].first == ")")
				{
					status = statusCheck::w_operator;
					breck_count--;
				}
				else
				{
					status = statusCheck::erorr;
				}
				break;
			case Expression::statusCheck::w_number_or_bracket:
				if (data[i].second == priority::number)
				{
					status = statusCheck::w_operator_or_bracket; 
				}
				else if (data[i].second == priority::brecket)
				{
					status = statusCheck::w_number;
					breck_count++;
				}
				else
				{
					status = statusCheck::erorr;
				}
				break;
			case Expression::statusCheck::w_operator_or_bracket:
				if (data[i].second == priority::brecket)
				{
					status = statusCheck::w_operator;
					breck_count--;
				}
				else if (data[i].second == priority::add_or_sub || data[i].second == priority::div_or_mul)
				{
					status = statusCheck::w_number_or_bracket;
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

	friend ostream& operator<<(ostream& out, const Expression& expr)
	{
		for (int i = 0; i < expr.data.size(); i++)
		{
		}
		return out;
	}

};

