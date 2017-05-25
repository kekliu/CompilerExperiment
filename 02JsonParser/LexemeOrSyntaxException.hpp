#pragma once

#include <exception>

using namespace std;

class LexemeOrSyntaxException :public exception
{
	string msg;
public:
	LexemeOrSyntaxException(short row, short column, const char *_Format, char *_Para = NULL)
	{
		char buffer[128];
		sprintf_s(buffer, "line %d, position %d: ", row, column);
		msg += buffer;
		sprintf_s(buffer, _Format, _Para);
		msg += buffer;
	}

	const char *what()
	{
		return msg.c_str();
	}

};
