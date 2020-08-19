/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parser.y" /* yacc.c:339  */

    #include <stdio.h>
    #include <stdlib.h>
    #include "avm.h"


    int yylex(void);
    extern FILE* yyin;
    int yydebug = 1;
    extern int lineno;

    int currentScope = 0;
    int anonFuncCounter = 0;
    int maxScope = 0;

    int insideFunction = 0;
    int breakflag = 0;
    int continueflag = 0;

    int isMember = 0;

    int * hiddenVarPositions;
    int hiddenVarCounter = 0;

    char * idlistarray = NULL;

    SymbolTableEntry * listhead = NULL;
    SymbolTableEntry * listtail = NULL;

    char * tmptok = NULL;


    unsigned programVarOffset = 0;
    unsigned functionLocalOffset = 0;
    unsigned formalArgOffset = 0;
    unsigned scopeSpaceCounter = 1;

    int globalsSum = 0;

    scopeoffsetstack_t * scopeoffsetStack;
    loopcounterstack_t * loopcounterstack;

    stmt s;

    int firstFunction = 0;
    int secondFunction = 0;

    int loopcounter = 0;

    int yyerror(const char * yaccProvidedMessage);

    SymbolTableEntry * insert(const char * name, unsigned int scope, unsigned int line, const char * arguments, enum symbol_t type){

        SymbolTableEntry * tmp = malloc(sizeof(SymbolTableEntry));

        tmp->next = listhead;
        tmp->prev = NULL;

        if(listhead != NULL){
            listhead->prev = tmp;
        }

        if(listhead == NULL){
            listtail = tmp;
        }

        listhead = tmp;

        if(type == 1 || type == 2){
            //einai sinartisi
            tmp->value.funcVal = malloc(sizeof(Function));
            tmp->value.funcVal->name = strdup(name);
            tmp->value.funcVal->scope = scope;
            tmp->value.funcVal->line = line;
            if(arguments != NULL)
                tmp->value.funcVal->arguments = strdup(arguments);
            else
                tmp->value.funcVal->arguments = NULL;
            tmp->isActive = 1;
            tmp->type = type;
            tmp->functionIntervene = 0;
        }
        else{
            //einai metavliti

            tmp->value.varVal = malloc(sizeof(Variable));

            tmp->value.varVal->name = strdup(name);
            tmp->value.varVal->scope = scope;
            tmp->value.varVal->line = line;

            tmp->isActive = 1;
            tmp->type = type;
            tmp->functionIntervene = 0;
        }

        return tmp;

    }

    int lookUp(int scope, char * name, int currentScope){
        SymbolTableEntry * tmp;
        tmp = listhead;
        /*
        an perasoume scope == -1 caxnoume sto scope pou eimaste eite se mikrotera.
        */

        while(tmp){
            if(tmp->type == 3 || tmp->type == 4){
                if(strcmp(tmp->value.funcVal->name, name) == 0){

                    /*vrikes to simvolo*/
                    if((tmp->value.funcVal->scope == scope || (scope == -1 && currentScope >= tmp->value.funcVal->scope)) && tmp->isActive == 1){

                        return 1;
                    }

                    /*vrike lib func*/
                    if(tmp->value.funcVal->line == 0)
                        return -1;

                }
            }
            else{
                if(strcmp(tmp->value.varVal->name, name) == 0){
                    /*vrikes to simvolo*/
                    if((tmp->value.varVal->scope == scope || (scope == -1 && currentScope >= tmp->value.varVal->scope)) && tmp->isActive == 1){

                        return 1;
                    }
                }
            }

            tmp = tmp->next;
        }

        return 0;

    }

    /* gia na kaneis lookup kai na sou epistrecei ena simbolo - node tou struct. */
    SymbolTableEntry * lookUpSymbol(int scope, char * name, int currentScope){
        SymbolTableEntry * tmp;
        tmp = listhead;
        /*
        an perasoume scope == -1 caxnoume sto scope pou eimaste eite se mikrotera.
        */
        while(tmp){
            if(tmp->type == 3 || tmp->type == 4){
                if(strcmp(tmp->value.funcVal->name, name) == 0){

                    /*vrike lib func*/
                    if(tmp->value.funcVal->line == 0)
                        return tmp;

                    /*vrikes to simvolo*/
                    if((tmp->value.funcVal->scope == scope || (scope == -1 && currentScope >= tmp->value.funcVal->scope))){

                        return tmp;
                    }
                }
            }
            else{
                if(strcmp(tmp->value.varVal->name, name) == 0){
                    /*vrikes to simvolo*/
                    if((tmp->value.varVal->scope == scope || (scope == -1 && currentScope >= tmp->value.varVal->scope))){

                        return tmp;
                    }
                }
            }

            tmp = tmp->next;
        }

        return NULL;

    }


    void hide(int scope){
        SymbolTableEntry * tmp;
        tmp = listtail;

        while(tmp){

            /*userfunc*/
            if(tmp->type == 3){

                if(tmp->value.funcVal->scope == scope){
                    if(tmp->isActive){
                        tmp->isActive = 0;;
                    }
                }
            }
            else{
                if(tmp->value.varVal->scope == scope){
                    if(tmp->isActive){
                        tmp->isActive = 0;
                    }
                }
            }
            tmp = tmp->prev;
        }
    }

    void hideForFunctions(int scope){
        SymbolTableEntry * tmp;
        tmp = listtail;

        int counter = 0;

        while(tmp){

            /*userfunc*/
            if(tmp->type == 3){

                if(tmp->value.funcVal->scope == scope){
                    if(!tmp->functionIntervene){
                    }
                }
            }
            else{
                if(tmp->value.varVal->scope == scope){
                    if(!tmp->functionIntervene){
                        tmp->functionIntervene = 1;

                        hiddenVarCounter++;
                        hiddenVarPositions = realloc(hiddenVarPositions ,hiddenVarCounter  * sizeof(int));

                        hiddenVarPositions[hiddenVarCounter - 1] = counter;
                    }
                }
            }

            counter++;

            tmp = tmp->prev;
        }
    }

    void unhide(){

        int counter = 0;
        int arrayCounter = 0;

        SymbolTableEntry * tmp;
        tmp = listtail;

        if(hiddenVarPositions == NULL){
            return;
        }

        while(tmp){

            if(hiddenVarPositions[arrayCounter] == counter){

                tmp->functionIntervene = 0;
                arrayCounter++;
            }

            counter++;
            tmp = tmp->prev;
        }

        hiddenVarPositions = NULL;
        hiddenVarCounter = 0;
    }

    void insertLibFunctions(){
        insert("print", 0, 0, NULL, libraryfunc_s);
        insert("input", 0, 0, NULL, libraryfunc_s);
        insert("objectmemberkeys", 0, 0, NULL, libraryfunc_s);
        insert("objecttotalmembers", 0, 0, NULL, libraryfunc_s);
        insert("objectcopy", 0, 0, NULL, libraryfunc_s);
        insert("totalarguments", 0, 0, NULL, libraryfunc_s);
        insert("argument", 0, 0, NULL, libraryfunc_s);
        insert("typeof", 0, 0, NULL, libraryfunc_s);
        insert("strtonum", 0, 0, NULL, libraryfunc_s);
        insert("sqrt", 0, 0, NULL, libraryfunc_s);
        insert("cos", 0, 0, NULL, libraryfunc_s);
        insert("sin", 0, 0, NULL, libraryfunc_s);
    }

    void printList(){
        SymbolTableEntry * tmp;

        int tmpScope = 0;

        while(tmpScope <= maxScope){
            //printf("-----------     Scope %d     -----------\n", tmpScope);
            tmp = listtail;
            while(tmp != NULL){

                if(tmp->type == 3 || tmp->type == 4){
                    /* prosvasi sto value apo to funcVal. */
                    if(tmp->value.funcVal->scope == tmpScope){
                        /* pros ektiposi */
                        if(tmp->type == 3){
                            /*USERFUNC*/
                            //printf("\"%s\" [user function] (line %d) (scope %d)\n", tmp->value.funcVal->name, tmp->value.funcVal->line, tmp->value.funcVal->scope);
                        }
                        else if(tmp->type == 4){
                            /*LIBFUNC*/
                            //printf("\"%s\" [library function] (line %d) (scope %d)\n", tmp->value.funcVal->name, tmp->value.funcVal->line, tmp->value.funcVal->scope);
                        }
                    }
                }
                else if(tmp->type == 0 || tmp->type == 1 || tmp->type == 2){
                    /* prosvasi sto value apo to varVal. */
                    if(tmp->value.varVal->scope == tmpScope){
                        /* pros ektiposi */
                        if(tmp->type == 0){
                            /*GLOBAL*/
                            //printf("\"%s\" [global variable] (line %d) (scope %d)\n", tmp->value.varVal->name, tmp->value.varVal->line, tmp->value.varVal->scope);
                        }
                        else if(tmp->type == 1){
                            /*LOCAL*/
                            //printf("\"%s\" [local variable] (line %d) (scope %d)\n", tmp->value.varVal->name, tmp->value.varVal->line, tmp->value.varVal->scope);
                        }
                        else if(tmp->type == 2){
                            /*FORMAL*/
                            //printf("\"%s\" [formal argument] (line %d) (scope %d)\n", tmp->value.varVal->name, tmp->value.varVal->line, tmp->value.varVal->scope);
                        }
                    }
                }

                tmp = tmp->prev;
            }
            tmpScope++;
        }
    }

    int tempcounter = 0;
    char * newtempname(){
        char * name;
        name = malloc(10);
        sprintf(name, "_t%d", tempcounter);
        tempcounter++;
        return name;
    }
    void resettemp(){
        tempcounter = 0;
    }

    int currscope(){
        return currentScope;
    }

    SymbolTableEntry * newtemp(){
        char * name;
        name = newtempname();
        SymbolTableEntry * sym = lookUpSymbol(currscope(), name, currscope());

        if(sym == NULL){
            /* oles oi krifes metavlites einai sto line 0 kai dilonontai os local variables */
            insert(name, currscope(), 0, NULL, 0);
            sym = lookUpSymbol(currscope(), name, currscope());
        }

        return sym;
    }

    scopespace_t currscopespace(void){
        if(scopeSpaceCounter == 1){
            return programvar;
        }
        else if(scopeSpaceCounter % 2 == 0){
            return formalarg;
        }
        else{
            return functionlocal;
        }
    }

    unsigned currscopeoffset(void){
        switch(currscopespace()){
            case programvar     : return programVarOffset;
            case functionlocal  : return functionLocalOffset;
            case formalarg      : return formalArgOffset;
            default: assert(0);
        }
    }

    void incurrscopeoffset(void){
        switch(currscopespace()){
            case programvar     : ++programVarOffset; break;
            case functionlocal  : ++functionLocalOffset; break;
            case formalarg      : ++formalArgOffset; break;
            default: assert(0);
        }
    }

    void enterscopespace(void){
        ++scopeSpaceCounter;
    }

    void exitscopespace(void){
        assert(scopeSpaceCounter > 1);
        --scopeSpaceCounter;
    }

    void resetformalargsoffset(void){
        formalArgOffset = 0;
    }

    void resetfunctionlocalsoffset(void){
        functionLocalOffset = 0;
    }

    unsigned int istempname(const char * s){
        return * s == '_';
    }

    unsigned int istempexpr(expr * e){
        return e->sym && istempname(e->sym->value.varVal->name);
    }

    expr * emit_iftableitem(expr * e){
        if(e->type != tableitem_e){
            return e;
        }
        else {
            expr * result = newexpr(var_e);
            result->sym = newtemp();
            emit(tablegetelem, result, e->index, e, -1, lineno);
            return result;
        }
    }

    expr * make_call(expr * lv, expr * reversed_elist){
        expr * func = emit_iftableitem(lv);
        while(reversed_elist){
            //printf("\n!!param!!\n");
            emit(param, reversed_elist, NULL, NULL, -1, lineno);
            reversed_elist = reversed_elist->next;
        }
        emit(call, func, NULL, NULL, -1, lineno);
        expr * result = newexpr(var_e);
        result->sym = newtemp();
        emit(getretval, result, NULL, NULL, -1, lineno);
        return result;
    }

    void restorecurrscopeoffset(unsigned n){
        switch(currscopespace()){
            case programvar     : programVarOffset = n; break;
            case functionlocal  : functionLocalOffset = n; break;
            case formalarg      : formalArgOffset = n; break;
            default				: assert(0);
        }
    }

    /*
        kai stmts program
        stmt1:  stmt{
                $<forstmt_t>$ = $<forstmt_t>1;
            }
            ;

        stmts:      stmt1 stmt      {
                                        $<forstmt_t>$->breakList = mergelist($<forstmt_t>1->breakList, $<forstmt_t>2->breakList);
                                        $<forstmt_t>$->contList = mergelist($<forstmt_t>1->contList, $<forstmt_t>2->contList);
                                    }
            ;
    */

