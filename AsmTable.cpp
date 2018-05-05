#include "AsmTable.h"
#include <algorithm>
#include <iostream>
#include "SymbolTable.h"

using namespace std;



#pragma region Constructors

AsmTable::AsmTable()
{
	lineNum = 0;

}

AsmTable::AsmTable(SymbolTable sm)
{
	this->sm = sm;
	this->lexemes = sm.lexemes;
	this->lexemes.pop_front();
	lineNum = 0;
}

#pragma endregion

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
		
		this->lexemes.erase(it, end);


		return tmp;
	}
	else
	{
		list<string> tmp;
		return tmp;
	}


}

list<string> AsmTable::getLine(list<string> lexemes, string delimiter)
{
	list<string>::iterator it = lexemes.begin();

	list<string>::iterator end;

	end = find(lexemes.begin(), lexemes.end(), delimiter);

	if (end != lexemes.end())
	{
		list<string> tmp(it, ++end);

		lexemes.erase(it, end);


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

		this->lexemes.erase(it, end);


		return tmp;
	}
	else
	{
		list<string> tmp;
		return tmp;
	}
}
#pragma endregion

#pragma region Functions

queue<string> AsmTable::Expression(list<string> ex)
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
		
		//gets id
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
		///gets higher order 
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

		while (paren_result.size() != 0)
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
		if (*it == "+" || *it == "-")
		{
			result.push(*it);
		}
		it++;
	}

	return result;
}

void AsmTable::Assign(list<string> line)
{
	list<string>::iterator first = find(line.begin(), line.end(), "=");

	list<string>::iterator last = find(first, line.end(), ";");

	list<string> tmp(first, ++last);

	queue<string> postfix = Expression(tmp);

	fillTable(postfix);

	/*while (postfix.size() > 0)
	{
		asmTableInput input;

		int address = 0;
		string tmp = postfix.front();

		address = this->sm.getAddress(tmp);

		if (address != -1)
		{
			input.line = ++lineNum;
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
				input.line = ++lineNum;
				input.action = "PUSHI";
				input.address = stoi(tmp);

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "*")
			{
				input.line = ++lineNum;
				input.action = "MUL";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();


			}
			else if (tmp == "/")
			{
				input.line = ++lineNum;
				input.action = "DIV";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();


			}
			else if (tmp == "+")
			{

				input.line = ++lineNum;
				input.action = "ADD";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "-")
			{

				input.line = ++lineNum;
				input.action = "SUB";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "==")
			{

				input.line = ++lineNum;
				input.action = "EQU";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "^=")
			{

				input.line = ++lineNum;
				input.action = "NEQ";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "=<")
			{

				input.line = ++lineNum;
				input.action = "LEQ";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "=>")
			{

				input.line = ++lineNum;
				input.action = "GEQ";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "<")
			{

				input.line = ++lineNum;
				input.action = "LES";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == ">")
			{

				input.line = ++lineNum;
				input.action = "GRT";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}

			 


		}

		

	}
	*/
	asmTableInput popm;

	popm.line = ++lineNum;
	popm.action = "POPM";
	popm.address = sm.getAddress(*line.begin());

	asmTable.push_back(popm);


}


