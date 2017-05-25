#ifndef _CMPLR_SYMBOL_HPP
#define _CMPLR_SYMBOL_HPP

#include <cmath>
#include <iostream>
#include <cstring>
#include "types.hpp"

// macro for register a mathematical function
#define REGERSTER_FUNCTION(INPUT) if(!strcmp(funcName,#INPUT)) return INPUT;


namespace cmplr
{

	double print(double d)
	{
		std::cout << d << std::endl;
		return 0;
	}

	double(*getFunction(const char *funcName))(double)
	{
		if (!strcmp(funcName, "print"))	return print;

		REGERSTER_FUNCTION(sin);
		REGERSTER_FUNCTION(tan);
		REGERSTER_FUNCTION(cos);
		REGERSTER_FUNCTION(abs);
		REGERSTER_FUNCTION(asin);
		REGERSTER_FUNCTION(acos);
		REGERSTER_FUNCTION(atan);
		REGERSTER_FUNCTION(exp);
		REGERSTER_FUNCTION(log);
		REGERSTER_FUNCTION(log2);

		return nullptr;
	}

	struct Symbol
	{
		std::string name;
		TokenStruct tokenValue;
	};

	struct SymbolTable
	{
		std::list<Symbol> SymbolList;

		TokenStruct *getLToken(const char *id)
		{
			for (auto _it = SymbolList.begin(); _it != SymbolList.end(); ++_it)
				if (!strcmp(_it->name.c_str(), id))
					return &(_it->tokenValue);
			return nullptr;
		}

		void addOrUpdate(const char *id, TokenStruct token)
		{
			TokenStruct *ptok;
			ptok = getLToken(id);
			if (ptok)
				*ptok = token;
			else
				SymbolList.push_back({ id,token });
		}

		double getTokenValue(TokenStruct &token)
		{
			if (token.type == TK_INTCONST) return token.value.i;
			if (token.type == TK_DOUBLECONST) return token.value.d;
			if (token.type == TK_ID)
			{
				TokenStruct *t = getLToken(token.value.id);
				if (t) return getTokenValue(*t);
				char errorBuffer[256];
				sprintf_s(errorBuffer, "undefined identifier '%s'", token.value.id);
				throw std::exception(errorBuffer);
			}
			return 0;
		}


	};

}

#endif
