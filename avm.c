
#include "avm.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

unsigned int top, topsp;
avm_memcell retval;

unsigned char executionFinished = 0;
unsigned pc = 0;
unsigned currLine = 0;
unsigned codeSize = 0;

instruction * code = (instruction*)0;
instruction * instr_save;
#define AVM_ENDING_PC total_instr


unsigned totalActuals = 0;

unsigned total_instr;
double*	Consts_Numbers;
unsigned total_Numbers_Consts;
char** 	Consts_String;
unsigned total_String_Consts;
userfunc* Consts_Func;
unsigned total_Func_Consts;

unsigned countlibFuncs = 0;
unsigned totalLibs = 0;
int globalsSum;

avm_memcell 	ax, bx;

extern instruction* allInstructions;

static void avm_initstack(void){
    unsigned i;
    for(i = 0; i< AVM_STACKSIZE; ++i){
        AVM_WIPEOUT(stack[i]);
        stack[i].type = undef_m;
    }
}

void avm_tableincrefcounter(avm_table * t){
    ++t->refCounter;
}

void avm_tabledecrefcounter(avm_table * t){
    assert(t->refCounter > 0);
    if(!--t->refCounter)
        avm_tabledestroy(t);
}

void avm_tablebucketinit(avm_table_bucket ** p){
    unsigned i;
    for(i = 0; i < AVM_TABLE_HASHSIZE; ++i){
        p[i] = (avm_table_bucket*) 0;
    }
}

avm_table * avm_tablenew(void){
    avm_table * t;
    t = (avm_table*)malloc(sizeof(avm_table));
    AVM_WIPEOUT(*t);

    t->refCounter = t->total = 0;
    avm_tablebucketinit(t->numIndexed);
    avm_tablebucketinit(t->strIndexed);

    return t;
}

void avm_memcellclear(avm_memcell * m){
    m->type = undef_m;
}

void avm_tablebucketdestroy(avm_table_bucket** p){
    unsigned i;
    avm_table_bucket * b;
    avm_table_bucket * b_for_clear;

    for(i=0; i<AVM_TABLE_HASHSIZE; ++i){
        for(b = *p; b;){
            b_for_clear = b;
            b = b->next;
            free(b_for_clear);
        }
        p[i] = (avm_table_bucket*) 0;
    }
}

void avm_tabledestroy(avm_table * t){
    avm_tablebucketdestroy(t->strIndexed);
    avm_tablebucketdestroy(t->numIndexed);
    free(t);
}

char * libfuncs_getused(unsigned i){
    char * name;

    switch (i)
    {
        case 0: {
            name = "print";
            return name;
        }
        case 1: {
            name = "input";
            return name;
        }
        case 2: {
            name = "objectmemberkeys";
            return name;
        }
        case 3: {
            name = "objecttotalmembers";
            return name;
        }
        case 4: {
            name = "objectcopy";
            return name;
        }
        case 5: {
            name = "totalarguments";
            return name;
        }
        case 6: {
            name = "argument";
            return name;
        }
        case 7: {
            name = "typeof";
            return name;
        }
        case 8: {
            name = "strtonum";
            return name;
        }
        case 9: {
            name = "sqrt";
            return name;
        }
        case 10: {
            name = "cos";
            return name;
        }
        case 11: {
            name = "sin";
            return name;
        }
        default: {
            printf("wrong number for lib function\n");
            return NULL;
        }
    }
}

double consts_getnumber(unsigned i){
    return Consts_Numbers[i];
}

char* consts_getstring(unsigned i){
	return Consts_String[i];
}