#line 534 "parser.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.h".  */
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
#line 471 "parser.y" /* yacc.c:355  */

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

#line 683 "parser.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 700 "parser.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  72
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   517

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  192

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   303

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   513,   513,   515,   521,   523,   525,   527,   529,   531,
     546,   557,   559,   561,   567,   569,   583,   597,   611,   625,
     639,   657,   675,   693,   711,   729,   747,   762,   777,   783,
     789,   805,   817,   855,   892,   931,   969,   976,  1016,  1022,
    1024,  1026,  1028,  1030,  1036,  1041,  1092,  1116,  1126,  1129,
    1130,  1137,  1139,  1143,  1147,  1156,  1164,  1166,  1170,  1179,
    1188,  1190,  1193,  1197,  1220,  1243,  1246,  1252,  1260,  1262,
    1266,  1275,  1284,  1287,  1301,  1328,  1334,  1340,  1347,  1352,
    1373,  1377,  1381,  1385,  1388,  1391,  1396,  1398,  1402,  1420,
    1424,  1432,  1438,  1441,  1447,  1454,  1461,  1464,  1467,  1475,
    1491,  1497,  1502,  1507,  1515,  1528,  1535
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTEGER", "REAL", "STRING", "ID",
  "BREAK", "CONTINUE", "FUNCTION", "WHILE", "IF", "FOR", "RETURN", "ELSE",
  "AND", "OR", "NOT", "TRUE", "FALSE", "NIL", "ADD", "SUB", "MUL", "MOD",
  "DIV", "EQ", "NEQ", "GR", "GREQ", "LESS", "LEQ", "INC", "DEC", "LBR",
  "RBR", "LCURBR", "RCURBR", "LPAR", "RPAR", "COMMA", "SEMIC", "COLON",
  "DCOLON", "DOT", "DDOT", "LOCALL", "ASS", "UMINUS", "$accept", "program",
  "stmt", "expr", "term", "assignexpr", "primary", "lvalue", "member",
  "call", "callsuffix", "normcall", "methodcall", "elist", "objectdef",
  "indexed", "indexedelem", "blocknext", "lcbrcode", "block", "funcname",
  "funcprefix", "funcargs", "funcbody", "funcblockstart", "funcblockend",
  "funcdef", "const", "idlistnext", "idlist", "ifprefix", "elseprefix",
  "ifstmt", "whilestart", "whilecond", "loopstart", "loopend", "loopstmt",
  "whilestmt", "N", "M", "forkeyword", "forprefix", "forstmt",
  "returnstmt", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303
};
# endif

