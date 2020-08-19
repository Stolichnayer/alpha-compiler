#include "quads.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Dynamically allocated array storing quads
quad* quads = NULL;
unsigned total = 0;
unsigned int currQuad = 0;

extern int lineno;



void printQuads(){

	int i = 0;
	int counter = 1;
	quad * quad = quads;
	iopcode op;
	expr* quad_result;
	expr* quad_arg1;
	expr* quad_arg2;


	fprintf(stdout, "quad#			opcode			result			arg1			arg2			label\n-----------------------------------------------------------------------------------------------------------------------------\n");

	for(i = 0; i < currQuad; i++)
	{


		fprintf(stdout, "%d:\t\t\t", i);
		op = quad->op;


		if(op == assign){
			fprintf(stdout, "%s\t\t\t", "assign");
		}
		else if(op == add){
			fprintf(stdout, "%s\t\t\t", "add");
		}
		else if(op == sub){
			fprintf(stdout, "%s\t\t\t", "sub");
		}
		else if(op == mul){
			fprintf(stdout, "%s\t\t\t", "mul");
		}
		else if(op == divide){
			fprintf(stdout, "%s\t\t\t", "divide");
		}
		else if(op == mod){
			fprintf(stdout, "%s\t\t\t", "mod");
		}
		else if(op == uminus){
			fprintf(stdout, "%s\t\t\t", "uminus");
		}
		else if(op == and){
			fprintf(stdout, "%s\t\t\t", "and");
		}
		else if(op == or){
			fprintf(stdout, "%s\t\t\t", "or");
		}
		else if(op == not){
			fprintf(stdout, "%s\t\t\t", "not");
		}
		else if(op == if_eq){
			fprintf(stdout, "%s\t\t\t", "if_eq");
		}
		else if(op == if_noteq){
			fprintf(stdout, "%s\t\t", "if_noteq");
		}
		else if(op == if_lesseq){
			fprintf(stdout, "%s\t\t", "if_lesseq");
		}
		else if(op == if_greatereq){
			fprintf(stdout, "%s\t\t", "if_greatereq");
		}
		else if(op == if_less){
			fprintf(stdout, "%s\t\t\t", "if_less");
		}
		else if(op == if_greater){
			fprintf(stdout, "%s\t\t", "if_greater");
		}
		else if(op == jump){
			fprintf(stdout, "%s\t\t\t", "jump");
		}
		else if(op == call){
			fprintf(stdout, "%s\t\t\t", "call");
		}
		else if(op == param){
			fprintf(stdout, "%s\t\t\t", "param");
		}
		else if(op == ret){
			fprintf(stdout, "%s\t\t\t", "ret");
		}
		else if(op == getretval){
			fprintf(stdout, "%s\t\t", "getretval");
		}
		else if( op == funcstart){
			fprintf(stdout, "%s\t\t\t", "funcstart");
		}
		else if(op == funcend){
			fprintf(stdout, "%s\t\t\t", "funcend");
		}
		else if(op == tablecreate){
			fprintf(stdout, "%s\t\t", "tablecreate");
		}
		else if(op == tablegetelem){
			fprintf(stdout, "%s\t\t", "tablegetelem");
		}
		else if(op == tablesetelem){
			fprintf(stdout, "%s\t\t", "tablesetelem");
		}



		if((quad_result = quad -> result)!= NULL){
			if(quad_result -> type == var_e || quad_result -> type == programfunc_e || quad_result -> type == libraryfunc_e ||quad_result -> type == tableitem_e ||
          quad_result -> type == boolexpr_e || quad_result -> type == arithexpr_e ||
          quad_result -> type == newtable_e || quad_result -> type == assignexpr_e){
        if(quad_result -> sym){
          if(quad_result -> sym -> type == 0 || quad_result -> sym -> type == 1 || quad_result -> sym -> type == 2)
            fprintf(stdout, "%s", quad_result -> sym ->value.funcVal -> name);
          else
            fprintf(stdout, "%s", quad_result -> sym ->value.varVal -> name);
        }

      }
			else if(quad_result -> type == constnum_e){
				fprintf(stdout, "%.2f", quad_result -> numConst);
			}
			else if(quad_result -> type == constbool_e){
				if (quad->result->boolConst == 1) {
					fprintf(stdout, "TRUE");
				}
				else {
					fprintf(stdout, "FALSE");
				}

			}
			else if(quad_result -> type == conststring_e){
				fprintf(stdout, "%s", quad_result -> strConst);
			}
			else if(quad_result -> type == nil_e){
				fprintf(stdout, "NILL");
			}
		}

		fprintf(stdout, "\t\t\t");

    ///////////////////////////////////////////////////////////////////////
    //arg1


		if((quad_arg1 = quad -> arg1) != NULL){
			if(quad_arg1 -> type == var_e || quad_arg1 -> type == programfunc_e || quad_arg1 -> type == libraryfunc_e ||quad_arg1 -> type == tableitem_e ||
          quad_arg1 -> type == arithexpr_e || quad_arg1 -> type == newtable_e || quad_arg1 -> type == assignexpr_e){
        if(quad_arg1 -> sym -> type == 3 || quad_arg1 -> sym -> type == 4)
					fprintf(stdout, "%s", quad_arg1 -> sym ->value.funcVal -> name);
        else
          fprintf(stdout, "%s", quad_arg1 -> sym ->value.varVal -> name);
      }
			else if(quad_arg1 -> type == boolexpr_e){
        if(quad_arg1 -> sym -> type == 3 || quad_arg1 -> sym -> type == 4){
          if (quad->arg1->sym->value.funcVal->name != NULL) {
						fprintf(stdout, "%s", quad->arg1->sym->value.funcVal->name);
          }
          else
          {
            if (quad->arg1->boolConst == 1) {
              fprintf(stdout, "TRUE");
            }
            else {
              fprintf(stdout, "FALSE");
            }
          }
        }
        else{
          if (quad->arg1->sym->value.varVal->name != NULL) {
						fprintf(stdout, "%s", quad->arg1->sym->value.varVal->name);
          }
          else
          {
            if (quad->arg1->boolConst == 1) {
              fprintf(stdout, "TRUE");
            }
            else {
              fprintf(stdout, "FALSE");
            }
          }
        }

			}
			else if(quad_arg1 -> type == constnum_e){
				fprintf(stdout, "%.2f", quad_arg1 -> numConst);
			}
			else if(quad_arg1 -> type == constbool_e){
				if(quad->arg1->sym)
				{
          if(quad_arg1 -> sym -> type == 3 || quad_arg1 -> sym -> type == 4){
            if (quad->arg1->sym->value.funcVal->name != NULL) {
              fprintf(stdout, "%s", quad->arg1->sym->value.funcVal->name);
            }
          }
          else{
            if (quad->arg1->sym->value.varVal->name != NULL) {
              fprintf(stdout, "%s", quad->arg1->sym->value.varVal->name);
            }
          }

				}
				else
				{
					if (quad->arg1->boolConst == 1) {
						fprintf(stdout, "TRUE");
					}
					else {
						fprintf(stdout, "FALSE");
					}
				}

			}
			else if(quad_arg1 -> type == conststring_e){
				fprintf(stdout, "%s", quad_arg1 -> strConst);
			}
			else if(quad_arg1 -> type == nil_e){
				fprintf(stdout, "NILL");
			}
		}

		fprintf(stdout, "\t\t\t");

		//////////////////////////////////////////////////////////////////////////////
    //arg2

		if((quad_arg2 = quad -> arg2) != NULL){
			if(quad_arg2 -> type == var_e || quad_arg2 -> type == programfunc_e || quad_arg2 -> type == libraryfunc_e ||quad_arg2 -> type == tableitem_e ||
          quad_arg2 -> type == arithexpr_e || quad_arg2 -> type == newtable_e || quad_arg2 -> type == assignexpr_e){
        if(quad_arg2 -> sym -> type == 3 || quad_arg2 -> sym -> type == 4)
				  fprintf(stdout, "%s", quad_arg2 -> sym ->value.funcVal -> name);
        else
          fprintf(stdout, "%s", quad_arg2 -> sym ->value.varVal -> name);
      }
			else if(quad_arg2 -> type == boolexpr_e){

        if(quad_arg2 -> sym -> type == 3 || quad_arg2 -> sym -> type == 4){
          if (quad->arg2->sym->value.funcVal->name != NULL) {
						fprintf(stdout, "%s", quad->arg2->sym->value.funcVal->name);
          }
          else
          {
            if (quad->arg2->boolConst == 1) {
              fprintf(stdout, "TRUE");
            }
            else {
              fprintf(stdout, "FALSE");
            }
          }
        }
        else{
          if (quad->arg2->sym->value.varVal->name != NULL) {
						fprintf(stdout, "%s", quad->arg2->sym->value.varVal->name);
          }
          else
          {
            if (quad->arg2->boolConst == 1) {
              fprintf(stdout, "TRUE");
            }
            else {
              fprintf(stdout, "FALSE");
            }
          }
        }

			}
			else if(quad_arg2 -> type == constnum_e){
				fprintf(stdout, "%.2f", quad_arg2 -> numConst);
			}
			else if(quad_arg2 -> type == constbool_e){
				if(quad->arg2->sym)
				{
          if(quad_arg2 -> sym -> type == 3 || quad_arg2 -> sym -> type == 4){
            if (quad->arg2->sym->value.funcVal->name != NULL) {
              fprintf(stdout, "%s", quad->arg2->sym->value.funcVal->name);
            }
          }
          else{
            if (quad->arg2->sym->value.varVal->name != NULL) {
              fprintf(stdout, "%s", quad->arg2->sym->value.varVal->name);
            }
          }

				}
				else
				{
					if (quad->arg2->boolConst == 1) {
						fprintf(stdout, "TRUE");
					}
					else {
						fprintf(stdout, "FALSE");
					}
				}

			}
			else if(quad_arg2 -> type == conststring_e){
				fprintf(stdout, "%s", quad_arg2 -> strConst);
			}
			else if(quad_arg2 -> type == nil_e){
				fprintf(stdout, "NILl");
			}
		}

		fprintf(stdout, "\t\t\t");


    //////////////////////////////////////////////////////////


		if(quad->label != -1){
			fprintf(stdout, "%d\n", quad->label);
		}else{
			fprintf(stdout, "\n");
		}

		quad = quad + 1;
		counter++;

	}

}