avm_memcell* avm_translate_operand(vmarg* arg, avm_memcell * reg){
	//printf("type = %d\n", arg->type);


	switch (arg->type)
    {
        case global_a: return &stack[AVM_STACKSIZE-1-arg->val];
        case local_a:  return &stack[topsp-arg->val];
        case formal_a: return &stack[topsp+AVM_STACKENV_SIZE + 1 + arg->val];
        case retval_a: return &retval;
        case number_a: {

            if(reg == NULL) return NULL;

            reg->type = number_m;

            if(arg->val == -1){      // uminus
              reg->data.numVal = -1;
            }else{
              reg->data.numVal = consts_getnumber(arg->val);
            }

            //printf("arg inside translate = %f\n",arg->val);

            return reg;
        }
        case bool_a: {
            if (reg == NULL)
                return NULL;
            reg->type = bool_m;
            reg->data.boolVal = arg->val;
            return reg;
        }
        case nil_a:
            if (reg == NULL)
                return NULL;
            reg->type = nil_m;
            return reg;
        case userfunc_a: {
            if (reg == NULL)
                return NULL;
            reg->type = userfunc_m;
            reg->data.funcVal = Consts_Func[arg->val].address;
            return reg;
        }
        case string_a: 	{
            if (reg == NULL)
                return NULL;
            reg->type = string_m;
			reg->data.strVal = strdup(consts_getstring(arg->val));
			return reg;
		}
        case libfunc_a: {
            if (reg == NULL)
                return NULL;
            reg->type = libfunc_m;
			//printf("edo logika prepei na bei\n");
            reg->data.libfuncVal = libfuncs_getused(arg->val);
			return reg;
        }
        default: assert(0);
    }
}

void avm_error(char *msg, char * smth){
    executionFinished = 1;
    if(smth != NULL){
        printf("\n%s ", msg);
        printf("%s\n",smth);
    }
    else{
        printf("\n%s ", msg);
    }
}

void avm_dec_top(void){
	if(!top){
		avm_error("stack overflowed!", NULL);
	}
	else
		--top;
}

void avm_push_envvalue(unsigned i){
	stack[top].type = number_m;
	stack[top].data.numVal 	= i;
	avm_dec_top();
}

void avm_callsaveenvironment(void){
    avm_push_envvalue(totalActuals);
	avm_push_envvalue(pc + 1);
	avm_push_envvalue(top + totalActuals + 2);
	avm_push_envvalue(topsp);
}

library_func_t libFuncs[] = {
    0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0
};

library_func_t avm_getlibraryfunc(char * id){
    if(!strcmp(id, "print")){
        return libFuncs[0];
    }
    else if(!strcmp(id, "input")){
        return libFuncs[1];
    }
    else if(!strcmp(id, "objectmemberkeys")){
        return libFuncs[2];
    }
    else if(!strcmp(id, "objecttotalmembers")){
        return libFuncs[3];
    }
    else if(!strcmp(id, "objectcopy")){
        return libFuncs[4];
    }
    else if(!strcmp(id, "totalarguments")){
        return libFuncs[5];
    }
    else if(!strcmp(id, "argument")){
        return libFuncs[6];
    }
    else if(!strcmp(id, "typeof")){
        return libFuncs[7];
    }
    else if(!strcmp(id, "strtonum")){
        return libFuncs[8];
    }
    else if(!strcmp(id, "sqrt")){
        return libFuncs[9];
    }
    else if(!strcmp(id, "cos")){
        return libFuncs[10];
    }
    else if(!strcmp(id, "sin")){
        return libFuncs[11];
    }
}

void avm_arrayOfLibFuncsInit(library_func_t func){
    libFuncs[countlibFuncs] = func;
    countlibFuncs++;
}

void avm_warning(char* msg){
	printf("\n%s \n ",msg);
}

unsigned avm_get_envvalue(unsigned i){
	unsigned val;
	assert(stack[i].type == number_m);
	val = (unsigned) stack[i].data.numVal;
	assert(stack[i].data.numVal == ((double) val));
	return val;
}

void execute_funcexit(instruction* unused){
	unsigned int oldTop = top;
	top = avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
	pc = avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
	topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
	while (oldTop++ <= top){
		avm_memcellclear(&stack[oldTop]);
	}
}

void avm_calllibfunc(char * id){
    library_func_t f = avm_getlibraryfunc(id);

    if(!f){
        avm_error("Error: unsupported lib func called:\n", id);
    }
    else{
        topsp = top;
        totalActuals = 0;
        (*f)();
        if(!executionFinished){
            execute_funcexit((instruction*) 0);
        }
    }
}

void execute_call(instruction* instr){

    if (instr->result == NULL)
        return;

  avm_memcell* func = avm_translate_operand(instr->result, &ax);
	assert(func);
	avm_callsaveenvironment();

	//printf("\n!the type is: %d!\n", func->type);


	switch (func->type){

		case userfunc_m: {
			pc = func->data.funcVal;
			assert(pc < AVM_ENDING_PC);
			assert(instr[pc].opcode = funcenter_v);
			break;
		}

		case string_m: {
			//printf("\n!the library functions is: %s!\n", func->data.strVal);
			avm_calllibfunc(func->data.strVal);
			break;
		}

		case libfunc_m: {
			//printf("\n!the library functions is: %s!\n", func->data.strVal);
			avm_calllibfunc(func->data.strVal);
			break;
		}

		default: {
			avm_error("Error: error in call!\n", NULL);
		}
	}
}

