#include "targetCode.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned totalInstructions = 0;

incomplete_jump *ij_head = NULL;
extern quad* quads;
extern unsigned currQuad;

int counternumber;
double * numConsts;
unsigned totalNumConsts = 0;
char ** stringConsts;
unsigned totalStringConsts = 0;
int libfuncsindex = 0;
char * namedLibfuncs[12] = {
    "print",
    "input",
    "objectmemberkeys",
    "objecttotalmembers",
    "objectcopy",
    "totalarguments",
    "argument",
    "typeof",
    "strtonum",
    "sqrt",
    "cos",
    "sin"};
unsigned totalNamedLibfuncs = 0;
userfunc* userFuncs;
unsigned totalUserFuncs = 0;
int counteruserfuncs = 0;
unsigned int constArrayFilled = 0;

unsigned genetation_quads;
unsigned int	currInstr = 0;
instruction* allInstructions = (instruction*) 0;


unsigned consts_newstring(char* s){
	/*
	totalStringConsts++;

	char** newstringConsts = malloc(sizeof(char*)*totalStringConsts);

	memcpy(newstringConsts,stringConsts,sizeof(char*)*(totalStringConsts-1));
	free(stringConsts);

	stringConsts = newstringConsts;
	stringConsts[totalStringConsts-1] = strdup(s);

	return totalStringConsts-1;*/


        int i = 0;
		int counterstring = 0;
        if (!stringConsts) {
            char **stringArr = (char **)malloc(sizeof(char *) * 1024);

            stringConsts = stringArr;
        }

        if (counterstring == 1024) {
            char **stringArr = (char **)malloc(sizeof(char *) * 1024 + counterstring * sizeof(char *));
            memcpy(stringArr, stringConsts, counterstring * sizeof(char *));
            free(stringConsts);
            counterstring = 0;
            stringConsts = stringArr;
        }
        counterstring++;
        totalStringConsts++;
        stringConsts[totalStringConsts - 1] = strdup(s);

        return totalStringConsts - 1;

}

unsigned int consts_newnumber(double d){
	/*totalNumConsts++;
	double * newintConsts = malloc(sizeof(double)*totalNumConsts);

	memcpy(newintConsts,numConsts,4*(totalNumConsts-1));
	free(numConsts);

	numConsts = newintConsts;

	numConsts[totalNumConsts-1] = d;
    printf("double d = %f\n", d);

	return totalNumConsts-1;*/


    int i = 0;
	if(!numConsts){
		double *numArr = (double*)malloc(sizeof(double)*1024);

		numConsts = numArr;
	}

	if(counternumber == 1024){
		double *numArr = (double*)malloc(sizeof(double)*1024 + counternumber*sizeof(double));
		memcpy(numArr,numConsts,counternumber*sizeof(double));
		free(numConsts);
		counternumber = 0;
		numConsts = numArr;
	}
	counternumber++;
	totalNumConsts++;
	numConsts[totalNumConsts-1] = d;

	return totalNumConsts-1;
}

unsigned libfuncs_newused(char * s){
	int i = 0;
	for(i = 0; i < libfuncsindex; i++){
		if(namedLibfuncs[i]){
			if(!strcmp(namedLibfuncs[i],s)){
				return i;
			}
		}
	}
	namedLibfuncs[libfuncsindex] = strdup(s);
	libfuncsindex ++;

	return libfuncsindex - 1;
}


void make_operand(expr * e, vmarg * arg){

  //printf("-- %d\n",e->type);

  switch (e->type){

    case var_e:
    case arithexpr_e:
    case tableitem_e:
    case boolexpr_e:
	  case assignexpr_e:
    case newtable_e: {

        assert(e->sym);
        arg->val = e->sym->offset;

        switch (e->sym->space)
        {
        case programvar:    arg->type = global_a; break;
        case functionlocal: arg->type = local_a; break;
        case formalarg:     arg->type = formal_a; break;

        default:
            assert(0);
        }
        break;
    }
/*

    case var_e:{
      arg->val = e->sym->offset;
      arg->type = number_a; break;
    }
 */
    case constbool_e: {
        arg->val = e->boolConst;
        arg->type = bool_a; break;
    }
    case conststring_e: {
        arg->val = consts_newstring(e->strConst);
        arg->type = string_a; break;
    }
    case constnum_e: {
        arg->val = consts_newnumber(e->numConst);
        arg->type = number_a; break;
    }
    case nil_e: arg->type = nil_a; break;

    case programfunc_e: {
        arg->type = userfunc_a;
        arg->val = e->sym->iaddress;
        arg->val = userfuncs_newfunc(e->sym);
        break;
    }
    case libraryfunc_e: {
        arg->type = libfunc_a;
        arg->val = libfuncs_newused(e->sym->value.funcVal->name);
        break;
    }
    default:
	printf("TYPE: %d\n", e->type);
	 assert(0);
    }
}