// Emiting quads
void expand(void){
    assert(total == currQuad);
    quad * p = malloc(NEW_SIZE);

    if(quads){
        memcpy(p, quads, CURR_SIZE);
        free(quads);
    }
    quads = p;
    total += EXPAND_SIZE;
}

void emit(iopcode op,  expr* result,  expr* arg1,  expr* arg2,  unsigned label, unsigned line){

  if(currQuad == total){
    expand();
  }
  quad* newQuad = quads + currQuad++;
  // Filling new Quad
  newQuad->op     = op;
  newQuad->result = result;
  newQuad->arg1   = arg1;
  newQuad->arg2   = arg2;
  newQuad->label  = label;
  newQuad->line   = line;
}

expr* newexpr(expr_t t){
  expr* e = (expr*) malloc(sizeof(expr));
  memset(e,0,sizeof(expr));
  e->type = t;
  //e->next = NULL;
  return e;
}

expr* lvalue_expr(SymbolTableEntry* sym){
  assert(sym);
  expr* e = (expr*) malloc(sizeof(expr));
  memset(e,0,sizeof(expr));

  e->next = (expr*) 0;
  e->sym = sym;

  switch (sym->type) {
    case var_s: e->type = var_e; break;
    case programfunc_s: e->type = programfunc_e; break;
    case libraryfunc_s: e->type = libraryfunc_e; break;
    default: assert(0);
  }
  return e;
}