void avm_assign(avm_memcell* lv, avm_memcell* rv){

	if(lv == rv)			/* Same cells? Destructive to assign*/
		return;

	if((lv->type == table_m) && (rv->type == table_m) && 			/* Same tables? No need to assign*/
		(lv->data.tableVal == rv->data.tableVal)){
		return;
	}


	avm_memcellclear(lv);			/* Clear old cell contents */
	memcpy(lv, rv, sizeof(avm_memcell));

	/* Now take care of copied valus or reference counting */
	if(lv->type == string_m){

		lv->data.strVal = strdup(rv->data.strVal);
	}else if(lv->type == table_m){
		avm_tableincrefcounter(lv->data.tableVal);
	}

}

void execute_assign (instruction* instr){

    if (instr->result == NULL || instr->arg2 == NULL){
      return;
    }
  avm_memcell *lv = avm_translate_operand(instr->result, (avm_memcell *)0);
	avm_memcell* rv = avm_translate_operand(instr->arg2, &ax);



	assert(lv && (&stack[AVM_STACKSIZE - 1] >= lv && lv > &stack[top] || lv == &retval));
	assert(rv);

	avm_assign(lv, rv);
}

void execute_pusharg(instruction* instr){
  if (instr->result == NULL){
    return;
  }

  avm_memcell* arg = avm_translate_operand(instr->result, &ax);

	assert(arg);
	avm_assign(&stack[top], arg);
	++totalActuals;
	avm_dec_top();
}

userfunc* avm_getfuncinfo(unsigned address){
	return (Consts_Func + address);
}

void execute_funcenter(instruction* instr){
	userfunc* funcInfo;

    if (instr->result == NULL)
        return;

    avm_memcell* func = avm_translate_operand(instr->result, &ax);
	assert(func);
	printf("#func->data.funcVal = %d\n", func->data.funcVal);
	assert(pc == func->data.funcVal);
	totalActuals = 0;
	funcInfo = avm_getfuncinfo(pc-1);
	topsp = top;
	top = top - funcInfo->localSize;
}

