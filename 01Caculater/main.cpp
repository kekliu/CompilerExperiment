#include <iostream>
#include "scanner.hpp"
#include "parser.hpp"
#include "memoryleakcheck.hpp"
#include <fstream>
#include <conio.h>

using namespace std;
using namespace cmplr;



void PrintUsage()
{
	cout << "Usage: Calculator[.exe] filename" << endl;
}

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();
	//_CrtSetBreakAlloc(172);

	vector<ErrorMsg> ErrorList;
	SymbolTable symbol_table;
	symbol_table.addOrUpdate("PI", TokenStruct(TK_DOUBLECONST, 3.14159265));
	symbol_table.addOrUpdate("E", TokenStruct(TK_DOUBLECONST, 2.718281828));

	ifstream infile;
	string lineStr;
	int line_count = 1;

	bool toBeContinue = true;
	while (toBeContinue)
	{
		switch (argc)
		{
		case 2:
			infile = ifstream(argv[1]);
			if (!infile) throw exception("open file error!!");
			else toBeContinue = false;
			break;
		default:
			PrintUsage();
			break;
		}
	}

	while (getline(infile, lineStr))
	{
		parser prsr = parser(ErrorList, symbol_table, line_count);
		try
		{
			prsr.parse(lineStr.c_str());
		}
		catch (exception e)
		{
			cout << e.what() << endl;
		}
		//prsr.~parser();
	}

	_getch();

	return 0;
}