expr * member_item(expr* lv, char* name){
  lv = emit_iftableitem(lv);
  expr* ti = newexpr(tableitem_e);
  ti->sym = lv->sym;
  ti->index = newexpr_conststring(name);
  return ti;
}

expr * newexpr_conststring(char * s){
  expr * e = newexpr(conststring_e);
  e->strConst = strdup(s);
  return e;
}

unsigned nextquadlabel(void){
    return currQuad;
}

unsigned nextquad(void){
    return currQuad;
}

void patchlabel(unsigned quadNo, unsigned label){
    assert(quadNo < currQuad && !quads[quadNo].label);
    quads[quadNo].label = label;
}

expr * newexpr_constnum(double i){
  expr * e = newexpr(constnum_e);
  e->numConst = i;
  return e;
}

expr * newexpr_constint(int i){
  expr * e = newexpr(constnum_e);
  e->numConst = i;
  return e;
}

void check_arith(expr * e){
  if(	e->type == constbool_e		||
		e->type == conststring_e	||
		e->type == nil_e			||
		e->type == newtable_e		||
		e->type == programfunc_e 	||
		e->type == libraryfunc_e 	||
		e->type == boolexpr_e)
    printf("Illegal expr used\n");
}

expr * newexpr_constbool(unsigned int b){
  expr * e = newexpr(constbool_e);
  e->boolConst = !!b;
  return e;
}