bool AsmTable::isSeparetor(string to_lex)
{
	const int SEPERATOR_LIST_SIZE = 9;
	const string seperator_list[SEPERATOR_LIST_SIZE] = { ",", ";", "{", "}", "(" , ")" , "[" , "]" };


	for (int i = 0; i < SEPERATOR_LIST_SIZE; i++)
	{
		if (to_lex == seperator_list[i])
		{
			return true;

		}

	}
	return false;

}

 list<string> AsmTable::getLastDelimiter(list<string> line,string delim, int *count)
{
	
	 list<string>::iterator placeHolder;
	 if (delim == "}" ) {
		 int delim_layer = 0;
		 list<string>::iterator beg = line.begin();
		 while (beg != line.end())
		 {
			 if (delim == "}" && *beg == "{")
			 {
				 delim_layer++;
			 }
			 beg++;
		 }
		 //have to find a way to not make this true
		 list<string>::iterator placeHolder;
		 for (list<string>::iterator i = line.begin(); i != line.end(); i++)
		 {
			 //can only be used for while() or ()
			 //we can try to make this more universal
			 if (*i == delim)
			 {
				 (*count)++;
				 placeHolder = i;
				 list<string> tmp(line.begin(), ++placeHolder);
				 if (*count == delim_layer)
				 {
					 return tmp;
				 }
			 }
			 else if (*i == delim && (*(++i) == ";" || *(++i) == "{"))
			 {
				 --i;
				 placeHolder = i;
				 list<string> tmp(line.begin(), ++placeHolder);
				 return tmp;
			 }
			 else if (*i == delim)
			 {
				 (*count)++;
			 }


		 }
	 }

	 if (delim == ")" || delim == "endif")
	 {
		 string open;
		 string close;
		 if (delim == ")")
		 {
			 open = "(";
			 close = ")";
		 }
		 else if (delim == "endif")
		 {
			 open = "if";
			 close = "endif";
		 }
		 int delim_layer = 0;
		 list<string>::iterator beg = line.begin();
		 list<string>::iterator it = beg;
		 while (it != line.end())
		 {
			 if (*it == open)
			 {
				 delim_layer++;
			 }
			 else if (*it == close)
			 {
				 delim_layer--;
				 if (delim_layer == 0)
				 {
					 list<string> result(beg, ++it);
					 return result;
				 }
			 }
			 it++;
		 }
	 }
	
	list<string> tmp;
	return tmp;

	

}



queue<string> AsmTable::RelopExpression(list<string> line)
{
	queue<string> result;
	list<string>::iterator first = line.begin();
	int count = 0;
	list<string>::iterator last;
	list<string> tmp = getLastDelimiter(line, ")", &count);


	first = find(line.begin(), line.end(), *(tmp.begin()));
	last = find(line.begin(), line.end(), *(--tmp.end()));

	line.erase(first, last);

	if (count == 1)
	{
		//while (  ) simple case
		for (list<string>::iterator i = tmp.begin(); i != tmp.end(); i++)
		{
			for (int j = 0; j < sm.symbolTable.size(); j++)
			{
				if (*i == sm.symbolTable[j].id)
				{
					result.push(*i);
				}
			}
		}

		for (list<string>::iterator i = tmp.begin(); i != tmp.end(); i++)
		{
			if (*i == "==" || *i == "^=" || *i == "=<"  || *i == "=>" || *i == ">" || *i == "<")  // == | ^= | > | < | => | =< 
			{
				result.push(*i);
			}
		}
	}
	else if ((count % 2) == 0)
	{
		//while     ( () ^=  () )

		//removing first paranthesis
		tmp.pop_front();
		for (int i = 0; i <= count; i++)
		{
			// ( ( points to second paranthesis
			first = find( tmp.begin(), tmp.end(), "(");
			last = find(first, tmp.end(), ")");
			
			list<string> paren(first, ++last);
			tmp.erase(first, last);
			 
			queue<string> currentQueue = Expression(paren);

			for (int i = 0; i < currentQueue.size(); i++)
			{
				result.push(currentQueue.front());
				currentQueue.pop();

			}



		}

		for (list<string>::iterator i = tmp.begin(); i != tmp.end(); i++)
		{
			for (int j = 0; j < sm.symbolTable.size(); j++)
			{
				if (*i == sm.symbolTable[j].id)
				{
					result.push(*i);
				}
			}
		}

		for (list<string>::iterator i = tmp.begin(); i != tmp.end(); i++)
		{
			if (*i == "==" || *i == "^=" || *i == "=<" || *i == "=>" || *i == ">" || *i == "<")  // == | ^= | > | < | => | =< 
			{
				result.push(*i);
			}
		}

	


	}

	
	return result;
	
	

}