int avm_totalactuals(void){
    return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell* avm_getactual(unsigned i){
    assert(i < avm_totalactuals());
    return &stack[topsp + AVM_STACKENV_SIZE + 1 + i];
}

char* number_tostring(avm_memcell* m){
	char* s = (char*)malloc(512*sizeof(char));
	sprintf(s, "%f", m->data.numVal);
	return s;
}

char* string_tostring(avm_memcell* m){
	char* s;
	s = strdup(m->data.strVal);
	return s;
}

char* bool_tostring(avm_memcell* m){
	if(m->data.boolVal == 1)
		return "true";
	else
		return "false";
}

char* table_tostring(avm_memcell* m){

}

char* userfunc_tostring(avm_memcell* m){
	char* s = (char*) malloc(512*sizeof(char));
	sprintf(s, "%d", m->data.funcVal);
	return s;
}

char* libfunc_tostring(avm_memcell* m){
	char* s;
	s = strdup(m->data.libfuncVal);
	return s;
}

char* nil_tostring(avm_memcell* m){
	return "nil";
}

char* undef_tostring(avm_memcell* m){
	return "undef";
}

tostring_func_t tostringFuncs[] = {
	number_tostring,
	string_tostring,
	bool_tostring,
	table_tostring,
	userfunc_tostring,
	libfunc_tostring,
	nil_tostring,
	undef_tostring
};

char* avm_tostring(avm_memcell* m){
	assert(m->type >= 0 && m->type <= undef_m);
	return (*tostringFuncs[m->type])(m);
}

void libfunc_print(void){
	unsigned n = avm_totalactuals();
	unsigned i;
	char* s;
	for(i = 0; i < n; ++i){
		s = (char*)avm_tostring(avm_getactual(i));
		puts(s);
	}
}

char* typeStrings[] = {
	"number",
	"string",
	"bool",
	"table",
	"userfunc",
	"libfunc",
	"nil",
	"undef"
};

void libfunc_typeof(void){
    unsigned n = avm_totalactuals();

    char str[10];
    sprintf(str, "%d", n);

    if(n!= 1){
        avm_error("Error: one argument expected in 'typeof' not: ", str);
    }
    else {
        avm_memcellclear(&retval);
        retval.type = string_m;
        retval.data.strVal = strdup(typeStrings[avm_getactual(0)->type]);
    }
}

void libfunc_totalarguments(void){
	unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
	avm_memcellclear(&retval);
	if(!p_topsp){
		avm_error("Error: 'totalarguments' called outside a function!", NULL);
		retval.type = nil_m;
	}
	else{
		retval.type = number_m;
		retval.data.numVal = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
	}
}

double add_impl(double x, double y) {return x+y;}
double sub_impl(double x, double y) {return x-y;}
double mul_impl(double x, double y) {return x*y;}
double div_impl(double x, double y) {
    if(y == 0){
      //printf("%d / %d\n",x,y );
        avm_error("Error: cannot divide a number by 0!", NULL);
        return 0.0;
    }
    else {
        return x/y;
    }
}
double mod_impl(double x, double y){
    if(y == 0){
		avm_error("Error: cannot divide a number by 0!", NULL);
		return 0.0;
	}
	return ((unsigned ) x) % ((unsigned) y);
}

arithmetic_func_t arithmeticFuncs[] = {
    add_impl,
    sub_impl,
    mul_impl,
    div_impl,
    mod_impl
};

void execute_arithmetic(instruction* instr){

    avm_memcell* lv = avm_translate_operand(instr->result, (avm_memcell*)0);
    avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

    assert(lv && (&stack[AVM_STACKSIZE - 1] >= lv && lv > &stack[top] || lv == &retval));
	  assert(rv1 && rv2 );

    //printf("rv1: %d --- rv2: %d\n",rv1->type,rv2->type );

	if (rv1->type != number_m || rv2->type != number_m) {
		avm_error("Error: not a number in arithmetic!", NULL);
		executionFinished = 1;
	}
	else{

		arithmetic_func_t op = arithmeticFuncs[instr->opcode - 1];
		avm_memcellclear(lv);
		lv->type = number_m;
		lv->data.numVal = (*op)(rv1->data.numVal, rv2->data.numVal);
	}
}

unsigned char jge_impl(double x, double y) { return x >= y; }
unsigned char jgt_impl(double x, double y) { return x > y; }
unsigned char jle_impl(double x, double y) { return x <= y; }
unsigned char jlt_impl(double x, double y) { return x < y; }

jumps_t jumpsFuncs[] = {
  jle_impl,
	jge_impl,
	jlt_impl,
	jgt_impl
};

void execute_jWithsomething(instruction* instr){
    avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
	avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);
	jumps_t op;
	unsigned char result = 0;

	assert(instr->result->type == label_a);
	assert(rv1 && rv2 );

	if (rv1->type != number_m || rv2->type != number_m) {
	}
	else{
		op = jumpsFuncs[instr->opcode - 12];
		result = (*op)(rv1->data.numVal, rv2->data.numVal);
		if(result)
			pc = instr->result->val;
	}
}

unsigned char number_tobool(avm_memcell* m) {
    return m->data.numVal != 0;
}
unsigned char string_tobool(avm_memcell* m){
    return m->data.strVal[0] != 0;
}
unsigned char bool_tobool(avm_memcell* m){
    return m->data.boolVal;
}
unsigned char table_tobool(avm_memcell* m) {
    return 1;
}
unsigned char userfunc_tobool(avm_memcell* m){
    return 1;
}
unsigned char libfunc_tobool(avm_memcell* m){
    return 1;
}
unsigned char nil_tobool(avm_memcell* m) {
    return 0;
}
unsigned char undef_tobool(avm_memcell* m){
    assert(0);
    return 0;
}

tobool_func_t toboolFuncs[] = {
    number_tobool,
    string_tobool,
    bool_tobool,
    table_tobool,
    userfunc_tobool,
    libfunc_tobool,
    nil_tobool,
    undef_tobool
};

unsigned char avm_tobool(avm_memcell * m){
    assert(m->type >= 0 && m->type < undef_m);
    return(*toboolFuncs[m->type])(m);
}

