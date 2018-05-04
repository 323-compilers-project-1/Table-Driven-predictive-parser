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


queue<string> AsmTable::Assign(list<string> line)
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

queue<string> AsmTable::Expression(list<string> line)
{




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
