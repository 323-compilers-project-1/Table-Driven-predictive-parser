#include "stdafx.h"
#include "AsmTable.h"
#include <algorithm>
//#include "SymbolTable.h"


AsmTable::AsmTable(SymbolTable sm)
{
	this->sm = sm;
	this->lexemes = sm.lexemes;



}

#pragma region ASM TABLE generation

#pragma region getLine functions
list<string> AsmTable::getLine(string delimiter)
{
	list<string>::iterator it = this->lexemes.begin();

	list<string>::iterator end;

	end = find(this->lexemes.begin(), this->lexemes.end(), delimiter);

	if (end != this->lexemes.end())
	{
		list<string> tmp(it, ++end);
		
		this->lexemes.erase(it, ++end);


		return tmp;
	}
	else
	{
		list<string> tmp;
		return tmp;
	}


}

list<string> AsmTable::getLine(string first, string last)
{
	list<string>::iterator it = find(this->lexemes.begin(), this->lexemes.end(), first);

	list<string>::iterator end;

	end = find(this->lexemes.begin(), this->lexemes.end(), last);

	if (end != this->lexemes.end())
	{
		list<string> tmp(it, ++end);

		this->lexemes.erase(it, ++end);


		return tmp;
	}
	else
	{
		list<string> tmp;
		return tmp;
	}
}
#pragma endregion


void AsmTable::Assign(list<string> line)
{
	list<string>::iterator first = find(line.begin(), line.end(), "=");

	list<string>::iterator last = find(first, line.end(), ";");

	list<string> tmp(first, last);

	queue<string> postfix = Expression(tmp);

	int tableNum = 0;

	

	while (postfix.size() > 0)
	{
		asmTableInput input;

		int address = 0;
		string tmp = postfix.front();

		address = sm.getAddress(tmp);

		if (address != -1)
		{
			input.line = ++tableNum;
			input.address = address;
			input.action = "PUSHM";

			asmTable.push_back(input);
			postfix.pop();

		}
		else
		{
			//check for ints
			if (isdigit(tmp[0]))
			{
				input.line = ++tableNum;
				input.action = "PUSHI";
				input.address = stoi(tmp);

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "*")
			{
				input.line = ++tableNum;
				input.action = "MUL";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();


			}
			else if (tmp == "/")
			{
				input.line = ++tableNum;
				input.action = "DIV";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();


			}
			else if (tmp == "+")
			{

				input.line = ++tableNum;
				input.action = "ADD";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "-")
			{

				input.line = ++tableNum;
				input.action = "SUB";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}

			 


		}

		






	}
	
	asmTableInput popm;

	popm.line = ++tableNum;
	popm.action = "POPM";
	popm.address = sm.getAddress(*line.begin());

	asmTable.push_back(popm);


}

queue<string> AsmTable::expression(list<string> ex) 
{
	queue<string> result;
	
	list<string>::iterator it = find(ex.begin(), ex.end(), "(");
	while (it != ex.end())
	{
		list<string>::iterator start_of_paren = it;
		list<string>::iterator end_of_paren = find(ex.begin(), ex.end(), ")");

		list<string> temp;
		temp.splice(temp.begin(), ex, std::next(start_of_paren, 1), end_of_paren);

		queue<string> paren_result;

		for (list<string>::iterator i = temp.begin(); i != temp.end(); i++)
		{
			for (int j = 0; j < sm.symbolTable.size(); j++)
			{
				if (*i == sm.symbolTable[j].id)
				{
					paren_result.push(*i);
				}
			}
		}

		for (list<string>::iterator i = temp.begin(); i != temp.end(); i++)
		{
			if (*i == "*" || *i == "/")
			{
				paren_result.push(*i);
			}
		}

		for (list<string>::iterator i = temp.begin(); i != temp.end(); i++)
		{
			if (*i == "+" || *i == "-")
			{
				paren_result.push(*i);
			}
		}

		while(paren_result.size() != 0)
		{
			result.push(paren_result.front());
			paren_result.pop();
		}

		ex.erase(start_of_paren, std::next(end_of_paren, 1));
		it = find(ex.begin(), ex.end(), "(");
	}

	it = ex.begin();
	while (it != ex.end())
	{
		for (int j = 0; j < sm.symbolTable.size(); j++)
		{
			if (*it == sm.symbolTable[j].id)
			{
				result.push(*it);
			}
		}
		it++;
	}

	it = ex.begin();
	while (it != ex.end())
	{
		if (*it == "*" || *it == "/") 
		{
			result.push(*it);
		}
		it++;
	}

	it = ex.begin();
	while (it != ex.end())
	{
		if (*it == "+" || *it =="-")
		{
			result.push(*it);
		}
		it++;
	}

	return result;
}

/*vector<asmTableInput> AsmTable::table()
{





}
*/




void AsmTable::makeAsmTable()
{
	list<string> lineMarker;
	while (this->lexemes.size() > 0)
	{
		list<string>::iterator first;

		lineMarker = getLine(";");

		first = lineMarker.begin();

		if (*first == "while")
		{
			//While();

		}
		else if (*first == "if")
		{
			//If();
		}
		else if (*first == "get")
		{

			//Get()

		}
		else if (*first == "put")
		{
			//Put();
		}
		else if (sm.getAddress(*first) != -1)
		{
			//Assign();
		}

	}
	


}


#pragma endregion

AsmTable::~AsmTable()
{
}