void execute_jeq(instruction* instr){
	assert(instr->result->type == label_a);

	if(instr->arg1 == NULL || instr->arg2 == NULL)
		return;

	avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
	avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);

    unsigned char result = 0;

	if (rv1->type == undef_m || rv2->type == undef_m)
		avm_error("Error: 'undef' involved in equality!", NULL);
	else if(rv1->type == nil_m || rv2->type == nil_m)
		result = rv1->type == nil_m && rv2->type == nil_m;
	else if(rv1->type == bool_m || rv2->type == bool_m)
		result = (avm_tobool(rv1) == avm_tobool(rv2));
	else if(rv1->type != rv2->type)
		avm_error("Error: illegal equality!", NULL);
	else{
        if(rv1->type == number_m){
            int i;
            i = rv1->data.numVal == rv2->data.numVal;
            result = i;
        }
        else if(rv1->type == string_m )
            result = !strcmp(rv1->data.strVal, rv2->data.strVal);
        else if(rv1->type == table_m){
            int i;
            i = rv1->data.tableVal == rv2->data.tableVal;
            result = i;
        }
        else if(rv1->type == userfunc_m){
            int i;
            i = rv1->data.funcVal == rv2->data.funcVal;
            result = i;
        }
        else
            result = !strcmp(rv1->data.libfuncVal, rv2->data.libfuncVal);
    }

	if (!executionFinished && result )
		pc = instr->result->val;
}

void execute_jne(instruction* instr){
	assert(instr->result->type == label_a);

    if (instr->arg1 == NULL || instr->arg2 == NULL)
        return;

    avm_memcell* rv1 = avm_translate_operand(instr->arg1, &ax);
	avm_memcell* rv2 = avm_translate_operand(instr->arg2, &bx);
	unsigned char result = 0;

	if (rv1->type == undef_m || rv2->type == undef_m)
		avm_error("cant compare something with 'undef' \n", NULL);
	else if(rv1->type == nil_m || rv2->type == nil_m)
		result = !(rv1->type == nil_m && rv2->type == nil_m);
	else if(rv1->type == bool_m || rv2->type == bool_m)
		result = (avm_tobool(rv1) != avm_tobool(rv2));
	else if(rv1->type != rv2->type)
		avm_error("only can compare the same types\n", NULL);
	else{
        if(rv1->type == number_m)
            result = rv1->data.numVal != rv2->data.numVal;
        else if(rv1->type == string_m )
            result = strcmp(rv1->data.strVal, rv2->data.strVal);
        else if(rv1->type == table_m)
            result = (rv1->data.tableVal != rv2->data.tableVal);
        else if(rv1->type == userfunc_m)
            result = rv1->data.funcVal != rv2->data.funcVal ;
        else
            result = strcmp(rv1->data.libfuncVal, rv2->data.libfuncVal);
    }
	if (!executionFinished && result )
		pc = instr->result->val;
}

void execute_jump(instruction* instr){
	assert(instr->result->type == label_a);
	pc = instr->result->val;
}

void execute_newtable(instruction* instr){

    if (instr->result == NULL)
        return;

    avm_memcell* lv = avm_translate_operand(instr->result,(avm_memcell*) 0);
	assert(lv && (&stack[AVM_STACKSIZE-1] >= lv && lv > &stack[top] || lv == &retval));

    avm_memcellclear(lv);

	lv->type = table_m;
	lv->data.tableVal = avm_tablenew();
	avm_tableincrefcounter(lv->data.tableVal);
}

avm_memcell* avm_tablegetelem(avm_table* table,avm_memcell* index){
    //??
}

void execute_tablegetelem(instruction* instr){

    if (instr->arg1 == NULL || instr->arg2 == NULL || instr->result == NULL)
        return;

    avm_memcell* lv = avm_translate_operand(instr->result,(avm_memcell*) 0);
	avm_memcell* t = avm_translate_operand(instr->arg1,(avm_memcell*) 0);
	avm_memcell* i = avm_translate_operand(instr->arg2,&ax);

    assert(lv && (&stack[AVM_STACKSIZE-1] >= lv && lv > &stack[top] || lv == &retval));
	assert(t && &stack[AVM_STACKSIZE-1] >= t && t > &stack[top]);
	assert(i);

	avm_memcellclear(lv);
	lv->type = nil_m;

	if(t->type != table_m){
		avm_error("Error: illigal use of table!" , NULL);
	}
	else{
		avm_memcell* content = avm_tablegetelem(t->data.tableVal,i);
		if(content)
			avm_assign(lv,content);
		else{
			char* ts = avm_tostring(t);
			char* is = avm_tostring(i);
			avm_warning("not found!");
			free(ts);
			free(is);
		}
	}
}

