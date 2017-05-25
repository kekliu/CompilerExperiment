#ifndef DEMO_COMPLILER_PARSER_HPP
#define DEMO_COMPLILER_PARSER_HPP

// ReSharper disable CppUseAuto
#include <iostream>
#include <stack>
#include "symbol.hpp"
#include "ast.hpp"
#include <vector>

using namespace std;


namespace cmplr
{

	class parser
	{

		// public variables
		vector<ErrorMsg> &m_errorList;
		SymbolTable &m_symbolTable;

		// assist variables
		vector<TokenStruct> m_tokenList;
		vector<TokenStruct> PostfixTokenlist;
		vector<TokenStruct>::iterator it;
		char errorBuffer[256];
		vector<void*> p_nodes;

		TokenStruct BasicComputeBetweenTokens(TokenType tokentype, TokenStruct &l, TokenStruct &r) const
		{
			double lv = m_symbolTable.getTokenValue(l), rv = m_symbolTable.getTokenValue(r), re;
			switch (tokentype)
			{
			case TK_ADD: re = lv + rv; break;
			case TK_SUB: re = lv - rv; break;
			case TK_MUL: re = lv * rv; break;
			case TK_DIV: re = lv / rv; break;
			default:
				throw "invalid operator between two tokens";
			}
			if (l.type == TK_INTCONST && r.type == TK_INTCONST)
				return TokenStruct(TK_INTCONST, int(re));
			return TokenStruct(TK_DOUBLECONST, re);
		}

		AstNode *GetNewAstNode(const AstNode &ast_node)
		{
			AstNode *node = new AstNode(ast_node);
			p_nodes.push_back(node);
			return node;
		}

		void PostTraverse(AstNode *tree)
		{
			if (tree)
			{
				PostTraverse(tree->left);
				PostTraverse(tree->right);
				//cout << "post-traverse: " << tree->tok << endl;
				PostfixTokenlist.push_back(tree->tok);
			}
		}


	public:
		parser(vector<ErrorMsg> &errorList, SymbolTable &symbolList, int r) :
			m_errorList(errorList), m_symbolTable(symbolList) { }

		~parser() { for (auto astit = p_nodes.begin(); astit != p_nodes.end(); ++astit)	free(*astit); }


		AstNode *E()
		{
			AstNode *l = T();
			while (it->type == TK_ADD || it->type == TK_SUB)
			{
				TokenStruct token = *it;
				++it;
				AstNode *r_ast_node = T();
				l = GetNewAstNode({ token,l,r_ast_node });
			}
			return l;
		}

		AstNode *T()
		{
			AstNode *l = F();
			while (it->type == TK_DIV || it->type == TK_MUL)
			{
				TokenStruct token = *it;
				++it;
				AstNode *r_ast_node = F();
				l = GetNewAstNode({ token,l,r_ast_node });
			}
			return l;
		}

		AstNode *F()
		{
			AstNode *node = GetNewAstNode({ *it,nullptr,nullptr });
			switch (it->type)
			{
			case TK_INTCONST:
			case TK_DOUBLECONST:
			case TK_ID:
				++it;
				if (it->type == TK_LPAREN)
				{
					node = GetNewAstNode({ node->tok,nullptr,nullptr });
					++it;
					node->tok.type = TK_FUNC;
					node->left = E();
					if (it->type == TK_RPAREN)
						++it;
					else
						throw exception("expected ')'");
				}
				return node;
			case TK_LPAREN:
				++it;
				node = E();
				if (it->type == TK_RPAREN)
					++it;
				else
					throw exception("expected ')'");
				return node;
			default:
				//delete node;
				sprintf_s(errorBuffer, "unexpected '%s'\n", TokenNames[it->type]);
				throw exception(errorBuffer);
			}
		}

		AstNode *S()
		{
			if (it->type == TK_ID)
			{
				AstNode *node = nullptr;
				TokenStruct token = *it;
				++it;
				if (it->type == TK_ASSIGN)
				{
					node = GetNewAstNode({ *it,nullptr,nullptr });
					node->left = GetNewAstNode({ token,nullptr,nullptr });
					++it;
					node->right = E();
				}
				else if (it->type == TK_LPAREN)
				{
					--it;
					node = E();
				}

				if (it->type == TK_SEMICOLON && node)
					return node;
				cout << "expected ';'" << endl;
				return node;
				//throw exception("expected ';'");
			}
			throw exception("invalid expression");
		}

		void calc(AstNode *tree) {
			PostTraverse(tree);
			stack<TokenStruct> TokenStack;
			TokenStruct r, l;
			double(*func)(double);

			//cout << PostfixTokenlist;

			for (auto it = PostfixTokenlist.begin(); it != PostfixTokenlist.end(); ++it) {

				switch (it->type)
				{
				case TK_INTCONST:case TK_DOUBLECONST:case TK_ID:
					TokenStack.push(*it);
					break;
				case TK_ASSIGN:
					r = TokenStack.top();
					TokenStack.pop();
					l = TokenStack.top();
					TokenStack.pop();
					{
						TokenStruct *ptok;
						if (r.type == TK_ID)
							ptok = m_symbolTable.getLToken(r.value.id);
						else
							ptok = &r;
						if (ptok)
						{
							m_symbolTable.addOrUpdate(l.value.id, *ptok);
							TokenStack.push(*m_symbolTable.getLToken(l.value.id));
						}
						else
						{
							char errorBuffer[256];
							sprintf_s(errorBuffer, "undefined identifier '%s'", r.value.id);
							throw exception(errorBuffer);
						}
					}
					break;
				case TK_ADD:case TK_SUB:case TK_MUL:case TK_DIV:
					r = TokenStack.top();
					TokenStack.pop();
					l = TokenStack.top();
					TokenStack.pop();
					if (it->type == TK_DIV && m_symbolTable.getTokenValue(r) == 0)
						throw exception("divided by zero");
					TokenStack.push(BasicComputeBetweenTokens(it->type, l, r));
					break;
				case TK_FUNC:
					r = TokenStack.top();
					TokenStack.pop();
					func = getFunction(it->value.id);
					if (func)
						TokenStack.push({ TK_DOUBLECONST,func(m_symbolTable.getTokenValue(r)) });
					else
					{
						sprintf_s(errorBuffer, "undefined function '%s'", static_cast<char*>(it->value.id));
						throw exception("function not found");
					}
					break;
				default:
					sprintf_s(errorBuffer, "%s is not a valid ast node", TokenNames[it->type]);
					throw exception(errorBuffer);
				}

			}

			//cout << "size of stack: " << TokenStack.size() << "  result: " << TokenStack.top().d << endl;

		}

		int parse(const char *lineStr)
		{
			m_tokenList.clear();

			int linecount = -1;
			scanner scnnr = scanner(m_tokenList, m_errorList, m_symbolTable, linecount);
			scnnr.exec(lineStr);

			if (m_tokenList.empty()) return 0;

			// add an EOF token to the end of the token list to make the recursion terminated normally
			m_tokenList.push_back({ TK_END });

			it = m_tokenList.begin();

			//cout << "syntax analyzer:" << endl;

			AstNode *tree = S();

			calc(tree);

			if (it->type == TK_SEMICOLON || it->type == TK_END)
			{
				//cout << "right expr!!!" << endl;
			}
			else
			{
				cout << "unexpected '" << TokenNames[it->type] << "'" << endl;
			}

			return 0;
		}

	};

}

#endif