void make_numberoperand(vmarg* arg, double val){
	arg->val = consts_newnumber(val);
	arg->type = number_a;
}

void make_booloperand(vmarg * arg, unsigned val){
    arg->val = val;
    arg->type = retval_a;
}

void make_retvaloperand(vmarg * arg){
    arg->type = retval_a;
}

vmarg * reset_operand(vmarg * arg){
    arg = NULL;
}

void expand_target(){
	assert(totalInstructions == currInstr);
	instruction* i = (instruction*)malloc(T_NEW_SIZE);
	if(allInstructions){
		memcpy(i,allInstructions,T_CURR_SIZE);
		free(allInstructions);
	}
	allInstructions = i;
	totalInstructions += EXPAND_SIZE;
}

void emit_target(instruction * instr){
    struct instruction* i;
	if(currInstr == totalInstructions)
		expand_target();
	i = allInstructions + currInstr++;
	i->opcode = instr->opcode;
    if(instr->arg1){
        i->arg1 = instr->arg1;
        i->arg1->val = instr->arg1->val;
    }
    if(instr->arg2){
	    i->arg2 = instr->arg2;
        i->arg2->val = instr->arg2->val;
    }
    if(instr->result){
        i->result = instr->result;
        i->result->val = instr->result->val;
    }
	i->strLine = instr->strLine;
	totalInstructions++;
}

unsigned int  nextinstructionlabel(){
	return currInstr;
}

void generate_all(vmopcode op, quad * quad){

  instruction* t = (instruction*)malloc(sizeof(instruction));
	t->opcode = op;
	t->arg1 = (vmarg*)NULL;
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)NULL;

	int flag = 0;

	if (quad->arg1 != NULL) {
		t->arg1 = (vmarg*)malloc(sizeof(vmarg));
    if(quad->arg1->type <= 11){
		  make_operand(quad->arg1, t->arg1);
		  flag = 1;
		}
	}

	if (quad->arg2 != NULL) {
		t->arg2 = (vmarg*)malloc(sizeof(vmarg));
    if(quad->arg2->type <= 11){
		  make_operand(quad->arg2, t->arg2);
		  flag = 1;
		}
	}

	if (quad->result != NULL) {
		t->result = (vmarg*)malloc(sizeof(vmarg));
    if(quad->result->type <= 11){
		  make_operand(quad->result, t->result);
		  flag = 1;
		}
	}

	if(!flag)
		return;



	quad->taddress = nextinstructionlabel();
	t->strLine = quad->line;
	emit_target(t);
}

void generate_ADD(quad* quad){
	generate_all(add_v, quad);
}

void generate_SUB(quad* quad){
	generate_all(sub_v, quad);
}

void generate_MUL(quad* quad){
	generate_all(mul_v, quad);
}

void generate_DIV(quad* quad){
	generate_all(div_v, quad);
}

void generate_MOD(quad* quad){
	generate_all(mod_v, quad);
}

void generate_UMINUS(quad* quad){

  instruction* i = (instruction*)malloc(sizeof(instruction));

  i->arg1 = (vmarg*)malloc(sizeof(vmarg));
	i->arg2 = (vmarg*)malloc(sizeof(vmarg));
	i->result = (vmarg*)malloc(sizeof(vmarg));
	i->opcode = mul_v;

  make_operand(quad->arg2, i->arg1);
	i->arg2->type = number_a;
	i->arg2->val = -1;
	make_operand(quad->result,i->result);

	quad->taddress = nextinstructionlabel();
	i->strLine = quad->line;
	emit_target(i);
}

void generate_NEWTABLE(quad* quad){
	generate_all(newtable_v, quad);
}