void avm_tablesetelem(avm_table* table,avm_memcell* index, avm_memcell* content){

}

void execute_tablesetelem(instruction* instr){

    if (instr->arg1 == NULL || instr->arg2 == NULL || instr->result == NULL)
        return;

    avm_memcell* t = avm_translate_operand(instr->arg1,(avm_memcell*) 0);
	avm_memcell* i = avm_translate_operand(instr->arg2,&ax);
	avm_memcell* c = avm_translate_operand(instr->result,&bx);

    assert(t && &stack[AVM_STACKSIZE-1] >= t && t > &stack[top]);
	assert(i && c);
	if(t->type != table_m)
		avm_error("Error: illigal use of table", NULL);
	else
		avm_tablesetelem(t->data.tableVal, i, c);
}

execute_func_t executeFuncs[]={
  execute_assign,
	execute_add,
	execute_sub,
	execute_mul,
	execute_div,
	execute_mod,
	0,
	0,
	0,
	0,
	execute_jeq,
	execute_jne,
	execute_jle,
	execute_jge,
	execute_jlt,
	execute_jgt,
	execute_call,
	execute_pusharg,
	execute_funcenter,
	execute_funcexit,
	execute_newtable,
	execute_tablegetelem,
	execute_tablesetelem,
	execute_jump,
	0
};

void execute_cyrcle(void){
    if(executionFinished){
        return;
    }
    else if(pc == AVM_ENDING_PC){
		//printf("elpizo oxi?");
        executionFinished = 1;
        return;
    }
    else {
        assert(pc<AVM_ENDING_PC);
        instruction * instr = code + pc;
        assert(instr->opcode >= 0 && instr->opcode <=AVM_MAX_INSTRUCTIONS);
        if(instr->strLine){
            currLine = instr->strLine;
        }
        unsigned oldPc = pc;

        //printf("Executing %d",instr->opcode);

        //printf("GAMW TO SOI : %d\n", instr->opcode);
        (*executeFuncs[instr->opcode])(instr);

        //printf("GAMW TO SOI : %d\n", instr->opcode);
        //printf("    Done!\n");


        if(pc == oldPc){
            ++pc;
        }
    }
}

void libfunc_input(void){

	char *s = (char*)malloc(1024 * sizeof(char));
	fgets(s, sizeof(s), stdin);
	unsigned int i = 0;
	unsigned int isalpha = 0;
	unsigned int isdot = 0;

	for(i = 0; i < strlen(s); i++){
		if(isalpha(s[i]) && strcmp(&s[i],".")){
			isalpha = 1;
		}
		else if(!strcmp(s,"nil")){

			avm_memcellclear(&retval);
			retval.type = nil_m;
			break;
		}
		else if(!strcmp(s,"true") || !strcmp(s,"TRUE")){

			avm_memcellclear(&retval);
			retval.type = bool_m;
			retval.data.boolVal = 1;
			break;
		}
		else if(!strcmp(s,"false") || !strcmp(s,"FALSE")){

			avm_memcellclear(&retval);
			retval.type = bool_m;
			retval.data.boolVal = 0;
			break;
		}
		else if(!strcmp(&s[i],".")){
			isdot = 1;
		}
	}
	if(isalpha){
		avm_memcellclear(&retval);
		retval.type = string_m;
		retval.data.strVal = strdup(s);
	}
	else{
		avm_memcellclear(&retval);
		retval.type = number_m;
		if(isdot > 0){
			retval.data.numVal = atof(s);
		}
		else {
			retval.data.numVal = atoi(s);
		}
	}
}

void libfunc_cos(void){

	unsigned n  = avm_totalactuals();

    char str[10];
    sprintf(str, "%d", n);

	if(n!=1){
		avm_error("One argument (not %d) expected in cos\n", str);
	}
	else if(avm_getactual(0) -> type != number_m){
		avm_error("cos() function expected a real number\n", NULL);
	}
	else{
		double num = avm_getactual(0) -> data.numVal;

		avm_memcellclear(&retval);
		retval.type = number_m;
		retval.data.numVal = cos(num);

	}
}