#define YYPACT_NINF -175

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-175)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-1)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     156,  -175,  -175,  -175,  -175,   -28,   -19,    11,  -175,    -4,
    -175,    25,   253,  -175,  -175,  -175,   253,    18,    18,   201,
    -175,   208,  -175,    20,    42,   253,    52,   156,   282,  -175,
    -175,  -175,   100,  -175,    63,  -175,   156,  -175,    22,  -175,
    -175,   156,  -175,    27,  -175,    29,  -175,  -175,  -175,  -175,
    -175,  -175,  -175,   253,  -175,   303,  -175,  -175,    82,   250,
     -24,   250,   253,   453,    -3,    -2,  -175,   345,    65,  -175,
    -175,   453,  -175,  -175,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,  -175,  -175,  -175,
     253,   253,    96,   105,   253,  -175,  -175,  -175,   253,   208,
     106,  -175,   245,   156,    68,   107,  -175,   102,   253,  -175,
     253,   253,   364,  -175,    92,   253,    99,  -175,   253,  -175,
     101,  -175,    97,   464,   464,    26,    26,  -175,  -175,  -175,
     475,   475,   486,   486,   486,   486,   419,   -35,  -175,    98,
     453,   436,    15,  -175,  -175,  -175,   108,   103,   104,  -175,
     156,   383,   156,  -175,    -1,    49,  -175,    97,   253,   453,
    -175,   253,  -175,  -175,   253,  -175,  -175,   107,  -175,  -175,
    -175,  -175,  -175,  -175,  -175,  -175,  -175,   402,    56,    60,
    -175,  -175,  -175,   253,  -175,  -175,  -175,  -175,   324,  -175,
    -175,  -175
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,    80,    81,    82,    45,     0,     0,    73,    94,     0,
     102,     0,     0,    84,    85,    83,     0,     0,     0,    62,
      70,     0,    13,     0,     0,     0,     0,     3,     0,    28,
      14,    36,    39,    48,    40,    42,    69,    11,     0,    12,
      44,     0,     5,     0,     6,     0,   100,     7,     8,     9,
      10,    72,    74,     0,   105,     0,    31,    30,     0,    32,
       0,    34,     0,    60,     0,     0,    65,     0,     0,    47,
      46,    38,     1,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,    33,    35,
       0,    62,     0,     0,     0,    54,    56,    57,     0,    62,
       0,    41,     0,    69,     0,    89,    77,    92,     0,    96,
      62,    62,     0,   106,     0,    62,     0,    63,     0,    64,
       0,    29,    43,    26,    27,    15,    16,    17,    19,    18,
      24,    25,    20,    21,    22,    23,     0,     0,    49,     0,
      37,     0,     0,    51,    68,    71,    87,     0,     0,    91,
       0,     0,     0,    99,     0,     0,    90,     0,     0,    61,
      66,    62,    50,    58,    62,    52,    53,    89,    88,    75,
      76,    78,    93,    95,    97,   101,   100,     0,     0,     0,
      86,    79,    98,     0,    96,    67,    55,    59,     0,   100,
     103,   104
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -175,   112,   -33,     0,  -175,   115,  -175,   -11,  -175,     1,
    -175,  -175,  -175,   -18,  -175,  -175,    31,    50,  -175,     6,
    -175,  -175,  -175,  -175,  -175,  -175,   -12,  -175,  -175,   -10,
    -175,  -175,  -175,  -175,  -175,  -175,  -175,   -29,  -175,  -174,
    -175,  -175,  -175,  -175,  -175
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    26,    27,    63,    29,    30,    31,    32,    33,    34,
      95,    96,    97,   142,    35,    65,    66,   104,    36,    37,
      52,    38,   106,   171,   148,   181,    39,    40,   168,   147,
      41,   150,    42,    43,   109,   152,   182,   153,    44,   111,
     183,    45,    46,    47,    48
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,    64,   184,   103,   163,   118,    59,    61,   107,    68,
      98,    55,    56,    49,   115,   191,    57,    51,    60,    60,
     100,    67,    50,   102,     4,    71,    69,    28,     1,     2,
       3,     4,   117,   119,    53,    60,    28,   118,   120,   118,
     175,    28,    12,    13,    14,    15,   114,    16,    70,    78,
      79,    80,    72,   112,   166,   118,    58,    17,    18,    19,
     105,    23,   116,    21,    24,   108,    54,   110,    23,     4,
     103,    24,    25,   137,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   114,   176,   118,
     136,     7,   154,   155,   140,   186,   118,    98,   141,   187,
     118,    99,   138,    28,   122,   145,    23,   100,   151,    24,
      25,   139,   143,   146,    74,    75,   149,   172,   159,   174,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,   157,    88,    89,    90,   161,   164,    62,    91,    73,
      20,   158,   169,   178,    92,    93,   179,    94,   167,   101,
      28,   160,    28,   144,   170,   189,     0,   180,   177,     1,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,     0,    16,     0,
       0,     0,     0,   188,     0,     0,     0,     0,    17,    18,
      19,     0,    20,     0,    21,     0,     0,    22,     0,    23,
       0,     0,    24,    25,     1,     2,     3,     4,     0,     0,
       0,     1,     2,     3,     4,     0,     0,     7,    12,    13,
      14,    15,     0,    16,     0,    12,    13,    14,    15,     0,
      16,     0,     0,    17,    18,    19,     0,    62,     0,    21,
      17,    18,    19,     0,    23,     0,    21,    24,    25,     0,
       0,    23,     0,     0,    24,    25,     1,     2,     3,     4,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      12,    13,    14,    15,     0,    16,     0,     0,     0,    90,
       0,     0,     0,    91,    90,    17,    18,    19,    91,    92,
      93,    21,    94,     0,    92,    93,    23,    74,    75,    24,
      25,     0,     0,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,     0,     0,     0,     0,    74,    75,
       0,     0,     0,    87,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,     0,     0,     0,     0,    74,
      75,     0,     0,     0,   113,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,     0,     0,     0,     0,
      74,    75,     0,     0,     0,   190,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,     0,     0,    74,
      75,     0,     0,     0,   121,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,     0,     0,    74,    75,
       0,     0,     0,   156,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,     0,     0,    74,    75,     0,
       0,     0,   173,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    74,    75,     0,     0,     0,   185,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    74,    75,     0,   162,     0,     0,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    74,    75,
       0,   165,     0,     0,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    76,    77,    78,    79,
      80,    -1,    -1,    83,    84,    85,    86,    76,    77,    78,
      79,    80,     0,     0,    -1,    -1,    -1,    -1
};