void generate_TABLEGETELEM(quad* quad){
	generate_all(tablegetelem_v, quad);
}

void generate_TABLESETELEM(quad* quad){
	generate_all(tablesetelem_v, quad);
}

void generate_ASSIGN(quad* quad){
	generate_all(assign_v, quad);
}

void generate_NOP(quad * quad){
	instruction *t = (instruction*)malloc(sizeof(instruction));
	t->opcode = nop_v;
	t->strLine = quad->line;
	t->arg1 = (vmarg*)NULL;
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)NULL;
	emit_target(t);
}

void add_incomplete_jump(unsigned instrNo, unsigned iaddress){
	incomplete_jump *temp = NULL;
	temp= malloc(sizeof(incomplete_jump));
	temp->instrNo=instrNo;
	temp->iaddress = iaddress;
	if (ij_head== NULL){
        temp->next=NULL;
	}
	else{
		temp->next=ij_head;
	}
    ij_head=temp;
}

void generate_relational(vmopcode op, quad* quad){
	instruction* t = (instruction*)malloc(sizeof(instruction));
	t->opcode = op;
    t->strLine = quad->line;

    t->arg1 = (vmarg*)NULL;
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)malloc(sizeof(vmarg));
	int flag = 0;
	if(quad->arg1){
        t->arg1 = (vmarg*)malloc(sizeof(vmarg));
        if(quad->arg1->type <= 11){
			make_operand(quad->arg1,t->arg1);
			flag = 1;
		}

    }
	if(quad->arg2){
        t->arg2 = (vmarg*)malloc(sizeof(vmarg));
        if(quad->arg2->type <=11){
			make_operand(quad->arg2,t->arg2);
			flag = 1;
		}

    }

	if(!flag && op != jump_v )
		return;

	t->result->type = label_a;

	if(quad->label < genetation_quads){
		t->result->val = quads[quad->label].taddress-1;
	}
	else{
		add_incomplete_jump(nextinstructionlabel(),quad->label);
	}

	quad->taddress = nextinstructionlabel();
	t->strLine = quad->line;
	emit_target(t);

}

void generate_JUMP(quad* quad){
	generate_relational(jump_v,quad);
}

void generate_IF_EQ(quad* quad){
	generate_relational(jeq_v, quad);
}

void generate_IF_NOTEQ(quad* quad){
	generate_relational(jne_v, quad);
}

void generate_IF_GREATER(quad* quad){
	generate_relational(jgt_v, quad);
}

void generate_IF_GREATERQ(quad* quad){
	generate_relational(jge_v, quad);
}

void generate_IF_LESS(quad* quad){
	generate_relational(jlt_v, quad);
}

void generate_IF_LESSEQ(quad* quad){
	generate_relational(jle_v, quad);
}

void generate_PARAM(quad* quad){
	quad->taddress = nextinstructionlabel();
    instruction* t = (instruction*)malloc(sizeof(instruction));

	t->arg1 = (vmarg*)NULL;
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)malloc(sizeof(vmarg));

	t->opcode = pusharg_v;
    t->strLine = quad->line;
    if(quad->result->type <= 11){

	    make_operand(quad->result, t->result);
      //printf("---- %f\n",t->result->val);
    }else
		  return;


    emit_target(t);
}

void generate_NOT(quad * quad){
    quad->taddress = nextinstructionlabel();

	instruction* t1 = (instruction*)malloc(sizeof(instruction));

	t1->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t1->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t1->result = (vmarg*)malloc(sizeof(vmarg));

	t1->opcode = jeq_v;
	make_operand(quad->arg1, t1->arg1);
	make_booloperand(t1->arg2, 0);	/* 0 = FALSE */
	t1->result->type = label_a;
	t1->result->val = nextinstructionlabel() + 3;
	t1->strLine = quad->line;
	emit_target(t1);

    instruction* t2 = (instruction*)malloc(sizeof(instruction));
	t2->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t2->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t2->result = (vmarg*)malloc(sizeof(vmarg));

	t2->opcode = assign_v;
	make_booloperand(t2->arg1, 0);	/* 0 = FALSE */
	reset_operand(t2->arg2);
	make_operand(quad->result, t2->result);
	t2->strLine = quad->line;
	emit_target(t2);

	instruction* t3 = (instruction*)malloc(sizeof(instruction));

	t3->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t3->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t3->result = (vmarg*)malloc(sizeof(vmarg));

	t3->opcode = jump_v;
	reset_operand(t3->arg1);
	reset_operand(t3->arg2);
	t3->result->type = label_a;
	t3->result->val = nextinstructionlabel() + 2;
	t3->strLine = quad->line;
	emit_target(t3);

	instruction* t4 = (instruction*)malloc(sizeof(instruction));

	t4->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t4->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t4->result = (vmarg*)malloc(sizeof(vmarg));

	t4->opcode = assign_v;
	make_booloperand(t4->arg1, 1);	/* 1 = TRUE */
	reset_operand(t4->arg2);
	make_operand(quad->result, t4->result);
	t4->strLine = quad->line;
	emit_target(t4);
}

