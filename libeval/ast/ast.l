/* Copyright (c) 2012-2015 Todd Freed <todd.freed@gmail.com>

   This file is part of fab.
   
   fab is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   fab is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with fab.  If not, see <http://www.gnu.org/licenses/>. */

%{
	#include <inttypes.h>
	#include <stdio.h>

	#include "internal.h"

	#include "ast.parse.h"
	#include "ast/ast.tab.h"
	#include "ast/ast.tokens.h"

	#include "macros.h"

	// contains much of the implementation of the scanner
	#include "yyutil.h"

	/// LEX
	//
	// SUMMARY
	//  parse a whitespace or character token with no semantic value possibly at an offset from yytext
	// 
	// PARAMETERS
	//  token - token to parse
	//  ldel  - offset at which the location for this token begins 
	//
	#define LEX(token)						\
		lexify(											\
			  token										\
			, yylval									\
			, yylloc									\
			, yyextra									\
			, yytext									\
			, yyleng									\
			, 0												\
			, 0												\
			, __FUNCTION__						\
			, __FILE__								\
			, __LINE__								\
			, 0												\
		)

	int lexify(const int token, YYSTYPE * const lval, yyu_location * const lloc, parse_param * const xtra, char * const text, const int leng, const int ldel, const int vdel, const char * func, const char * file, int line, void * const v)
	{
		// populate semantic value if any on a per token basis
		if(token == INT)
			sscanf(text + vdel, "%"SCNdMAX, &lval->integer);
		else if(token == FLOAT)
			sscanf(text + vdel, "%f", &lval->fp);

		return yyu_lexify(token, lval, sizeof(*lval), lloc, &xtra->yyu_extra, text, leng, ldel, 0, func, file, line);
	}
%}

%option noinput nounput noyywrap noyy_top_state
%option reentrant
%option bison-bridge bison-locations
%option prefix="ast_yy"
%option extra-type="parse_param*"

%%

 /* ignored */
[\t ]																										{ LOCWRITE; }

 /* operators */
[~)(%/*+<>=!&^|-]																				{ return LEX(yytext[0]); }

 /* operators */
"&&"																										{ return LEX(LOGAND); }
"||"																										{ return LEX(LOGOR); }
"<<"																										{ return LEX(SHL); }
">>"																										{ return LEX(SHR); }

 /* values */
-?[0-9]+																								{ return LEX(INT); }
[a-zA-Z_]+																							{ return LEX(VAR); }
[+|-]?[0-9]+\.[0-9]*																		{ return LEX(FLOAT); }


<*>[\x00-\x1F\x7F-\xFF]                                 {
                                                          LOCWRITE;
                                                          yyu_scanner_error(yylloc, yyextra, EVAL_ILLBYTE, "unexpected byte 0x%02hhx", yytext[0]);
                                                          return 0; /* return end-of-input */
                                                        }
<*>[\x20-\x7E]                                          {
                                                          LOCWRITE;
                                                          yyu_scanner_error(yylloc, yyextra, EVAL_ILLBYTE, "unexpected character '%1$c'(0x%1$02hhx)", yytext[0]);
                                                          return 0; /* return end-of-input */
                                                        }