void make_stmt(stmt * s){
  s->breakList = s->contList = 0;
}

int newlist(int i){
  quads[i].label = 0;
  return i;
}

int mergelist(int l1, int l2){
  if(!l1)
    return l2;
  else if(!l2)
    return l1;
  else {
    int i = l1;
    while(quads[i].label)
      i = quads[i].label;
    quads[i].label = l2;
    return l1;
  }
}

void patchlist(int list, int label){

  while(list){
	  int next = quads[list-1].label;


    quads[list-1].label = label;
    list = next;
  }
}

expr * insertToExprList(expr * head, expr * expre){

  expr * tmphead = head;
  expr * tmpexpr = expre;

  if(tmphead == NULL){
    head = expre;
    head->next = NULL;
    return head;
  }

  while(tmphead->next != NULL){
    tmphead = tmphead->next;
  }
  tmphead->next = tmpexpr;

  return head;
}

////////////////////////////////////////////////
void createEmptyStack(scopeoffsetstack_t *s)
{
    s->top=-1;
}

int isfull(scopeoffsetstack_t *s)
{
    if (s->top==MAX-1)
        return 1;
    else
        return 0;
}

int isempty(scopeoffsetstack_t *s)
{
    if (s->top==-1)
        return 1;
    else
        return 0;
}

void push(scopeoffsetstack_t *s, unsigned newitem)
{
    if (isfull(s))
    {
        printf("STACK FULL");
    }
    else
    {
        s->top++;
        s->offsets[s->top]=newitem;
    }
}


void pop (scopeoffsetstack_t *s)
{
    if (isempty(s))
    {
        printf("\n STACK EMPTY \n");
    }
    else
    {
        printf("Item popped= %d",s->offsets[s->top]);
        s->top--;
    }
}

unsigned pop_and_top (scopeoffsetstack_t *s)
{
    if (isempty(s))
    {
        printf("\n STACK EMPTY \n");
    }
    else
    {
        printf("Item popped= %d",s->offsets[s->top]);
        s->top--;
        return s->offsets[s->top + 1];
    }
}

///////////////////////////////////////////////////////////

void createEmptyStack_loop(loopcounterstack_t *s)
{
    s->top=-1;
}

int isfull_loop(loopcounterstack_t *s)
{
    if (s->top==MAX-1)
        return 1;
    else
        return 0;
}

int isempty_loop(loopcounterstack_t *s)
{
    if (s->top==-1)
        return 1;
    else
        return 0;
}

void push_loop(loopcounterstack_t *s, unsigned newitem)
{
    if (isfull_loop(s))
    {
        printf("STACK FULL");
    }
    else
    {
        s->top++;
        s->loopcounters[s->top]=newitem;
    }
}

void pop_loop (loopcounterstack_t *s)
{
    if (isempty_loop(s))
    {
        printf("\n STACK EMPTY \n");
    }
    else
    {
        printf("Item popped= %d",s->loopcounters[s->top]);
        s->top--;
    }
}

int pop_and_top_loop (loopcounterstack_t *s)
{
    if (isempty_loop(s))
    {
        printf("\n STACK EMPTY \n");
    }
    else
    {
        printf("Item popped= %d",s->loopcounters[s->top]);
        s->top--;
        return s->loopcounters[s->top + 1];
    }
}