void generate_OR(quad * q){
	q->taddress = nextinstructionlabel();

	instruction* t1 = (instruction*)malloc(sizeof(instruction));

	t1->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t1->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t1->result = (vmarg*)malloc(sizeof(vmarg));

	t1->opcode = jeq_v;
	make_operand(q->arg1, t1->arg1);
	make_booloperand(t1->arg2, 1);	/* 1 = TRUE */
	t1->result->type = label_a;
	t1->result->val = nextinstructionlabel() + 4;
	t1->strLine = q->line;
	emit_target(t1);

	instruction* t2 = (instruction*)malloc(sizeof(instruction));

	t2->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t2->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t2->result = (vmarg*)malloc(sizeof(vmarg));
	t2->opcode = jeq_v;
	make_operand(q->arg2, t2->arg1);
	make_booloperand(t2->arg2, 1);	/* 1 = TRUE */
	t2->result->val = nextinstructionlabel() + 3;
	t2->strLine = q->line;
	emit_target(t2);

	instruction* t3 = (instruction*)malloc(sizeof(instruction));

	t3->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t3->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t3->result = (vmarg*)malloc(sizeof(vmarg));
	t3->opcode = assign_v;
	make_booloperand(t3->arg1, 0);	/* 0 = FALSE */
	reset_operand(t3->arg2);
	make_operand(q->result, t3->result);
	t3->strLine = q->line;
	emit_target(t3);

	instruction* t4 = (instruction*)malloc(sizeof(instruction));

	t4->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t4->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t4->result = (vmarg*)malloc(sizeof(vmarg));
	t4->opcode = jump_v;
	reset_operand(t4->arg1);
	reset_operand(t4->arg2);
	t4->result->type = label_a;
	t4->result->val = nextinstructionlabel() + 2;
	t4->strLine = q->line;
	emit_target(t4);

	instruction* t5 = (instruction*)malloc(sizeof(instruction));

	t5->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t5->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t5->result = (vmarg*)malloc(sizeof(vmarg));
	t5->opcode = assign_v;
	make_booloperand(t5->arg1, 1);	/* 1 = TRUE */
	reset_operand(t5->arg2);
	make_operand(q->result, t5->result);
	t5->strLine = q->line;
	emit_target(t5);
}