void AsmTable::If(list<string> line)
{
	queue<string> result;

	list<string>::iterator first = line.begin();
	list<string>::iterator last = --line.end();
	list<string>::iterator end;

	asmTableInput entry;

	entry.line = ++lineNum;
	entry.action = "LABEL";
	entry.address = 0;

	asmTable.push_back(entry);
	line.pop_front();

	list<string> condition = getLastDelimiter(line, ")", 0);
	first = find(line.begin(), line.end(), *(condition.begin()));
	end = std::next(first, condition.size() - 1);

	line.erase(first, ++end);
	
	list<string> left_condition;
	list<string> right_condition;
	
	list<string>::iterator condition_it = ++condition.begin();
	string condition_relop;


	vector<string> relops = { "<", ">", "=<", "=>", "==", "^=" };
	
	for (condition_it; condition_it != condition.end(); condition_it++)
	{
		for (int i = 0; i < relops.size(); i++) 
		{
			if (*condition_it == relops[i])
			{
				condition_relop = relops[i];
				for (list<string>::iterator left = ++condition.begin(); left != condition_it; left++)
				{
					left_condition.push_back(*left);
				}
				for (list<string>::iterator right = ++condition_it; right != --condition.end(); right++)
				{
					right_condition.push_back(*right);
				}
			}
		}
	}

	queue<string> left_expression = Expression(left_condition);
	queue<string> right_expression = Expression(right_condition);

	vector<string> badIdea; //this part is a bad idea

	while (left_expression.size() > 0)
	{
		badIdea.push_back(left_expression.front());
		left_expression.pop();
	}
	while (right_expression.size() > 0)
	{
		badIdea.push_back(right_expression.front());
		right_expression.pop();
	}

	badIdea.push_back(condition_relop);
	for (int i = 0; i < badIdea.size(); i++)
	{
		result.push(badIdea[i]);
	}
	fillTable(result);

	if (line.front() == "{")
	{
		lineNum++;
		entry.line = lineNum;
		entry.action = "JMPZ";
		entry.address = 0;

		asmTable.push_back(entry);
		jmpstack.push(entry.line);


		line.pop_front();
		line.pop_back();
		line.pop_back();
	}

	while (line.size() > 0)
	{
		list<string>::iterator first = line.begin();

		//to pop the } in front of else
		if (line.front() == "}")
		{
			line.pop_front();
			first = line.begin();
		}

		if (*first == "else")
		{
			line.pop_front();
			line.pop_front();
			first = line.begin();
		}

		if (*first == "while")
		{
			//While();
			int delimNum;
			list<string>::iterator end_it;
			int count = 0;
			list<string> lineMarker = getLastDelimiter(line, "}", &count);

			first = find(line.begin(), line.end(), *(lineMarker.begin()));
			end_it = find(line.begin(), line.end(), *(--lineMarker.end()));

			line.erase(first, ++end_it);
			While(lineMarker);

		}
		else if (*first == "if")
		{
			list<string>::iterator end_it;
			list<string> lineMarker = getLastDelimiter(line, "endif", 0);

			first = find(line.begin(), line.end(), *(lineMarker.begin()));
			end_it = find(line.begin(), line.end(), *(--lineMarker.end()));

			line.erase(first, ++end_it);
			If(lineMarker);
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
			list<string> lineMarker;
			lineMarker = getLine(line, ";");

			first = find(line.begin(), line.end(), *(lineMarker.begin()));
			end = find(line.begin(), line.end(), *(--lineMarker.end()));

			line.erase(first, ++end);

			Assign(lineMarker);
		}
		else if (*first == "}")
		{
			line.pop_back();
		}

	}

}

