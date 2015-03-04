#ifndef ast_TOKENS_H
#define ast_TOKENS_H
#define YY_NULL 0
extern int ast_numtokens;
extern int * ast_tokennumbers;
extern char ** ast_tokennames;
enum ast_tokennames {
     ast_INT = 258,
     ast_FLOAT = 259,
     ast_VAR = 260,
     ast_LOGAND = 261,
     ast_LOGOR = 262,
     ast_SHL = 263,
     ast_SHR = 264,
};
#define ast_MINTOKEN      258
#define ast_MAXTOKEN      264
#endif