void generate_AND(quad * q){
	q->taddress = nextinstructionlabel();

	instruction* t1 = (instruction*)malloc(sizeof(instruction));

	t1->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t1->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t1->result = (vmarg*)malloc(sizeof(vmarg));

	t1->opcode = jeq_v;
	make_operand(q->arg1, t1->arg1);
	make_booloperand(t1->arg2, 0);	/* 0 = FALSE */
	t1->result->type = label_a;
	t1->result->val = nextinstructionlabel() + 4;
	t1->strLine = q->line;
	emit_target(t1);

	instruction* t2 = (instruction*)malloc(sizeof(instruction));

	t2->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t2->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t2->result = (vmarg*)malloc(sizeof(vmarg));
	t2->opcode = jeq_v;
	make_operand(q->arg2, t2->arg1);
	make_booloperand(t2->arg2, 0);	/* 0 = FALSE */
	t2->result->val = nextinstructionlabel() + 3;
	t2->strLine = q->line;
	emit_target(t2);

	instruction* t3 = (instruction*)malloc(sizeof(instruction));

	t3->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t3->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t3->result = (vmarg*)malloc(sizeof(vmarg));
	t3->opcode = assign_v;
	make_booloperand(t3->arg1, 1);	/* 1 = TRUE */
	reset_operand(t3->arg2);
	make_operand(q->result, t3->result);
	t3->strLine = q->line;
	emit_target(t3);

	instruction* t4 = (instruction*)malloc(sizeof(instruction));

	t4->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t4->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t4->result = (vmarg*)malloc(sizeof(vmarg));
	t4->opcode = jump_v;
	reset_operand(t4->arg1);
	reset_operand(t4->arg2);
	t4->result->type = label_a;
	t4->result->val = nextinstructionlabel() + 2;
	t4->strLine = q->line;
	emit_target(t4);

	instruction* t5 = (instruction*)malloc(sizeof(instruction));

	t5->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t5->arg2 = (vmarg*)malloc(sizeof(vmarg));
	t5->result = (vmarg*)malloc(sizeof(vmarg));
	t5->opcode = assign_v;
	make_booloperand(t5->arg1, 0);	/* 0 = FALSE */
	reset_operand(t5->arg2);
	make_operand(q->result, t5->result);
	t5->strLine = q->line;
	emit_target(t5);
}

void generate_CALL(quad* quad){

    if (quad->result->type > 11)
		return;

    quad->taddress = nextinstructionlabel();


	instruction * t= malloc(sizeof(instruction));;
	t->arg1 = (vmarg*)NULL;
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)malloc(sizeof(vmarg));

	t->opcode = call_v;
	t->strLine = quad->line;
    if(quad->result->type <= 11)
	    make_operand(quad->result, t->result);

	emit_target(t);
}

void generate_GETRETVAL(quad* quad){
	quad->taddress = nextinstructionlabel();
	instruction* t = (instruction*)malloc(sizeof(instruction));

    t->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)malloc(sizeof(vmarg));

	t->opcode = assign_v;
	make_operand(quad->result, t->result);
	make_retvaloperand(t->arg1);
	emit_target(t);
}

void push_func(func_stack* function){
	function->next = f_top;
	f_top = function;
}

func_stack* pop_func(){
	func_stack* tmp;

	if(f_top == NULL)
		return NULL;

	tmp = f_top;
	f_top = f_top->next;
	return tmp;

}

func_stack* top_func(){
	func_stack* tmp;
	if(f_top == NULL)
		return NULL;
	tmp = f_top;
	return tmp;
}

int addUserFunc(SymbolTableEntry * f){
    totalUserFuncs++;

    userfunc* userFuncsPlusOne = malloc(sizeof(userfunc)*totalUserFuncs);
    memcpy(userFuncsPlusOne, userFuncs, sizeof(userfunc)*(totalUserFuncs -1));

    free(userFuncs);
    userFuncs = userFuncsPlusOne;
    userFuncs[totalUserFuncs-1].id = strdup(f->value.funcVal->name);
    userFuncs[totalUserFuncs-1].address=f->iaddress;
	userFuncs[totalUserFuncs-1].localSize=f->totalLocals;

	return totalUserFuncs-1;
}

unsigned userfuncs_newfunc(SymbolTableEntry * sym){

	int i = 0;
	if(!userFuncs){
		userfunc *user = (userfunc*)malloc(sizeof(userfunc)*1024);

		userFuncs = user;
	}
	if(counteruserfuncs == 1024){
		userfunc *user = (userfunc*)malloc(sizeof(userfunc)*1024 + counteruserfuncs*sizeof(userfunc));
		memcpy(user,userFuncs,counteruserfuncs*sizeof(userfunc));
		free(userFuncs);
		counteruserfuncs = 0;
		userFuncs = user;


	}
	counteruserfuncs++;
	for(i = 0; i < totalUserFuncs; i++){
		if(!strcmp(userFuncs[i].id,sym->value.funcVal->name)){
			return i;
		}
	}
	userfunc *funcs = userFuncs + totalUserFuncs;
	totalUserFuncs++;

	funcs->id = strdup(sym->value.funcVal->name);
	funcs->address = sym->iaddress;
	funcs->localSize = sym->totalLocals;

	return totalUserFuncs-1;
}