void libfunc_sin(void){

	unsigned n  = avm_totalactuals();

    char str[10];
    sprintf(str, "%d", n);

	if(n!=1){
		avm_error("One argument (not %d) expected in sin\n", str);
	}
	else if(avm_getactual(0) -> type != number_m){
		avm_error("sin() function expected a real number\n",NULL);
	}
	else{

		double num = avm_getactual(0) -> data.numVal;

		avm_memcellclear(&retval);
		retval.type = number_m;
		retval.data.numVal = sin(num);

	}
}

void libfunc_objectmemberkeys(void){

	unsigned n = avm_totalactuals();

    char str[10];
    sprintf(str, "%d", n);

	if(n!=1){
		avm_error("One argument (not %d) expected in objectmemberkeys\n", str);
	}
	else if(avm_getactual(0) -> type != table_m){
		avm_error("objectmemberkeys() function expected a table\n", NULL);
	}
}

void libfunc_objecttotalmembers(void){

	unsigned n = avm_totalactuals();

    char str[10];
    sprintf(str, "%d", n);

	if(n!=1){
		avm_error("One argument (not %d) expected in objecttotalmembers\n", str);
	}
	else if(avm_getactual(0) -> type != table_m){
		avm_error("objecttotalmembers() function expected a table\n", NULL);
	}
	else{
		int num = avm_getactual(0) -> data.tableVal -> total;
		avm_memcellclear(&retval);
		retval.type = number_m;
		retval.data.numVal = num;

	}
}

void libfunc_objectcopy(void){

	unsigned n = avm_totalactuals();

    char str[10];
    sprintf(str, "%d", n);

	if(n!=1){
		avm_error("One argument (not %d) expected in objectcopy\n", str);
	}
	else if(avm_getactual(0) -> type != table_m){
		avm_error("objectcopy() function expected a table\n", NULL);

	}
}

void libfunc_argument(void){

	unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
	double num = 0.0;
	char *s;
	unsigned char boolean;

	avm_memcellclear(&retval);

	if(!p_topsp){
		avm_error("argument called outside a function\n", NULL);
		retval.type = nil_m;
	}
	else{
		if(avm_getactual(0) -> type == number_m){
			num = avm_getactual(0) -> data.numVal;
			retval.type = number_m;
			retval.data.numVal = num;
		}
		else if(avm_getactual(0) -> type == string_m){
			s = strdup(avm_getactual(0)->data.strVal);
			retval.type = string_m;
			retval.data.strVal =strdup((char *)s);
		}
		else if(avm_getactual(0) -> type == bool_m){
			boolean = avm_getactual(0) -> data.boolVal;
			retval.type = bool_m;
			retval.data.boolVal = boolean;
		}
		else if(avm_getactual(0) -> type == table_m){
			retval.type = table_m;
			retval.data.tableVal = avm_getactual(0) -> data.tableVal;
		}
		else if(avm_getactual(0) -> type == userfunc_m){
			retval.type = userfunc_m;
			retval.data.funcVal = avm_getactual(0) -> data.funcVal;
		}
		else if(avm_getactual(0) -> type == libfunc_m){
			retval.type = libfunc_m;
			retval.data.libfuncVal = avm_getactual(0) -> data.libfuncVal;
		}
	}
}

void libfunc_strtonum(void){
	unsigned n = avm_totalactuals();

    char str[10];
    sprintf(str, "%d", n);

	if(n!=1){
		avm_error("One argument (not %d) expected in strtonum \n", str);
	}
	else if(avm_getactual(0) -> type != string_m){
		avm_error("strtonum() function expected a string\n", NULL);
	}
	else{
		char *s = strdup(avm_getactual(0) -> data.strVal);
		double num = atof(s);
		if(num != 0.0){
			avm_memcellclear(&retval);
			retval.type = number_m;
			retval.data.numVal = num;
		}
		else{
			avm_memcellclear(&retval);
			retval.type = nil_m;
		}
	}
}

void libfunc_sqrt(void){

	unsigned n = avm_totalactuals();

    char str[10];
    sprintf(str, "%d", n);

	if(n!=1){
		avm_error("One argument (not %d) expected in sqrt\n", str);
	}
	else if(avm_getactual(0) -> type != number_m){
		avm_error("sqrt() function expected a real number\n", NULL);
	}
    else{
		double num = avm_getactual(0) -> data.numVal;
		if(num < 0){
			avm_memcellclear(&retval);
			retval.type = nil_m;
			avm_error("sqrt() function expected a positive number\n", NULL);
		}
		else{
			avm_memcellclear(&retval);
			retval.type = number_m;
			retval.data.numVal = sqrt(num);
		}
	}
}

