#ifndef AST_HPP
#define AST_HPP

#include "types.hpp"

namespace cmplr {

	typedef struct AstNode
	{
		TokenStruct tok;
		struct AstNode *left, *right;
	} AstNode;

}

#endif
