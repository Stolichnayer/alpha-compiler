/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INTEGER = 258,
    REAL = 259,
    STRING = 260,
    ID = 261,
    BREAK = 262,
    CONTINUE = 263,
    FUNCTION = 264,
    WHILE = 265,
    IF = 266,
    FOR = 267,
    RETURN = 268,
    ELSE = 269,
    AND = 270,
    OR = 271,
    NOT = 272,
    TRUE = 273,
    FALSE = 274,
    NIL = 275,
    ADD = 276,
    SUB = 277,
    MUL = 278,
    MOD = 279,
    DIV = 280,
    EQ = 281,
    NEQ = 282,
    GR = 283,
    GREQ = 284,
    LESS = 285,
    LEQ = 286,
    INC = 287,
    DEC = 288,
    LBR = 289,
    RBR = 290,
    LCURBR = 291,
    RCURBR = 292,
    LPAR = 293,
    RPAR = 294,
    COMMA = 295,
    SEMIC = 296,
    COLON = 297,
    DCOLON = 298,
    DOT = 299,
    DDOT = 300,
    LOCALL = 301,
    ASS = 302,
    UMINUS = 303
  };
#endif
/* Tokens.  */
#define INTEGER 258
#define REAL 259
#define STRING 260
#define ID 261
#define BREAK 262
#define CONTINUE 263
#define FUNCTION 264
#define WHILE 265
#define IF 266
#define FOR 267
#define RETURN 268
#define ELSE 269
#define AND 270
#define OR 271
#define NOT 272
#define TRUE 273
#define FALSE 274
#define NIL 275
#define ADD 276
#define SUB 277
#define MUL 278
#define MOD 279
#define DIV 280
#define EQ 281
#define NEQ 282
#define GR 283
#define GREQ 284
#define LESS 285
#define LEQ 286
#define INC 287
#define DEC 288
#define LBR 289
#define RBR 290
#define LCURBR 291
#define RCURBR 292
#define LPAR 293
#define RPAR 294
#define COMMA 295
#define SEMIC 296
#define COLON 297
#define DCOLON 298
#define DOT 299
#define DDOT 300
#define LOCALL 301
#define ASS 302
#define UMINUS 303

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 471 "parser.y" /* yacc.c:1909  */

    struct expr *  expression;
	  char *  stringValue;
    int     intValue;
    double  realValue;
    struct SymbolTableEntry * symbol;
    struct callstr * calls;
    unsigned unsignedvalue;
    struct forstruct * forstct;
    struct stmt * forstmt_t;
    struct forindexedstruct * forindexed;

#line 163 "parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