void avm_init(){
    avm_initstack();

    avm_arrayOfLibFuncsInit(libfunc_print);
    avm_arrayOfLibFuncsInit(libfunc_input);
    avm_arrayOfLibFuncsInit(libfunc_objectmemberkeys);
    avm_arrayOfLibFuncsInit(libfunc_objecttotalmembers);
    avm_arrayOfLibFuncsInit(libfunc_objectcopy);
    avm_arrayOfLibFuncsInit(libfunc_totalarguments);
    avm_arrayOfLibFuncsInit(libfunc_argument);
    avm_arrayOfLibFuncsInit(libfunc_typeof);
    avm_arrayOfLibFuncsInit(libfunc_strtonum);
    avm_arrayOfLibFuncsInit(libfunc_sqrt);
    avm_arrayOfLibFuncsInit(libfunc_cos);
    avm_arrayOfLibFuncsInit(libfunc_sin);
}


int readBinary(){
	FILE* file = fopen("code.bin","rb");
	int x=0;
	int i;
	int tot[4];

	char magic[10];

	fread(magic,10,1,file);
	if(strcmp(magic,"340200501") != 0){
		return -1;
	}

	fread(&globalsSum,sizeof(unsigned int),1,file);

	fread(tot,sizeof(int),4,file);

	double numbers[tot[0]];
	fread(numbers,sizeof(double),tot[0],file);

	total_Numbers_Consts = tot[0];
	Consts_Numbers = malloc(sizeof(double)*total_Numbers_Consts);
	for ( i = 0; i < total_Numbers_Consts; ++i)
		Consts_Numbers[i] = numbers[i];

    total_String_Consts = tot[1];

	
	Consts_String = malloc(sizeof(char * ) * total_String_Consts);
    for (int i = 0; i < total_String_Consts; i++) {
        int string_Length;
        fread(&string_Length, sizeof(int), 1, file);

		char * temp = malloc(sizeof(char) * string_Length);
        fread(temp, sizeof(char), string_Length, file);

        Consts_String[i] = strdup(temp);
        //puts(Consts_String[i]);
    }

	userfunc FuncCon[tot[2]];
	fread(FuncCon,sizeof(userfunc),tot[2],file);
	total_Func_Consts = tot[2];
	Consts_Func = malloc(sizeof(userfunc)*total_Func_Consts);
	for ( i = 0; i < total_Func_Consts; ++i)
		Consts_Func[i] = FuncCon[i];

    //printf("INSTRUCTIONS = %d\n", tot[3]);
    
    code = malloc(sizeof(instruction) * tot[3]);
    total_instr = tot[3];

    for (i = 0; i < total_instr; ++i) {

        vmopcode opcode;
        vmarg result;
        vmarg arg1;
        vmarg arg2;
        unsigned strLine;

        fread(&opcode, 	sizeof(vmopcode), 	1, file);
        fread(&result, 	sizeof(vmarg), 		1, file);
        fread(&arg1, 	sizeof(vmarg), 		1, file);
        fread(&arg2, 	sizeof(vmarg), 		1, file);
        fread(&strLine, sizeof(unsigned), 	1, file);

		code[i].opcode = opcode;
		
		if(result.val == 696969)
			code[i].result = NULL;
		else{
			code[i].result = malloc(sizeof(vmarg *));
			*(code[i].result) = result; 
		}

        if (arg1.val == 696969)
            code[i].arg1 = NULL;
        else {
            code[i].arg1 = malloc(sizeof(vmarg *));
            *(code[i].arg1) = arg1;
        }
        if (arg2.val == 696969)
            code[i].arg2 = NULL;
        else {
            code[i].arg2 = malloc(sizeof(vmarg *));
            *(code[i].arg2) = arg2;
        }

        code[i].strLine = strLine;

        
    }

	//assert(0);
	



	fclose(file);
	return 0;
}

void avm(){
	avm_init();
	top = AVM_STACKSIZE - globalsSum - 1;

	topsp = top;
	while (executionFinished != 1){
		execute_cyrcle();
	}
}


int main (int argc, char** argv) {
	FILE* fp;
	fp = fopen("code.bin", "r");
	if (fp) {
		readBinary();
		avm();
	}
	else {
		fprintf(stderr, "Cannot open file: code.bin\n");
		assert(0);
	}

	return 0;
}
