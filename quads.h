// Quads/Types declarations
//

#include <stddef.h>
#include <string.h>
#include "symbolTable.h"

#define MAX 10000


#define EXPAND_SIZE 1024
#define CURR_SIZE   (total*sizeof(quad))
#define NEW_SIZE    (EXPAND_SIZE*sizeof(quad)+CURR_SIZE)


typedef enum expr_t{
  var_e,
  tableitem_e,

  programfunc_e,
  libraryfunc_e,

  arithexpr_e,
  boolexpr_e,
  assignexpr_e,
  newtable_e,

  constnum_e,
  constbool_e,
  conststring_e,

  nil_e
}expr_t;

typedef enum iopcode{
  assign,         add,            sub,
  mul,            divide,         mod,
  uminus,         and,            or,
  not,            if_eq,          if_noteq,
  if_lesseq,      if_greatereq,   if_less,
  if_greater,     call,           param,
  ret,            getretval,      funcstart,
  funcend,        tablecreate,    tablegetelem,
  tablesetelem,   jump
}iopcode;

typedef struct expr{
  expr_t            type;
  SymbolTableEntry* sym;
  struct expr*      index;
  double            numConst;
  char*             strConst;
  unsigned char     boolConst;
  struct expr*      next;
}expr;

typedef struct quad{
  enum iopcode     op;
  struct expr*     result;
  struct expr*     arg1;
  struct expr*     arg2;
  unsigned         label;
  unsigned         line;
  unsigned         taddress;
}quad;

typedef struct callstr {
  expr * elist;
  unsigned char method;
  char * name;
}callstr;

typedef struct forstruct {
  unsigned test;
  unsigned enter;
}forstruct;


typedef struct stmt {
  int breakList;
  int contList;
}stmt;


typedef struct scopeoffsetstack
{
  unsigned offsets[MAX];
  int top;
}scopeoffsetstack_t;

typedef struct loopcounterstack
{
  int loopcounters[MAX];
  int top;
}loopcounterstack_t;

// Printing field based on type of expression
void printField(struct expr* ptr);
// Printing the given quad
void printQuad(quad* q);
// Printing whole QuadList
void printQuadList();

// Emiting quads
void emit(iopcode op, expr* result, expr* arg1, expr* arg2, unsigned label, unsigned line);

expr* newexpr(expr_t t);

expr* lvalue_expr(SymbolTableEntry* sym);

expr * member_item(expr* lv, char* name);

expr * newexpr_conststring(char * name);

expr * emit_iftableitem(expr * e);

expr * make_call(expr * lv, expr * reversed_elist);

expr * newexpr_constnum(double i);

void check_arith(expr * e);

unsigned nextquadlabel();

unsigned nextquad();

expr * newexpr_constbool(unsigned int b);

void patchlabel(unsigned quadNo, unsigned label);

void make_stmt(stmt * s);

int newlist(int i);

int mergelist(int l1, int l2);

void patchlist(int list, int label);

void createEmptyStack_loop(loopcounterstack_t *s);

int isfull_loop(loopcounterstack_t *s);

int isempty_loop(loopcounterstack_t *s);

void push_loop(loopcounterstack_t *s, unsigned newitem);

void pop_loop (loopcounterstack_t *s);

int pop_and_top_loop (loopcounterstack_t *s);

void createEmptyStack(scopeoffsetstack_t *s);

int isfull(scopeoffsetstack_t *s);

int isempty(scopeoffsetstack_t *s);

void push(scopeoffsetstack_t *s, unsigned newitem);

void pop (scopeoffsetstack_t *s);

unsigned pop_and_top (scopeoffsetstack_t *s);

void printQuads();

expr * newexpr_constint(int i);

expr * insertToExprList(expr * head, expr * expre);