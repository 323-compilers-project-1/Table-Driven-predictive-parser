#pragma once

#include <stack>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <list>
#include "SymbolTable.h"

using std::stack;
using std::queue;
using std::string;
using std::vector;
using std::unordered_map;
using std::ofstream;
using std::list;

struct asmTableInput
{
	int line;
	string action;
	int address;

};

class AsmTable
{
public:


	//vector<string> semantics = { "TYPE", "PUSHI", "PUSHM", "POPM", "STDOUT", "STDIN", "ADD", "SUB", "MUL", "DIV", "GRT", "LES", "EQU", "NEQ", "GEQ", "LEQ", "JUMPZ", "JUMP", "LABEL" };
	//list<string>::iterator line;
	SymbolTable sm;
	vector<asmTableInput> asmTable;
	
	queue<int> jmpstack;
	list<string> lexemes;

	//asmTable
	AsmTable(SymbolTable sm);
	void makeAsmTable();
	
	list<string> getLine(string c);
	list<string> getLine(string first, string last);

	queue<string> Assign(list<string> line);
	queue<string> Expression(list<string> line);






	
	~AsmTable();
};