static const yytype_int16 yycheck[] =
{
       0,    19,   176,    36,    39,    40,    17,    18,    41,    21,
      34,    11,    12,    41,    38,   189,    16,     6,    17,    18,
      44,    21,    41,    34,     6,    25,     6,    27,     3,     4,
       5,     6,    35,    35,    38,    34,    36,    40,    40,    40,
      41,    41,    17,    18,    19,    20,    58,    22,     6,    23,
      24,    25,     0,    53,    39,    40,    38,    32,    33,    34,
      38,    43,    62,    38,    46,    38,    41,    38,    43,     6,
     103,    46,    47,    91,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    99,    39,    40,
      90,     9,   110,   111,    94,    39,    40,    34,    98,    39,
      40,    38,     6,   103,    39,    37,    43,    44,   108,    46,
      47,     6,     6,     6,    15,    16,    14,   150,   118,   152,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    39,    32,    33,    34,    38,    38,    36,    38,    27,
      36,    42,    39,   161,    44,    45,   164,    47,    40,    34,
     150,   120,   152,   103,   148,   184,    -1,   167,   158,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      -1,    -1,    -1,    17,    18,    19,    20,    -1,    22,    -1,
      -1,    -1,    -1,   183,    -1,    -1,    -1,    -1,    32,    33,
      34,    -1,    36,    -1,    38,    -1,    -1,    41,    -1,    43,
      -1,    -1,    46,    47,     3,     4,     5,     6,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,     9,    17,    18,
      19,    20,    -1,    22,    -1,    17,    18,    19,    20,    -1,
      22,    -1,    -1,    32,    33,    34,    -1,    36,    -1,    38,
      32,    33,    34,    -1,    43,    -1,    38,    46,    47,    -1,
      -1,    43,    -1,    -1,    46,    47,     3,     4,     5,     6,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    19,    20,    -1,    22,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    38,    34,    32,    33,    34,    38,    44,
      45,    38,    47,    -1,    44,    45,    43,    15,    16,    46,
      47,    -1,    -1,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    -1,    -1,    -1,    15,    16,
      -1,    -1,    -1,    41,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,    15,
      16,    -1,    -1,    -1,    41,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
      15,    16,    -1,    -1,    -1,    41,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    -1,    15,
      16,    -1,    -1,    -1,    39,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    -1,    15,    16,
      -1,    -1,    -1,    39,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    -1,    15,    16,    -1,
      -1,    -1,    39,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    15,    16,    -1,    -1,    -1,    37,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    15,    16,    -1,    35,    -1,    -1,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    15,    16,
      -1,    35,    -1,    -1,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    21,    22,    23,
      24,    25,    -1,    -1,    28,    29,    30,    31
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    17,    18,    19,    20,    22,    32,    33,    34,
      36,    38,    41,    43,    46,    47,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    63,    67,    68,    70,    75,
      76,    79,    81,    82,    87,    90,    91,    92,    93,    41,
      41,     6,    69,    38,    41,    52,    52,    52,    38,    56,
      58,    56,    36,    52,    62,    64,    65,    52,    75,     6,
       6,    52,     0,    50,    15,    16,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    41,    32,    33,
      34,    38,    44,    45,    47,    59,    60,    61,    34,    38,
      44,    54,    56,    51,    66,    38,    71,    51,    38,    83,
      38,    88,    52,    41,    75,    38,    52,    35,    40,    35,
      40,    39,    39,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    62,     6,     6,
      52,    52,    62,     6,    66,    37,     6,    78,    73,    14,
      80,    52,    84,    86,    62,    62,    39,    39,    42,    52,
      65,    38,    35,    39,    38,    35,    39,    40,    77,    39,
      68,    72,    51,    39,    51,    41,    39,    52,    62,    62,
      78,    74,    85,    89,    88,    37,    39,    39,    52,    86,
      41,    88
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    49,    50,    50,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    53,
      53,    53,    53,    53,    53,    53,    53,    54,    54,    55,
      55,    55,    55,    55,    55,    56,    56,    56,    56,    57,
      57,    57,    57,    58,    58,    58,    59,    59,    60,    61,
      62,    62,    62,    63,    63,    64,    64,    65,    66,    66,
      67,    68,    69,    69,    70,    71,    72,    73,    74,    75,
      76,    76,    76,    76,    76,    76,    77,    77,    78,    78,
      79,    80,    81,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    93
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     2,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       2,     2,     2,     2,     2,     2,     1,     3,     2,     1,
       1,     2,     1,     3,     1,     1,     2,     2,     1,     3,
       4,     3,     4,     4,     2,     6,     1,     1,     3,     5,
       1,     3,     0,     3,     3,     1,     3,     5,     2,     0,
       1,     3,     1,     0,     2,     3,     1,     0,     0,     5,
       1,     1,     1,     1,     1,     1,     2,     0,     2,     0,
       4,     1,     2,     4,     1,     3,     0,     0,     3,     3,
       0,     0,     1,     7,     7,     2,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 513 "parser.y" /* yacc.c:1646  */
    {////printf("program -> stmt program \n");
                                }
#line 1984 "parser.c" /* yacc.c:1646  */
    break;

  case 3:
#line 515 "parser.y" /* yacc.c:1646  */
    {////printf("program -> empty \n");
                                }
#line 1991 "parser.c" /* yacc.c:1646  */
    break;

  case 4:
#line 521 "parser.y" /* yacc.c:1646  */
    {////printf("stmt -> expr SEMIC \n"); resettemp();
                                }
#line 1998 "parser.c" /* yacc.c:1646  */
    break;

  case 5:
#line 523 "parser.y" /* yacc.c:1646  */
    {////printf("stmt -> ifstmt \n"); resettemp();
                                }
#line 2005 "parser.c" /* yacc.c:1646  */
    break;

  case 6:
#line 525 "parser.y" /* yacc.c:1646  */
    {////printf("stmt -> whilestmt \n"); resettemp();
            }
#line 2012 "parser.c" /* yacc.c:1646  */
    break;

  case 7:
#line 527 "parser.y" /* yacc.c:1646  */
    {////printf("stmt -> forstmt \n"); resettemp();
            }
#line 2019 "parser.c" /* yacc.c:1646  */
    break;

  case 8:
#line 529 "parser.y" /* yacc.c:1646  */
    {////printf("stmt -> returnstmt \n"); resettemp();
            }
#line 2026 "parser.c" /* yacc.c:1646  */
    break;

  case 9:
#line 531 "parser.y" /* yacc.c:1646  */
    {
                                    if(loopcounter == 0){
                                        printf("Error at line %d: use of \"break\" while not in a loop\n", lineno);
                                        exit(1);
                                    }
                                        
                                    else {

                                        emit(jump, NULL, NULL, NULL, s.breakList, lineno);
                                        s.breakList = nextquad();

                                    }
                                    ////printf("stmt -> BREAK SEMIC \n");
                                    resettemp();
                                }
#line 2046 "parser.c" /* yacc.c:1646  */
    break;

  case 10:
#line 546 "parser.y" /* yacc.c:1646  */
    {
                                    if(loopcounter == 0)
                                        printf("Error at line %d: use of \"continue\" while not in a loop\n", lineno);
                                    else {

                                        emit(jump, NULL, NULL, NULL, s.contList, lineno);
                                        s.contList = nextquad();
                                    }
                                    //printf("stmt -> CONTINUE SEMIC \n");
                                    resettemp();
                                }
#line 2062 "parser.c" /* yacc.c:1646  */
    break;

  case 11:
#line 557 "parser.y" /* yacc.c:1646  */
    {//printf("stmt -> block \n");resettemp();
            }
#line 2069 "parser.c" /* yacc.c:1646  */
    break;

  case 12:
#line 559 "parser.y" /* yacc.c:1646  */
    {//printf("stmt -> funcdef \n");resettemp();
            }
#line 2076 "parser.c" /* yacc.c:1646  */
    break;

  case 13:
#line 561 "parser.y" /* yacc.c:1646  */
    {//printf("stmt -> SEMIC \n");resettemp();
            }
#line 2083 "parser.c" /* yacc.c:1646  */
    break;

  case 14:
#line 567 "parser.y" /* yacc.c:1646  */
    {//printf("expr -> assignexpr \n");$<expression>$ = $<expression>1;
}
#line 2090 "parser.c" /* yacc.c:1646  */
    break;

  case 15:
#line 569 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.expression) = newexpr(arithexpr_e);
                                    (yyval.expression)->sym = newtemp();

                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    emit(add, (yyval.expression), (yyvsp[-2].expression), (yyvsp[0].expression), -1, lineno);
                                    //printf("expr ADD expr\n");

                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
#line 2109 "parser.c" /* yacc.c:1646  */
    break;

  case 16:
#line 583 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.expression) = newexpr(arithexpr_e);
                                    (yyval.expression)->sym = newtemp();

                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    
                                    emit(sub, (yyval.expression), (yyvsp[-2].expression), (yyvsp[0].expression), -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
#line 2128 "parser.c" /* yacc.c:1646  */
    break;

  case 17:
#line 597 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.expression) = newexpr(arithexpr_e);
                                    (yyval.expression)->sym = newtemp();
                                                                        
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    
                                    emit(mul, (yyval.expression), (yyvsp[-2].expression), (yyvsp[0].expression), -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
#line 2147 "parser.c" /* yacc.c:1646  */
    break;

  case 18:
#line 611 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.expression) = newexpr(arithexpr_e);
                                    (yyval.expression)->sym = newtemp();
                                                                        
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    
                                    emit(divide, (yyval.expression), (yyvsp[-2].expression), (yyvsp[0].expression), -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
#line 2166 "parser.c" /* yacc.c:1646  */
    break;

  case 19:
#line 625 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.expression) = newexpr(arithexpr_e);
                                    (yyval.expression)->sym = newtemp();
                                                                        
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    
                                    emit(mod, (yyval.expression), (yyvsp[-2].expression), (yyvsp[0].expression), -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
#line 2185 "parser.c" /* yacc.c:1646  */
    break;

  case 20:
#line 639 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.expression) = newexpr(boolexpr_e);
                                    (yyval.expression)->sym = newtemp();
                                                                        
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(if_greater, NULL, (yyvsp[-2].expression), (yyvsp[0].expression), nextquad() + 3, lineno);
                                    emit(assign, (yyval.expression), newexpr_constbool(0), NULL, -1, lineno);
                                    emit(jump, NULL, NULL, NULL, nextquad() + 2, lineno);
                                    emit(assign, (yyval.expression), newexpr_constbool(1), NULL, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
#line 2208 "parser.c" /* yacc.c:1646  */
    break;

  case 21:
#line 657 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.expression) = newexpr(boolexpr_e);
                                    (yyval.expression)->sym = newtemp();
                                                                        
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(if_greatereq, NULL, (yyvsp[-2].expression), (yyvsp[0].expression), nextquad() + 3, lineno);
                                    emit(assign, (yyval.expression), newexpr_constbool(0), NULL, -1, lineno);
                                    emit(jump, NULL, NULL, NULL, nextquad() + 2, lineno);
                                    emit(assign, (yyval.expression), newexpr_constbool(1), NULL, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
#line 2231 "parser.c" /* yacc.c:1646  */
    break;

  case 22:
#line 675 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.expression) = newexpr(boolexpr_e);
                                    (yyval.expression)->sym = newtemp();
                                                                        
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(if_less, NULL, (yyvsp[-2].expression), (yyvsp[0].expression), nextquad() + 3, lineno);
                                    emit(assign, (yyval.expression), newexpr_constbool(0), NULL, -1, lineno);
                                    emit(jump, NULL, NULL, NULL, nextquad() + 2, lineno);
                                    emit(assign, (yyval.expression), newexpr_constbool(1), NULL, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                            globalsSum++;
                                }
#line 2254 "parser.c" /* yacc.c:1646  */
    break;

  case 23:
#line 693 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.expression) = newexpr(boolexpr_e);
                                    (yyval.expression)->sym = newtemp();
                                                                        
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(if_lesseq, NULL, (yyvsp[-2].expression), (yyvsp[0].expression), nextquad() + 3, lineno);
                                    emit(assign, (yyval.expression), newexpr_constbool(0), NULL, -1, lineno);
                                    emit(jump, NULL, NULL, NULL, nextquad() + 2, lineno);
                                    emit(assign, (yyval.expression), newexpr_constbool(1), NULL, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
#line 2277 "parser.c" /* yacc.c:1646  */
    break;

  case 24:
#line 711 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.expression) = newexpr(boolexpr_e);
                                    (yyval.expression)->sym = newtemp();
                                                                        
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(if_eq, NULL, (yyvsp[-2].expression), (yyvsp[0].expression), nextquad() + 3, lineno);
                                    emit(assign, (yyval.expression), newexpr_constbool(0), NULL, -1, lineno);
                                    emit(jump, NULL, NULL, NULL, nextquad() + 2, lineno);
                                    emit(assign, (yyval.expression), newexpr_constbool(1), NULL, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
#line 2300 "parser.c" /* yacc.c:1646  */
    break;

  case 25:
#line 729 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.expression) = newexpr(boolexpr_e);
                                    (yyval.expression)->sym = newtemp();
                                                                        
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(if_noteq, NULL, (yyvsp[-2].expression), (yyvsp[0].expression), nextquad() + 3, lineno);
                                    emit(assign, (yyval.expression), newexpr_constbool(0), NULL, -1, lineno);
                                    emit(jump, NULL, NULL, NULL, nextquad() + 2, lineno);
                                    emit(assign, (yyval.expression), newexpr_constbool(1), NULL, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
#line 2323 "parser.c" /* yacc.c:1646  */
    break;

  case 26:
#line 747 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.expression) = newexpr(boolexpr_e);
                                    (yyval.expression)->sym = newtemp();
                                                                        
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(and, (yyval.expression), (yyvsp[-2].expression), (yyvsp[0].expression), -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
#line 2343 "parser.c" /* yacc.c:1646  */
    break;

  case 27:
#line 762 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.expression) = newexpr(boolexpr_e);
                                    (yyval.expression)->sym = newtemp();
                                                                        
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(or, (yyval.expression), (yyvsp[-2].expression), (yyvsp[0].expression), -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
#line 2363 "parser.c" /* yacc.c:1646  */
    break;

  case 28:
#line 777 "parser.y" /* yacc.c:1646  */
    {
                                    //printf("expr -> term \n");
                                    (yyval.expression) = (yyvsp[0].expression);
                                }
#line 2372 "parser.c" /* yacc.c:1646  */
    break;

  case 29:
#line 783 "parser.y" /* yacc.c:1646  */
    {
                                    //printf("term -> LPAR expr RPAR \n");

                                    (yyval.expression) = (yyvsp[-1].expression);

                                }
#line 2383 "parser.c" /* yacc.c:1646  */
    break;

  case 30:
#line 789 "parser.y" /* yacc.c:1646  */
    {

                                        //printf("term -> UMINUS expr \n");

                                        check_arith((yyvsp[0].expression));
                                        (yyval.expression) = newexpr(arithexpr_e);
                                        (yyval.expression)->sym = istempexpr((yyvsp[0].expression)) ? (yyvsp[0].expression)->sym : newtemp();

                                                                            
                                        (yyval.expression)->sym->offset = currscopeoffset();
                                        (yyval.expression)->sym->space = currscopespace();
                                        incurrscopeoffset();

                                    
                                        emit(uminus, (yyval.expression), NULL, (yyvsp[0].expression), -1, lineno);
                                    }
#line 2404 "parser.c" /* yacc.c:1646  */
    break;

  case 31:
#line 805 "parser.y" /* yacc.c:1646  */
    {
                                    //printf("term -> NOT expr \n");
                                    (yyval.expression) = newexpr(boolexpr_e);
                                    (yyval.expression)->sym = newtemp();
                                                                        
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    
                                    emit(not, (yyval.expression), (yyvsp[0].expression), NULL, -1, lineno);
                                }
#line 2421 "parser.c" /* yacc.c:1646  */
    break;

  case 32:
#line 817 "parser.y" /* yacc.c:1646  */
    {
                                    //printf("term -> INC lvalue \n");

                                    SymbolTableEntry * tmp;
                                    tmp = lookUpSymbol(-1, (yyvsp[0].stringValue), currentScope);

                                    if(tmp != NULL){
                                        if(tmp->type == 3 || tmp->type == 4){
                                            if(tmp->isActive)
                                                printf("Error at line %d: cannot increase value of a function\n", lineno);
                                            else if(tmp->functionIntervene)
                                                printf("Error at line %d: cannot access symbol %s\n", lineno , tmp->value.funcVal->name);
                                        }
                                    }

                                    check_arith((yyvsp[0].expression));


                                    if((yyvsp[0].expression)->type == tableitem_e){
                                        (yyval.expression) = emit_iftableitem((yyvsp[0].expression));
                                        emit(add, (yyval.expression), newexpr_constnum(1), (yyval.expression), -1, lineno);
                                        emit(tablesetelem, (yyvsp[0].expression), (yyvsp[0].expression)->index, (yyval.expression), -1, lineno);
                                    }
                                    else {
                                        emit(add, (yyvsp[0].expression), newexpr_constnum(1), (yyvsp[0].expression), -1, lineno);
                                        (yyval.expression) = newexpr(arithexpr_e);
                                        (yyval.expression)->sym = newtemp();

                                                                            
                                        (yyval.expression)->sym->offset = currscopeoffset();
                                        (yyval.expression)->sym->space = currscopespace();
                                        incurrscopeoffset();

                                    
                                        emit(assign, (yyval.expression), NULL, (yyvsp[0].expression), -1, lineno);
                                    }

                                }
#line 2464 "parser.c" /* yacc.c:1646  */
    break;

  case 33:
#line 855 "parser.y" /* yacc.c:1646  */
    {

                                    //printf("term -> lvalue INC \n");

                                    SymbolTableEntry * tmp;
                                    tmp = lookUpSymbol(-1, (yyvsp[-1].stringValue), currentScope);

                                    if(tmp != NULL){
                                        if(tmp->type == 3 || tmp->type == 4){
                                            if(tmp->isActive)
                                                printf("Error at line %d: cannot increase value of a function\n", lineno);
                                            else if(tmp->functionIntervene)
                                                printf("Error at line %d: cannot access symbol %s\n", lineno, tmp->value.funcVal->name);
                                        }
                                    }

                                    check_arith((yyvsp[-1].expression));
                                    (yyval.expression) = newexpr(var_e);
                                    (yyval.expression)->sym = newtemp();

                                    
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    
                                    if((yyvsp[-1].expression)->type == tableitem_e){
                                        expr * val = emit_iftableitem((yyvsp[-1].expression));
                                        emit(assign, (yyval.expression), NULL, val, -1, lineno);
                                        emit(add, val, newexpr_constnum(1), val, -1, lineno);
                                        emit(tablesetelem, (yyvsp[-1].expression), (yyvsp[-1].expression)->index, val, -1, lineno);
                                    }
                                    else {
                                        emit(assign, (yyval.expression), NULL, (yyvsp[-1].expression), -1, lineno);
                                        emit(add, (yyvsp[-1].expression), newexpr_constnum(1), (yyvsp[-1].expression), -1, lineno);
                                    }
                                }
#line 2506 "parser.c" /* yacc.c:1646  */
    break;

  case 34:
#line 892 "parser.y" /* yacc.c:1646  */
    {

                                    //printf("term -> DEC lvalue \n");

                                    SymbolTableEntry * tmp;
                                    tmp = lookUpSymbol(-1, (yyvsp[0].stringValue), currentScope);

                                    if(tmp != NULL){
                                        if(tmp->type == 3 || tmp->type == 4){
                                            if(tmp->isActive)
                                                printf("Error at line %d: cannot decrease value of a function\n", lineno);
                                            else if(tmp->functionIntervene)
                                                printf("Error at line %d: cannot access symbol %s\n", lineno, tmp->value.funcVal->name);
                                        }
                                    }

                                    check_arith((yyvsp[0].expression));

                                    if((yyvsp[0].expression)->type == tableitem_e){
                                        (yyval.expression) = emit_iftableitem((yyvsp[0].expression));
                                        emit(sub, (yyval.expression), newexpr_constnum(1), (yyval.expression), -1, lineno);
                                        emit(tablesetelem, (yyvsp[0].expression), (yyvsp[0].expression)->index, (yyval.expression), -1, lineno);
                                    }
                                    else {

                                        emit(sub, (yyvsp[0].expression), newexpr_constnum(1), (yyvsp[0].expression), -1, lineno);
                                        (yyval.expression) = newexpr(arithexpr_e);
                                        (yyval.expression)->sym = newtemp();

                                                                            
                                        (yyval.expression)->sym->offset = currscopeoffset();
                                        (yyval.expression)->sym->space = currscopespace();
                                        incurrscopeoffset();

                                    
                                        emit(assign, (yyval.expression), NULL, (yyvsp[0].expression), -1, lineno);

                                    }
                                }
#line 2550 "parser.c" /* yacc.c:1646  */
    break;

  case 35:
#line 931 "parser.y" /* yacc.c:1646  */
    {

                                    //printf("term -> lvalue DEC \n");

                                    SymbolTableEntry * tmp;
                                    tmp = lookUpSymbol(-1, (yyvsp[-1].stringValue), currentScope);

                                    if(tmp != NULL){
                                        if(tmp->type == 3 || tmp->type == 4){
                                            if(tmp->isActive)
                                                printf("Error at line %d: cannot decrease value of a function\n", lineno);
                                            else if(tmp->functionIntervene)
                                                printf("Error at line %d: cannot access symbol %s\n", lineno, tmp->value.funcVal->name);
                                        }
                                    }

                                    check_arith((yyvsp[-1].expression));
                                    (yyval.expression) = newexpr(var_e);
                                    (yyval.expression)->sym = newtemp();

                                                                        
                                    (yyval.expression)->sym->offset = currscopeoffset();
                                    (yyval.expression)->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    if((yyvsp[-1].expression)->type == tableitem_e){
                                        expr * val = emit_iftableitem((yyvsp[-1].expression));
                                        emit(assign, (yyval.expression), NULL, val, -1, lineno);
                                        emit(sub, val, newexpr_constnum(1), val, -1, lineno);
                                        emit(tablesetelem, (yyvsp[-1].expression), (yyvsp[-1].expression)->index, val, -1, lineno);
                                    }
                                    else {
                                        emit(assign, (yyval.expression), NULL, (yyvsp[-1].expression), -1, lineno);
                                        emit(sub, (yyvsp[-1].expression), newexpr_constnum(1), (yyvsp[-1].expression), -1, lineno);
                                    }
                                }
#line 2593 "parser.c" /* yacc.c:1646  */
    break;

  case 36:
#line 969 "parser.y" /* yacc.c:1646  */
    {//printf("term -> primary \n");

                                    (yyval.expression) = (yyvsp[0].expression);

                                }
#line 2603 "parser.c" /* yacc.c:1646  */
    break;

  case 37:
#line 976 "parser.y" /* yacc.c:1646  */
    {
                                    //printf("assignexpr -> lvalue ASS expr \n");

                                    SymbolTableEntry * tmp = NULL;
                                    tmp = lookUpSymbol(-1, (yyvsp[-2].stringValue), currentScope);

                                    if(tmp != NULL){
                                        if((tmp->type == 3 || tmp->type == 4) && isMember == 0){
                                            printf("Error at line %d: cannot assign value to a function\n", lineno);

                                        }
                                        else if((tmp->type == 0 || tmp->type == 1 || tmp->type == 2) && isMember == 0){
                                            if(tmp->functionIntervene)
                                                printf("Error at line %d: cannot access symbol %s\n", lineno, tmp->value.funcVal->name);

                                        }
                                    }

                                    if((yyvsp[-2].expression)->type == tableitem_e){
                                        emit(tablesetelem, (yyvsp[-2].expression), (yyvsp[-2].expression)->index, (yyvsp[0].expression), -1, lineno);
                                        (yyval.expression) = emit_iftableitem((yyvsp[-2].expression));
                                        (yyval.expression)->type = assignexpr_e;
                                    }
                                    else {
                                        emit(assign, (yyvsp[-2].expression), NULL, (yyvsp[0].expression), -1, lineno);
                                        (yyval.expression) = newexpr(assignexpr_e);
                                        (yyval.expression)->sym = newtemp();

                                        (yyval.expression)->sym->offset = currscopeoffset();
					                    (yyval.expression)->sym->space = currscopespace();
					                    incurrscopeoffset();


                                        
                                        emit(assign, (yyval.expression), NULL, (yyvsp[-2].expression), -1, lineno);
                                    
                                        if(currscopespace() == programvar)
                                            globalsSum++;
                                    }
                                }
#line 2648 "parser.c" /* yacc.c:1646  */
    break;

  case 38:
#line 1016 "parser.y" /* yacc.c:1646  */
    {
                                    printf("Error at line %d: cannot assign expr to a function call\n", lineno);
                                    //printf("assignexpr -> ASS expr\n");
                                }
#line 2657 "parser.c" /* yacc.c:1646  */
    break;

  case 39:
#line 1022 "parser.y" /* yacc.c:1646  */
    {//printf("primary -> lvalue \n"); $<expression>$ = emit_iftableitem($<expression>1);
}
#line 2664 "parser.c" /* yacc.c:1646  */
    break;

  case 40:
#line 1024 "parser.y" /* yacc.c:1646  */
    {//printf("primary -> call \n"); //$<stringValue>$ = $<stringValue>1;
                                }
#line 2671 "parser.c" /* yacc.c:1646  */
    break;

  case 41:
#line 1026 "parser.y" /* yacc.c:1646  */
    {//printf("primary -> call assignexpr\n");
            }
#line 2678 "parser.c" /* yacc.c:1646  */
    break;

  case 42:
#line 1028 "parser.y" /* yacc.c:1646  */
    {//printf("primary -> objectdef \n");
            }
#line 2685 "parser.c" /* yacc.c:1646  */
    break;

  case 43:
#line 1030 "parser.y" /* yacc.c:1646  */
    {
                                    //printf("primary -> LPAR funcdef RPAR \n");

                                    (yyval.expression) = newexpr(programfunc_e);
                                    (yyval.expression)->sym = (yyvsp[-1].symbol);
                                }
#line 2696 "parser.c" /* yacc.c:1646  */
    break;

  case 44:
#line 1036 "parser.y" /* yacc.c:1646  */
    {//printf("primary -> const \n");
                                    (yyval.expression) = (yyvsp[0].expression);
                                }
#line 2704 "parser.c" /* yacc.c:1646  */
    break;

  case 45:
#line 1041 "parser.y" /* yacc.c:1646  */
    {
                                    SymbolTableEntry * tmp;

                                    int scopeTmp;
                                    for(scopeTmp = currentScope; scopeTmp >= 0; scopeTmp--){

                                        tmp = lookUpSymbol(scopeTmp, (yyvsp[0].stringValue), currentScope);

                                        if(tmp != NULL){
                                            if(tmp->isActive){
                                                break;
                                            }
                                        }
                                    }

                                    if(tmp != NULL){
                                        if(!tmp->isActive){
                                            tmp = lookUpSymbol(-1, (yyvsp[0].stringValue), currentScope);
                                        }
                                    }

                                    /*menei na doume an exoume prosvasi*/
                                    if(tmp != NULL){

                                        if(!tmp->isActive){
                                            if(tmp->type == 3 || tmp->type == 4){
                                                printf("Error at line %d: cannot access symbol \"%s\" \n", lineno, tmp->value.funcVal->name);
                                            }
                                            else{
                                                printf("Error at line %d: cannot access symbol \"%s\" type:%d \n", lineno, tmp->value.varVal->name, tmp->type);
                                            }
                                        }
                                    }
                                    else {
                                        

                                        if(currentScope == 0){
                                            if(currscopespace() == programvar)
                                                globalsSum++;
                                            tmp = insert((yyvsp[0].stringValue), currentScope, lineno, NULL, var_s);
                                        }
                                        else {
                                            tmp = insert((yyvsp[0].stringValue), currentScope, lineno, NULL, var_s);
                                        }
                                        tmp->space = currscopespace();
                                        tmp->offset = currscopeoffset();
                                    }

                                    (yyval.expression) = lvalue_expr(tmp);
                                    //printf("lvalue -> ID \n");
                                }
#line 2760 "parser.c" /* yacc.c:1646  */
    break;

  case 46:
#line 1092 "parser.y" /* yacc.c:1646  */
    {
                                    int value = lookUp(currentScope, (yyvsp[0].stringValue), currentScope);
                                    SymbolTableEntry * tmp;

                                    if(value == 0){
                                        if(currentScope == 0){
                                            if(currscopespace() == programvar)
                                                globalsSum++;
                                                tmp = insert((yyvsp[0].stringValue), currentScope, lineno, NULL, var_s);
                                        }                                            
                                        else
                                            tmp = insert((yyvsp[0].stringValue), currentScope, lineno, NULL, var_s);

                                        tmp->space = currscopespace();
                                        tmp->offset = currscopeoffset();
                                        incurrscopeoffset();
                                    }
                                    else if(value == -1){
                                        printf("Error at line %d: collision with library function.\n", lineno);
                                    }
                                    tmp->type = var_s;
                                    (yyval.expression) = lvalue_expr(tmp);
                                    //printf("lvalue -> LOCAL ID \n");
                                }
#line 2789 "parser.c" /* yacc.c:1646  */
    break;

  case 47:
#line 1116 "parser.y" /* yacc.c:1646  */
    {
                                    int value = lookUp(0, (yyvsp[0].stringValue), currentScope);

                                    if(value == 0){
                                        printf("Error at line %d: There is no variable on global scope with id \"%s\".\n", lineno, (yyvsp[0].stringValue));
                                    }
                                    (yyval.expression) = (yyvsp[0].expression);
                                    //printf("lvalue -> DCOLON ID \n");

                                }
#line 2804 "parser.c" /* yacc.c:1646  */
    break;

  case 48:
#line 1126 "parser.y" /* yacc.c:1646  */
    {(yyval.expression) = (yyvsp[0].expression); isMember = 1;}
#line 2810 "parser.c" /* yacc.c:1646  */
    break;

  case 49:
#line 1129 "parser.y" /* yacc.c:1646  */
    {(yyval.expression) = member_item((yyvsp[-2].expression), (yyvsp[0].stringValue));}
#line 2816 "parser.c" /* yacc.c:1646  */
    break;

  case 50:
#line 1130 "parser.y" /* yacc.c:1646  */
    {

                                        (yyvsp[-3].expression) = emit_iftableitem((yyvsp[-3].expression));
                                        (yyval.expression) = newexpr(tableitem_e);
                                        (yyval.expression)->sym = (yyvsp[-3].expression)->sym;
                                        (yyval.expression)->index = (yyvsp[-1].expression);
                                    }
#line 2828 "parser.c" /* yacc.c:1646  */
    break;

  case 51:
#line 1137 "parser.y" /* yacc.c:1646  */
    {//printf("member -> call DOT ID \n");
            }
#line 2835 "parser.c" /* yacc.c:1646  */
    break;

  case 52:
#line 1139 "parser.y" /* yacc.c:1646  */
    {//printf("member -> call LBR expr RBR \n");
            }
#line 2842 "parser.c" /* yacc.c:1646  */
    break;

  case 53:
#line 1143 "parser.y" /* yacc.c:1646  */
    {
                                        //printf("call -> call LPAR elist RPAR \n");
                                        (yyval.expression) = make_call((yyvsp[-3].expression), (yyvsp[-1].expression));
                                    }
#line 2851 "parser.c" /* yacc.c:1646  */
    break;

  case 54:
#line 1147 "parser.y" /* yacc.c:1646  */
    {//printf("call -> lvalue callsuffix \n");
                                        if((yyvsp[0].calls)->method==1){
                                            expr* self = (yyvsp[-1].expression);
                                            (yyvsp[-1].expression) = emit_iftableitem(member_item(self,(yyvsp[0].calls)->name));
                                            (yyvsp[0].calls)->elist=insertToExprList((yyvsp[0].calls)->elist,self);
                                        }

                                        (yyval.expression) = make_call((yyvsp[-1].expression), (yyvsp[0].calls)->elist);
                                    }
#line 2865 "parser.c" /* yacc.c:1646  */
    break;

  case 55:
#line 1156 "parser.y" /* yacc.c:1646  */
    {
                                                    //printf("call -> LPAR funcdef RPAR LPAR elist RPAR \n");
                                                    expr * func = newexpr(programfunc_e);
                                                    func->sym = (yyvsp[-4].symbol);
                                                    (yyval.expression) = make_call(func, (yyvsp[-1].expression));
                                                }
#line 2876 "parser.c" /* yacc.c:1646  */
    break;

  case 56:
#line 1164 "parser.y" /* yacc.c:1646  */
    {//printf("callsuffix -> normcall \n"); $$ = $1;
}
#line 2883 "parser.c" /* yacc.c:1646  */
    break;

  case 57:
#line 1166 "parser.y" /* yacc.c:1646  */
    {//printf("callsuffix -> methodcall \n"); $$ = $1;
            }
#line 2890 "parser.c" /* yacc.c:1646  */
    break;

  case 58:
#line 1170 "parser.y" /* yacc.c:1646  */
    {
                                        //printf("normcall -> LPAR elist RPAR \n");
                                        (yyval.calls) = malloc(sizeof(callstr));
                                        (yyval.calls)->elist = (yyvsp[-1].expression);
                                        (yyval.calls)->method = 0;
                                        (yyval.calls)->name = NULL;
                                    }
#line 2902 "parser.c" /* yacc.c:1646  */
    break;

  case 59:
#line 1179 "parser.y" /* yacc.c:1646  */
    {
                                        //printf("methodcall -> DDOT ID LPAR elist RPAR \n");
                                        (yyval.calls) = malloc(sizeof(callstr));
                                        (yyval.calls)->elist = (yyvsp[-1].expression);
                                        (yyval.calls)->method = 1;
                                        (yyval.calls)->name = strdup((yyvsp[-3].stringValue));
                                    }
#line 2914 "parser.c" /* yacc.c:1646  */
    break;

  case 60:
#line 1188 "parser.y" /* yacc.c:1646  */
    {//printf("elist -> expr \n"); $<expression>$ = insertToExprList(NULL, $<expression>1);
}
#line 2921 "parser.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1190 "parser.y" /* yacc.c:1646  */
    {//printf("elist -> elist COMMA expr \n");
                                        (yyval.expression) = insertToExprList((yyvsp[-2].expression), (yyvsp[0].expression));
                                    }
#line 2929 "parser.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1193 "parser.y" /* yacc.c:1646  */
    {//printf("elist -> empty \n"); $$ = NULL;
            }
#line 2936 "parser.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1197 "parser.y" /* yacc.c:1646  */
    {
                                        //printf("objectdef -> LBR elist RBR \n");

                                        expr * t = newexpr(newtable_e);
                                        t->sym = newtemp();
                                        emit(tablecreate, t, NULL, NULL, -1, lineno);

                                        int i = 0;
                                        expr * tmp = NULL;
                                        tmp = (yyvsp[-1].expression);

                                        if(currscopespace() == programvar)
                                            globalsSum++;

                                        while(tmp != NULL){

                                            emit(tablesetelem, t, newexpr_constnum(i++), tmp, -1, lineno);
                                            tmp = tmp->next;
                                        }


                                        (yyval.expression) = t;
                                    }
#line 2964 "parser.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1220 "parser.y" /* yacc.c:1646  */
    {
                                        //printf("objectdef -> LBR indexed RBR \n");


                                        expr * t = newexpr(newtable_e);
                                        t->sym = newtemp();
                                        emit(tablecreate, t, NULL, NULL, -1, lineno);

                                        if(currscopespace() == programvar)
                                            globalsSum++;

                                        int i;
                                        expr * tmp = NULL;
                                        tmp = (yyvsp[-1].expression);
                                        while(tmp != NULL){
                                            emit(tablesetelem, t, tmp, tmp->index, -1, lineno);

                                            tmp = tmp->next;
                                        }
                                        (yyval.expression) = t;
                                    }
#line 2990 "parser.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1243 "parser.y" /* yacc.c:1646  */
    {//printf("indexed -> indexedelem \n");
                                            (yyval.expression) = (yyvsp[0].expression);
                                        }
#line 2998 "parser.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1246 "parser.y" /* yacc.c:1646  */
    {//printf("indexed -> indexed COMMA indexedelem \n");
                                            (yyval.expression) = (yyvsp[-2].expression);
                                            (yyval.expression)->next = (yyvsp[0].expression);
                                        }
#line 3007 "parser.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1252 "parser.y" /* yacc.c:1646  */
    {
                                                //printf("indexedelem -> LCURBR expr COLON expr RCURBR \n");

                                                (yyval.expression) = (yyvsp[-3].expression);
                                                (yyval.expression)->index = (yyvsp[-1].expression);
                                            }
#line 3018 "parser.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1260 "parser.y" /* yacc.c:1646  */
    {//printf("blocknext -> stmt blocknext \n");
}
#line 3025 "parser.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1262 "parser.y" /* yacc.c:1646  */
    {//printf("blocknext -> empty \n");
            }
#line 3032 "parser.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1266 "parser.y" /* yacc.c:1646  */
    {
                        currentScope++;
                        if(maxScope < currentScope){
                            maxScope = currentScope;
                        }

                    }
#line 3044 "parser.c" /* yacc.c:1646  */
    break;

  case 71:
#line 1275 "parser.y" /* yacc.c:1646  */
    {
                                        hide(currentScope);
                                        currentScope--;

                                        //printf("block -> LCURBR blocknext RCURBR \n");
                                     }
#line 3055 "parser.c" /* yacc.c:1646  */
    break;

  case 72:
#line 1284 "parser.y" /* yacc.c:1646  */
    {
                        (yyval.stringValue) = (yyvsp[0].stringValue);
                    }
#line 3063 "parser.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1287 "parser.y" /* yacc.c:1646  */
    {

                        anonFuncCounter++;

                        char name[10];
                        sprintf(name, "$f%d", anonFuncCounter);

                        insert(name, currentScope, lineno, idlistarray, programfunc_s);

                        (yyval.stringValue) = strdup(name);
                    }
#line 3079 "parser.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1301 "parser.y" /* yacc.c:1646  */
    {
                                    insideFunction++;

                                    (yyval.symbol) = lookUpSymbol(currentScope, (yyvsp[0].stringValue), currentScope);
                                    if((yyval.symbol) == NULL){
                                        (yyval.symbol) = insert((yyvsp[0].stringValue), currentScope, lineno, NULL, programfunc_s);
                                    }
                                    (yyval.symbol)->iaddress = nextquadlabel();

                                    expr * tmp;
                                    tmp = lvalue_expr((yyval.symbol));



                                    emit(jump, NULL, NULL, NULL, 0, lineno);



                                    emit(funcstart, tmp, NULL, NULL, -1, lineno);

                                    push(scopeoffsetStack, currscopeoffset());

                                    enterscopespace();
                                    resetformalargsoffset();
                                }
#line 3109 "parser.c" /* yacc.c:1646  */
    break;

  case 75:
#line 1328 "parser.y" /* yacc.c:1646  */
    {
                                    enterscopespace();
                                    resetfunctionlocalsoffset();
                                }
#line 3118 "parser.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1334 "parser.y" /* yacc.c:1646  */
    {
                                    (yyval.intValue) = currscopeoffset();
                                    exitscopespace();
                                }
#line 3127 "parser.c" /* yacc.c:1646  */
    break;

  case 77:
#line 1340 "parser.y" /* yacc.c:1646  */
    {
                                    push_loop(loopcounterstack, loopcounter);
                                    loopcounter = 0;

                                }
#line 3137 "parser.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1347 "parser.y" /* yacc.c:1646  */
    {
                                    loopcounter = pop_and_top_loop(loopcounterstack);
                                }
#line 3145 "parser.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1352 "parser.y" /* yacc.c:1646  */
    {
                                                                            exitscopespace();
                                                                            (yyvsp[-4].symbol)->totalLocals = (yyvsp[-2].intValue);
                                                                            int offset;
                                                                            offset = pop_and_top(scopeoffsetStack);
                                                                            restorecurrscopeoffset(offset);
                                                                            (yyval.symbol) = (yyvsp[-4].symbol);

                                                                            expr * tmp;
                                                                            tmp = lvalue_expr((yyvsp[-4].symbol));
                                                                            emit(funcend, tmp, NULL, NULL, -1, lineno);


                                                                            patchlabel((yyval.symbol)->iaddress, nextquad());

                                                                            firstFunction = 0;
                                                                            secondFunction = 0;

                                                                        }
#line 3169 "parser.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1373 "parser.y" /* yacc.c:1646  */
    {//printf("const -> INTEGER \n");

                                        (yyval.expression) = newexpr_constint((yyvsp[0].intValue));
                                    }
#line 3178 "parser.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1377 "parser.y" /* yacc.c:1646  */
    {//printf("const -> REAL \n");

                                        (yyval.expression) = newexpr_constnum((yyvsp[0].realValue));
                                    }
#line 3187 "parser.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1381 "parser.y" /* yacc.c:1646  */
    {
                                        //printf("const -> STRING \n");
                                        (yyval.expression) = newexpr_conststring((yyvsp[0].stringValue));
                                    }
#line 3196 "parser.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1385 "parser.y" /* yacc.c:1646  */
    {//printf("const -> NIL \n");
                                        (yyval.expression) = newexpr(nil_e);
                                    }
#line 3204 "parser.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1388 "parser.y" /* yacc.c:1646  */
    {//printf("const -> TRUE \n");
                                        (yyval.expression) = newexpr_constbool(1);
                                    }
#line 3212 "parser.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1391 "parser.y" /* yacc.c:1646  */
    {//printf("const -> FALSE \n");
                                        (yyval.expression) = newexpr_constbool(0);
                                    }
#line 3220 "parser.c" /* yacc.c:1646  */
    break;

  case 86:
#line 1396 "parser.y" /* yacc.c:1646  */
    {//printf("idlistnext -> COMMA idlist \n");
}
#line 3227 "parser.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1398 "parser.y" /* yacc.c:1646  */
    {//printf("idlistnext -> empty \n");
            }
#line 3234 "parser.c" /* yacc.c:1646  */
    break;

  case 88:
#line 1402 "parser.y" /* yacc.c:1646  */
    {
                                        if(idlistarray == NULL){
                                            idlistarray = malloc(strlen((yyvsp[-1].stringValue)) + 1);
                                            strcpy(idlistarray, (yyvsp[-1].stringValue));
                                        }
                                        else {

                                            tmptok = malloc(strlen(idlistarray) + strlen((yyvsp[-1].stringValue)) + 1);

                                            strcpy(tmptok, (yyvsp[-1].stringValue));
                                            strcat(tmptok, ",");
                                            strcat(tmptok, idlistarray);

                                            idlistarray = tmptok;
                                        }

                                        //printf("idlist -> ID idlistnext \n");
                                    }
#line 3257 "parser.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1420 "parser.y" /* yacc.c:1646  */
    {//printf("idlist -> empty \n");
            }
#line 3264 "parser.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1424 "parser.y" /* yacc.c:1646  */
    {
                                        emit(if_eq, (yyvsp[-1].expression), newexpr_constbool(1), NULL, nextquad() + 2, lineno);

                                        (yyval.unsignedvalue) = nextquad();
                                        emit(jump, NULL, NULL, NULL, 0, lineno);
                                    }
#line 3275 "parser.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1432 "parser.y" /* yacc.c:1646  */
    {
                                        (yyval.unsignedvalue) = nextquad();
                                        emit(jump, NULL, NULL, NULL, 0, lineno);
                                    }
#line 3284 "parser.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1438 "parser.y" /* yacc.c:1646  */
    {
                                                patchlabel((yyvsp[-1].unsignedvalue), nextquad());
                                            }
#line 3292 "parser.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1441 "parser.y" /* yacc.c:1646  */
    {
                                                patchlabel((yyvsp[-3].unsignedvalue), (yyvsp[-1].unsignedvalue) + 1);
                                                patchlabel((yyvsp[-1].unsignedvalue), nextquad());
                                            }
#line 3301 "parser.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1447 "parser.y" /* yacc.c:1646  */
    {
                                breakflag++;
                                continueflag++;
                                (yyval.unsignedvalue) = nextquad();
                            }
#line 3311 "parser.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1454 "parser.y" /* yacc.c:1646  */
    {
                                emit(if_eq, (yyvsp[-1].expression), newexpr_constbool(1), NULL, nextquad() + 2, lineno);
                                (yyval.unsignedvalue) = nextquad();
                                emit(jump, NULL, NULL, NULL, 0, lineno);
                            }
#line 3321 "parser.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1461 "parser.y" /* yacc.c:1646  */
    {++loopcounter;}
#line 3327 "parser.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1464 "parser.y" /* yacc.c:1646  */
    {--loopcounter;}
#line 3333 "parser.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1467 "parser.y" /* yacc.c:1646  */
    {
                                        (yyval.forstmt_t) = malloc(sizeof(stmt));
                                        (yyvsp[-1].forstmt_t) = malloc(sizeof(stmt));

                                        (yyval.forstmt_t) = (yyvsp[-1].forstmt_t);
                                    }
#line 3344 "parser.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1475 "parser.y" /* yacc.c:1646  */
    {
                                            emit(jump, NULL, NULL, NULL, (yyvsp[-2].unsignedvalue), lineno);
                                            patchlabel((yyvsp[-1].unsignedvalue), nextquad());


                                            patchlist(s.breakList, nextquad());
                                            patchlist(s.contList, (yyvsp[-2].unsignedvalue));

                                            s.breakList = 0;
                                            s.contList = 0;

                                            breakflag--;
                                            continueflag--;
                                        }
#line 3363 "parser.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1491 "parser.y" /* yacc.c:1646  */
    {
                (yyval.unsignedvalue) = nextquad();
                emit(jump, NULL, NULL, NULL, 0, lineno);
            }
#line 3372 "parser.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1497 "parser.y" /* yacc.c:1646  */
    {
                (yyval.unsignedvalue) = nextquad();
            }
#line 3380 "parser.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1502 "parser.y" /* yacc.c:1646  */
    {
                        breakflag = 1; continueflag = 1;
                    }
#line 3388 "parser.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1507 "parser.y" /* yacc.c:1646  */
    {
                                                            (yyval.forstct) = malloc(sizeof(forstruct));
                                                            (yyval.forstct)->test = (yyvsp[-2].unsignedvalue);
                                                            (yyval.forstct)->enter = nextquad();
                                                            emit(if_eq, (yyvsp[-1].expression), newexpr_constbool(1), NULL, 0, lineno);
                                                        }
#line 3399 "parser.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1515 "parser.y" /* yacc.c:1646  */
    {
                                                            breakflag = 0; continueflag = 0;

                                                            patchlabel((yyvsp[-6].forstct)->enter, (yyvsp[-2].unsignedvalue) + 1);
                                                            patchlabel((yyvsp[-5].unsignedvalue), nextquad());
                                                            patchlabel((yyvsp[-2].unsignedvalue), (yyvsp[-6].forstct)->test);
                                                            patchlabel((yyvsp[0].unsignedvalue), (yyvsp[-5].unsignedvalue));

                                                            patchlist((yyvsp[-1].forstmt_t)->breakList, nextquad());
                                                            patchlist((yyvsp[-1].forstmt_t)->contList, (yyvsp[-5].unsignedvalue) + 1);
                                                        }
#line 3415 "parser.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1528 "parser.y" /* yacc.c:1646  */
    {
                                                    if(insideFunction == 0)
                                                        printf("Error at line %d: use of \"return\" while not in a function\n", lineno);

                                                    emit(ret, NULL, NULL, NULL, -1, lineno);
                                                    //printf("returnstmt -> RETURN SEMIC \n");
                                                }
#line 3427 "parser.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1535 "parser.y" /* yacc.c:1646  */
    {
                                                    if(insideFunction == 0)
                                                        printf("Error at line %d: use of \"return\" while not in a function\n", lineno);

                                                    emit(ret, (yyvsp[-1].expression), NULL, NULL, -1, lineno);
                                                    //printf("return -> RETURN expr SEMIC \n");
                                                }
#line 3439 "parser.c" /* yacc.c:1646  */
    break;


#line 3443 "parser.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1544 "parser.y" /* yacc.c:1906  */


int yyerror (const char * YaccProvidedMessage){
	return 0;
}

int main(int argc, char** argv){

    scopeoffsetStack = malloc(sizeof(scopeoffsetstack_t));
    loopcounterstack = malloc(sizeof(loopcounterstack_t));

    createEmptyStack(scopeoffsetStack);
    createEmptyStack_loop(loopcounterstack);

    if(argc > 1){

        insertLibFunctions();

        yyin = fopen(argv[1], "r");
        yyparse();

        printQuads();


        generate();
        printConsts(globalsSum);

        //printConsts();
        //readBinary();

        //avm();
    }else{
      /*
      //printf("============ TEST HERE ============\n");

      iopcode op = 1;

      expr* result = newexpr(8);
      result->numConst = 1;

      expr* arg1 = newexpr(9);
      arg1->boolConst = 3;

      expr* arg2 = newexpr(8);
      arg2->numConst = 3;


      //printf("==================================\n");
      printQuadList();
      */
    }
    return 0;
}