void generate_FUNCSTART(quad* quad){
    SymbolTableEntry * f = (SymbolTableEntry*)malloc(sizeof(SymbolTableEntry));
	int value;

	//if(quad->result->sym->value.funcVal->scope == 0){
		instruction* t = (instruction*)malloc(sizeof(instruction));
		t->arg1 = (vmarg*)NULL;
        t->arg2 = (vmarg*)NULL;
        t->result = (vmarg*)malloc(sizeof(instruction));

        t->opcode = jump_v;
		t->result->type = label_a;
		t->result->val = 0;
		t->strLine = quad->line;
        reset_operand(t->arg1);
        reset_operand(t->arg2);
        t->strLine = quad->line;
		emit_target(t);
		//func_jmp = nextinstructionlabel()-1;
	//}

    func_stack * function = malloc(sizeof(func_stack));

	f = quad->result->sym;
    function->func = f;
    function->returnNode = NULL;
	f->iaddress = nextinstructionlabel();

	quad->taddress = nextinstructionlabel();
	value = userfuncs_newfunc(f);

    push_func(function);

	instruction* t1 = (instruction*)malloc(sizeof(instruction));
	t1->result = (vmarg*)malloc(sizeof(vmarg));
	t1->opcode = funcenter_v;
	make_operand(quad -> result,t1->result);
	reset_operand(t1->arg1);
	reset_operand(t1->arg2);
	t1->strLine = quad->line;
    t1->result->val = value;

	emit_target(t1);
}

return_list * append(return_list * head, int i){
    return_list *returnListNode = malloc(sizeof(struct return_List));
	returnListNode->label = i;

	if( head == NULL ){
		returnListNode->next = NULL;
	}
	else{
        /*prosthiki stin arxi ti listas*/
		returnListNode->next = head;
	}
	head = returnListNode;
    return head;
}

void generate_RETURN(quad* quad){

	if(quad->result != NULL){
        if (quad->result->type > 11)
            return;
    }

    quad->taddress = nextinstructionlabel();
    instruction* t = (instruction*)malloc(sizeof(instruction));
	t->result = (vmarg*)malloc(sizeof(vmarg));
	t->arg1 = (vmarg*)malloc(sizeof(vmarg));
	t->arg2 = (vmarg*)NULL;
	t->opcode = assign_v;
	make_retvaloperand(t->result);

	if(quad->result != NULL){
        if (quad->result->type <= 11) {
            make_operand(quad->result, t->arg1);
        }
    }




	emit_target(t);

    instruction* t2 = (instruction*)malloc(sizeof(instruction));
	func_stack *f;
    f=pop_func();
    f->returnNode = append(f->returnNode, nextinstructionlabel());
    push_func(f);
    t2->result = (vmarg*)malloc(sizeof(vmarg));
	t2->opcode=jump_v;
	reset_operand(t2->arg1);
	reset_operand(t2->arg2);
	t2->result->type=label_a;
	emit_target(t2);
}

void generate_FUNCEND(quad* quad){
    func_stack *f;
    struct return_List * returnNode;
    f = pop_func();
    returnNode = f->returnNode;

    while(returnNode != NULL){
        allInstructions[returnNode->label].result->val = nextinstructionlabel();
        returnNode = returnNode->next;
    }

    quad->taddress = nextinstructionlabel();
    instruction* t = (instruction*)malloc(sizeof(instruction));
	t->arg1 = (vmarg*)NULL;
	t->arg2 = (vmarg*)NULL;
	t->result = (vmarg*)malloc(sizeof(vmarg));
    t->opcode = funcexit_v;
    make_operand(quad->result, t->result);
    t->result->val = totalUserFuncs-1;
    emit_target(t);
}
/*
  assign,         add,            sub,
  mul,            divide,         mod,
  uminus,         and,            or,
  not,            if_eq,          if_noteq,
  if_lesseq,      if_greatereq,   if_less,
  if_greater,     call,           param,
  ret,            getretval,      funcstart,
  funcend,        tablecreate,    tablegetelem,
  tablesetelem,   jump
  */
