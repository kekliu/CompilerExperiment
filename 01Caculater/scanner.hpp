#ifndef DEMO_COMPLILER_SCANNER_HPP
#define DEMO_COMPLILER_SCANNER_HPP

#include <string.h>
#include "types.hpp"
#include "symbol.hpp"
#include <vector>


using namespace std;


namespace cmplr
{

	class scanner
	{

	private:
		// 必须使用引用类型
		vector<TokenStruct> &m_tokenList;
		vector<ErrorMsg>    &m_errorList;
		SymbolTable         &m_symbolTable;
		int                 &m_row;
		int m_column;
		char *p;


	private:
		void count() {
			switch (*p)
			{
			case '\t':
				m_column += 8 - (m_column % 8);
				break;
			case '\n':
				m_column = 0;
				break;
			case '\b':
				break;
			default:
				m_column++;
				break;
			}
		}

		 inline char input() {
			count();
			return *p++;
		}

		 inline void unput(char c) {
			count();
			--p;
		}


		// constructor
	public:
		scanner(vector<TokenStruct> &tokenList,
			vector<ErrorMsg> &errorList,
			SymbolTable &symTable,
			int &rowCount) :
			m_tokenList(tokenList),
			m_errorList(errorList),
			m_symbolTable(symTable),
			m_row(rowCount),
			m_column(0) {
		}

		

		TokenType get_one_token(char *&p, char *&buffer_start)		{
			while (*p) { if (*p != ' ' && *p != '\t') break; input(); }	
			buffer_start = p;

			int status = 0;
			char c;
			while (true) {
				switch (status) {
				case 0:
					c = input();
					if (isdigit(c)) 
						status = 1;
					else if (isalpha(c) || '_' == c) status = 4;
					else					{
						switch (c)						{
						case '+': return TK_ADD;
						case '-': return TK_SUB;
						case '*': return TK_MUL;
						case '/': return TK_DIV;
						case '(': return TK_LPAREN;
						case ')': return TK_RPAREN;
						case ';': return TK_SEMICOLON;
						case '=': return TK_ASSIGN;
						default:  return TK_ERROR;
						}
					}
					break;
				case 1:
					c = input();
					if ('.' == c) status = 2;
					else if (isdigit(c)) status = 1;
					else { unput(c); return TK_INTCONST; }
					break;
				case 2:
					c = input();
					if (isdigit(c)) status = 3;
					else return TK_ERROR;
					break;
				case 3:
					c = input();
					if (isdigit(c)) status = 3;
					else { unput(c); return TK_DOUBLECONST; }
					break;
				case 4:
					c = input();
					if (isalpha(c) || '_' == c || isdigit(c)) status = 4;
					else { unput(c); return TK_ID; }
					break;
				default:
					throw "impossible state in transit diagram";
				}
			}
		}


		int exec(const char *psrc)		{
			p = const_cast<char*>(psrc);
			char buffer[30];
			char *buffer_start;
			TokenType tok_type;
			TokenStruct token;

			while (*p)			{
				tok_type = get_one_token(p, buffer_start);

				int idx = 0;
				while (buffer_start != p) buffer[idx++] = *buffer_start++;
				buffer[idx] = '\0';

				token.type = tok_type;
				token.col = int((p - psrc) / sizeof(char)) + 1;
				token.row = m_row;

				switch (tok_type)
				{
				case TK_ASSIGN:
				case TK_ADD:
				case TK_SUB:
				case TK_MUL:
				case TK_DIV:
				case TK_LPAREN:
				case TK_RPAREN:
				case TK_SEMICOLON:
					m_tokenList.push_back(token);
					break;
				case TK_INTCONST:
					token.value.i = atoi(buffer);
					m_tokenList.push_back(token);
					break;
				case TK_DOUBLECONST:
					token.value.d = atof(buffer);
					m_tokenList.push_back(token);
					break;
				case TK_ID:
					strcpy_s(token.value.id, buffer);
					m_tokenList.push_back(token);
					break;
				case TK_ERROR:
					m_errorList.push_back({ m_row,token.col,"lexcical error" });
					break;
				default:
					break;
				}
			}
			return 0;
		}

	};

}

#endif // DEMO_COMPLILER_SCANNER_HPP
