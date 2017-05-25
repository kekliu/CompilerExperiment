#ifndef COMPLILER_TYPES_HPP
#define COMPLILER_TYPES_HPP

#include <ostream>
#include <string>
#include <exception>
#include <list>
#include <functional>
#include "tokentype.h"


// a calculator demo with compiler technique
namespace cmplr
{

	struct ErrorMsg
	{
		int rows;
		int cols;
		char message[30];
	};

	struct TokenStruct
	{
		TokenType type;

		int row;
		int col;

		union
		{
			char   c;
			short  s;
			int    i;
			long   l;
			float  f;
			double d;
			void   *p;
			char   id[10];
		}value;

		TokenStruct() {}
		TokenStruct(TokenType t) { type = t; }
		TokenStruct(TokenType t, double value)
		{
			type = t;
			if (t == TK_INTCONST)
				this->value.i = int(value);
			else if (t == TK_DOUBLECONST)
				this->value.d = value;
			else
				throw std::exception("not implement");
		}
	};

	std::ostream &operator<<(std::ostream &out, TokenStruct &A)
	{
		switch (A.type)
		{
		case TK_ASSIGN:case TK_ADD:case TK_SUB:case TK_MUL:
		case TK_DIV:case TK_LPAREN:case TK_RPAREN:case TK_SEMICOLON:
			out << std::string(TokenNames[A.type]);
			break;
		case TK_INTCONST:
			out << "<" << TokenNames[A.type] << ", " << A.value.i << ">";
			break;
		case TK_DOUBLECONST:
			out << "<" << TokenNames[A.type] << ", " << A.value.d << ">";
			break;
		case TK_ID:case TK_FUNC:
			out << "<" << TokenNames[A.type] << ", " << std::string(A.value.id) << ">";
			break;
		default:
			out << "unrecognized token";
			break;
		}
		return out;
	}


	std::ostream &operator<<(std::ostream &out, std::list<TokenStruct> &l)
	{
		for (std::list<TokenStruct>::iterator it = l.begin(); it != l.end(); ++it)
			out << *it << std::endl;
		return out;
	}

}

#endif