generator_func_t generators[] = {
    generate_ASSIGN,
    generate_ADD,
    generate_SUB,
    generate_MUL,
    generate_DIV,
    generate_MOD,
    generate_UMINUS,
    generate_AND,
    generate_OR,
    generate_NOT,
    generate_IF_EQ,
    generate_IF_NOTEQ,
    generate_IF_LESSEQ,
    generate_IF_GREATERQ,
    generate_IF_LESS,
    generate_IF_GREATER,
    generate_CALL,
    generate_PARAM,
    generate_RETURN,
    generate_GETRETVAL,
    generate_FUNCSTART,
    generate_FUNCEND,
    generate_NEWTABLE,
    generate_TABLEGETELEM,
    generate_TABLESETELEM,
    generate_JUMP,
    generate_NOP
};

void patch_incomplete_jumps(){
    incomplete_jump * tmp = ij_head;

    while(tmp != NULL){
        if(tmp->iaddress == currQuad){
            allInstructions[tmp->instrNo].result->val = currInstr - 1;
        }
        else {
            allInstructions[tmp->instrNo].result->val =  quads[tmp->iaddress].taddress;
        }
		tmp = tmp->next;
    }
}

void generate(void){

	for (genetation_quads = 0; genetation_quads < currQuad; ++genetation_quads){

    //printf("Generating %d ",quads[genetation_quads].op);

    (*generators[quads[genetation_quads].op]) (quads + genetation_quads);
    //printf("        Done!\n");


    /*
      printf("a: %d \n",tmp->result->numConst );
      printf("b: %d \n",(quads + genetation_quads)->result->numConst);


    if(quads[0].arg1 == NULL){
      printf("AAAA --%d\n", tmp);
      printf("quads = %d -------- quads[0] = %d\n",quads,quads[0] );
    }
    */
	}
    patch_incomplete_jumps();
}


