#include "AsmTable.h"
#include <algorithm>
//#include "SymbolTable.h"


AsmTable::AsmTable(SymbolTable sm)
{
	this->sm = sm;
	this->lexemes = sm.lexemes;

}

#pragma region ASM TABLE generation

list<string> AsmTable::getLine(string delimiter)
{
	list<string>::iterator it = lexemes.begin();

	list<string>::iterator end;

	end = find(this->lexemes.begin(), this->lexemes.end(), delimiter);

	if (end != this->lexemes.end())
	{
		list<string> tmp(it, ++end);

		lexemes.erase(it, ++end);

		return tmp;
	}
	else
	{
		list<string> tmp;
		return tmp;
	}
}

queue<string> AsmTable::expression(list<string> ex) 
{
	queue<string> result;
	list<string>::iterator beg = ex.begin();
	list<string>::iterator end = ex.end();
	list<string>::iterator beg_paren_scan = ex.begin();
	
	while (beg_paren_scan != end)
	{
		list<string>::iterator start_of_paren = find(beg_paren_scan, end, '(')++;
		list<string>::iterator end_of_paren = find(beg_paren_scan, end, ')')--;

		list<string> temp;
		temp.splice(temp.begin(), ex, start_of_paren, end_of_paren);
	}
}

void AsmTable::makeAsmTable()
{
	list<string> lineMarker;
	





}


#pragma endregion

AsmTable::~AsmTable()
{
}