void AsmTable::While(list<string> line)
{
	queue<string> postfix;

	//this assumes that first word is while
	list<string>::iterator first = line.begin();
	list<string>::iterator last = --line.end();
	list<string>::iterator end;
	
	//initializing the table that corresponds to while statement
	asmTableInput tmp;

	tmp.line = ++lineNum;
	tmp.action = "LABEL";
	tmp.address = 0; // or -1

	jmpstack.push(tmp.line);

	this->asmTable.push_back(tmp);
	line.pop_front(); //removing while

	//getting the values in paranthesis
	int count = 0;
	//first = line.begin();
	


	list<string> section = this->getLastDelimiter(line, ")", &count);

	first = find(line.begin(), line.end(), *(section.begin()));
	end = find(line.begin(), line.end(), *(--section.end()));

	line.erase(first, ++end);
	postfix = RelopExpression(section);

	//filling the table with queue from relop expression
	fillTable(postfix);

	if (line.front() == "{")
	{
		tmp.line = ++lineNum;
		tmp.action = "JMPZ";
		tmp.address = 0; // or -1

		this->jmpstack.push(tmp.line);

		this->asmTable.push_back(tmp);
		line.pop_front(); //removing while

		while (line.size() > 0)
		{
			
			list<string>::iterator first = line.begin();

			//lineMarker = getLine(";");

			//first = lineMarker.begin();

			if (*first == "while")
			{
				//While();
				int delimNum;
				list<string>::iterator end;
				int count = 0;
				list<string> lineMarker = getLastDelimiter(line, "}", &count);

				first = find(line.begin(), line.end(), *(lineMarker.begin()));
				end = find(line.begin(), line.end(), *(--lineMarker.end()));

				line.erase(first, ++end);
				While(lineMarker);

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
				list<string> lineMarker;
				lineMarker = getLine(line,";");

				first = find(line.begin(), line.end(), *(lineMarker.begin()));
				end = find(line.begin(), line.end(), *(--lineMarker.end()));

				line.erase(first, ++end);

				Assign(lineMarker);
			}
			else if (*first == "}")
			{
				line.pop_back();
			}

		}


	}

}

#pragma endregion

void AsmTable::printTable()
{
	asmTableInput input;
	for (int i = 0; i < asmTable.size(); i++)
	{
		input = asmTable[i];
		cout << input.line << "\t" << input.action << "\t" << input.address << endl;

	}


}

void AsmTable::fillTable(queue<string> postfix)
{

	while (postfix.size() > 0)
	{
		asmTableInput input;

		int address = 0;
		string tmp = postfix.front();

		address = this->sm.getAddress(tmp);

		if (address != -1)
		{
			input.line = ++lineNum;
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
				input.line = ++lineNum;
				input.action = "PUSHI";
				input.address = stoi(tmp);

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "*")
			{
				input.line = ++lineNum;
				input.action = "MUL";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();


			}
			else if (tmp == "/")
			{
				input.line = ++lineNum;
				input.action = "DIV";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();


			}
			else if (tmp == "+")
			{

				input.line = ++lineNum;
				input.action = "ADD";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "-")
			{

				input.line = ++lineNum;
				input.action = "SUB";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "==")
			{

				input.line = ++lineNum;
				input.action = "EQU";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "^=")
			{

				input.line = ++lineNum;
				input.action = "NEQ";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "=<")
			{

				input.line = ++lineNum;
				input.action = "LEQ";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "=>")
			{

				input.line = ++lineNum;
				input.action = "GEQ";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == "<")
			{

				input.line = ++lineNum;
				input.action = "LES";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}
			else if (tmp == ">")
			{

				input.line = ++lineNum;
				input.action = "GRT";
				input.address = 0;

				asmTable.push_back(input);
				postfix.pop();

			}




		}



	}



}


void AsmTable::makeAsmTable()
{
	
	while (this->lexemes.size() > 0)
	{
		
		list<string>::iterator first = this->lexemes.begin();

		//lineMarker = getLine(";");

		//first = lineMarker.begin();

		if (*first == "while")
		{
			list<string>::iterator end;
			int count = 0;
			list<string> lineMarker = getLastDelimiter(lexemes, "}", &count);

			first = find(lexemes.begin(), lexemes.end(), *(lineMarker.begin()) );
			end = find(lexemes.begin(),lexemes.end() , *(--lineMarker.end()));

			lexemes.erase(first, ++end);
			While(lineMarker);

		}
		else if (*first == "if")
		{
			list<string>::iterator end;
			list<string> lineMarker = getLastDelimiter(lexemes, "endif", 0);

			first = find(lexemes.begin(), lexemes.end(), *(lineMarker.begin()));
			end = std::next(first, lineMarker.size());

			lexemes.erase(first, end);
			If(lineMarker);
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
			list<string> lineMarker;
			lineMarker = getLine(";");

			first = lineMarker.begin();

			Assign(lineMarker);
		}

	}
	
}


#pragma endregion

AsmTable::~AsmTable()
{
}