void printConsts(int globalsSum){
	int i = 0;
    /*
	printf("\ntotal_namedLibfuncs : %d\n", 12);
	printf("totalNumConsts : %d\n", totalNumConsts);
	printf("totalStringConsts : %d\n", totalStringConsts);
	printf("totalUserFuncs : %d\n", totalUserFuncs);


	for(i=0; i<totalNumConsts; i++){
		printf("NUMBER %d  %f\n", i, numConsts[i]);
	}

	for(i=0; i<totalStringConsts ; i++){
		printf("STRING %d  %s\n", i, stringConsts[i]);
	}

    for(i=0; i<totalUserFuncs; i++){
        printf("\n%d: %s %d %d ", i, userFuncs[i].id, userFuncs[i].address, userFuncs[i].localSize);
    }
	printf("\ninstructions:\n");

    vmopcode op;
	i = 0;
	char *table_op[25] = {"ASSIGN","ADD","SUB","MUL","DIV","MOD","UMINUS","AND","OR","NOT","JEQ","JNOTEQ",
      "JLESSEQ","JGREATEREQ","JLESS","JGREATER","CALL","PUSHARG","ENTERFUNC","EXITFUNC","NEWTABLE","TABLEGETELEM","TABLESETELEM","JUMP","NOP"};
    char *vmarg_t[12]={"label","global","formal","local","integer","double","string","bool","nil","userfunc","libfunc","retval"};

    for(i=0; i<currInstr; i++){

    	op = (allInstructions[i]).opcode;
		printf("CURRENT i = %d\n", i);
    	printf("%d: %s\t",i,table_op[op]);
    	if((op==add_v) || (op==and_v) || (op==or_v) || (op==sub_v) || (op==mul_v) || (op==div_v) || (op==mod_v) || (op==tablegetelem_v) || (op==tablesetelem_v)){
			printf("%d(%s),%d(value)\t", allInstructions[i].arg1.type,vmarg_t[allInstructions[i].arg1.type],allInstructions[i].arg1.val);
			printf("%d(%s),%d(value)\t", allInstructions[i].arg2.type,vmarg_t[allInstructions[i].arg2.type],allInstructions[i].arg2.val);
			printf("%d(%s),%d(value)\t", allInstructions[i].result.type,vmarg_t[allInstructions[i].result.type],allInstructions[i].result.val);
      	}else if((op==jgt_v) || (op==jump_v) || (op==jge_v)	|| (op==jlt_v) || (op==jle_v) || (op==jne_v) || (op==jeq_v)){
			printf("%d(%s),%d(value)\t", allInstructions[i].arg1.type,vmarg_t[allInstructions[i].arg1.type],allInstructions[i].arg1.val);
            if (allInstructions[i].arg2.type <= 11)
                printf("%d(%s),%d(value)\t", allInstructions[i].arg2.type, vmarg_t[allInstructions[i].arg2.type], allInstructions[i].arg2.val);
			printf("%d(%s),%d(value)\t", allInstructions[i].result.type,vmarg_t[allInstructions[i].result.type],allInstructions[i].result.val);
     	}else if((op==not_v) || (op==uminus_v) || (op==assign_v)){
     		if(allInstructions[i].arg1.type != 11)
				printf("%d(%s),%d(value)\t", allInstructions[i].arg1.type,vmarg_t[allInstructions[i].arg1.type],allInstructions[i].arg1.val);
			else
				printf("%d(%s)\t", allInstructions[i].arg1.type,vmarg_t[allInstructions[i].arg1.type]);
			if(allInstructions[i].result.type != 11)
				printf("%d(%s),%d(value)\t", allInstructions[i].result.type,vmarg_t[allInstructions[i].result.type],allInstructions[i].result.val);
			else
				printf("%d(%s)\t", allInstructions[i].result.type,vmarg_t[allInstructions[i].result.type]);

      	}else if((op==call_v) || (op==pusharg_v) || (op==funcenter_v) ||  (op==funcexit_v) || (op==newtable_v)){
			printf("%d(%s),%d(value)\t", allInstructions[i].result.type,vmarg_t[allInstructions[i].result.type],allInstructions[i].result.val);
      	}else{
	 		if(allInstructions[i].result.type==0 )
	    		printf("%d(%s),%d(value)\t", allInstructions[i].result.type,vmarg_t[allInstructions[i].result.type],allInstructions[i].result.val);
    	}
    		printf("\n");

	}*/


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    printf("-----------------------------------------------------------------------------------------------------------------------------\n\n");
	char *magicNo = "340200501";
    FILE* icode = fopen("code.bin","wb");
	if(!icode){
		fprintf(stdout, "Cannot open binary file\n");
		return;
	}
	printf("Creating binary file...\n");
	i = 0;
	int totals[] = {totalNumConsts, totalStringConsts, totalUserFuncs, currInstr};
	//printf("Total instr = %d, curr = %d\n", totalInstructions, currInstr);

	fwrite(magicNo, strlen(magicNo)+1,1,icode);
    fwrite(&globalsSum,sizeof(unsigned int),1,icode);
    /*metafora megethos pinakon*/
    fwrite(totals,sizeof(int), 4, icode);
	/*metafora pinakon*/
    fwrite(numConsts, sizeof(double),totalNumConsts,icode);

	for(int i = 0; i < totalStringConsts; i++) {
        int sizeOfString = strlen(stringConsts[i]) + 1;
        fwrite(&sizeOfString, sizeof(int), 1, icode);
        fwrite(stringConsts[i], sizeof(char), sizeOfString, icode);
        //printf("length = %d, string=%s\n", sizeOfString, stringConsts[i]);
    }
	
	fwrite(userFuncs,sizeof(userfunc),totalUserFuncs,icode);

    struct vmarg placeholder;

    placeholder.type = 7;
    placeholder.val = 696969;

    for(int i = 0; i < currInstr; i++){

        fwrite(&allInstructions[i].opcode, sizeof(vmopcode), 1, icode);

        if (allInstructions[i].result != NULL){
            fwrite(&(*allInstructions[i].result), sizeof(vmarg), 1, icode);
		}
		else{
			fwrite(&placeholder, sizeof(vmarg), 1, icode);

        }
           

        if (allInstructions[i].arg1 != NULL)
            fwrite(&(*allInstructions[i].arg1), sizeof(vmarg), 1, icode);
        else{
            fwrite(&placeholder, sizeof(vmarg), 1, icode);

        }
            

        if (allInstructions[i].arg2 != NULL)
            fwrite(&(*allInstructions[i].arg2), sizeof(vmarg), 1, icode);
        else{
			fwrite(&placeholder, sizeof(vmarg), 1, icode);

        }
            

        fwrite(&allInstructions[i].strLine, sizeof(unsigned), 1, icode);
    }
    

    fclose(icode);

	printf("Binary file was created successfully!\n");
}
