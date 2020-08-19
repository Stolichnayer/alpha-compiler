#include "quads.h"


#define T_CURR_SIZE (totalInstructions*sizeof(instruction))
#define T_NEW_SIZE 	(EXPAND_SIZE*sizeof(instruction)+T_CURR_SIZE)

//unsigned totalInstructions;



typedef enum vmopcode {
  assign_v,         add_v,            sub_v,
  mul_v,            div_v,            mod_v,
  uminus_v,         and_v,            or_v,
  not_v,            jeq_v,            jne_v,
  jle_v,            jge_v,            jlt_v,
  jgt_v,            call_v,
  pusharg_v,        funcenter_v,      funcexit_v,       
  newtable_v,       tablegetelem_v,   tablesetelem_v, jump_v,  
  nop_v
}vmopcode;

typedef enum vmarg_t {
    label_a = 0,
    global_a = 1,
    formal_a = 2,
    local_a = 3,
    number_a = 4,
    string_a = 5,
    bool_a = 6,
    nil_a = 7,
    userfunc_a = 8,
    libfunc_a = 9,
    retval_a = 10
}vmarg_t;

typedef struct vmarg {
    vmarg_t type;
    unsigned val;
}vmarg;

typedef struct instruction {
    vmopcode opcode;
    vmarg  *  result;
    vmarg  *  arg1;
    vmarg  *  arg2;
    unsigned strLine;
}instruction;

typedef struct userfunc {
    unsigned address;
    unsigned localSize;
    char * id;
}userfunc;

typedef struct incomplete_jump {
    unsigned instrNo;
    unsigned iaddress;
    struct incomplete_jump * next;
}incomplete_jump;

typedef struct return_List{
	int label;
	struct return_List *next;

}return_list;

typedef struct func_stack{
	struct SymbolTableEntry * func;
	struct func_stack* next;
	struct return_List * returnNode;
}func_stack;

struct func_stack *f_top;

typedef void (*generator_func_t) (quad*);

void make_operand(expr * e, vmarg * arg);
void make_booloperand(vmarg * arg, unsigned val);
unsigned consts_newstring(char * s);
unsigned consts_newnumber(double n);
unsigned libfuncs_newused(char * s);
unsigned userfuncs_newfunc(SymbolTableEntry * sym);
void make_retvaloperand(vmarg * arg);
vmarg * reset_operand();
void generate_all(vmopcode op, quad * quad);
void generate_ADD(quad* quad);
void generate_SUB(quad* quad);
void generate_MUL(quad* quad);
void generate_DIV(quad* quad);
void generate_MOD(quad* quad);
void generate_NEWTABLE(quad* quad);
void generate_TABLEGETELEM(quad* quad);
void generate_TABLESETELEM(quad* quad);
void generate_ASSIGN(quad* quad);
void generate_NOP();
void generate_JUMP(quad* quad);
void generate_IF_EQ(quad* quad);
void generate_IF_NOTEQ(quad* quad);
void generate_IF_GREATER(quad* quad);
void generate_IF_GREATERQ(quad* quad);
void generate_IF_LESS(quad* quad);
void generate_IF_LESSEQ(quad* quad);
void generate_PARAM(quad* quad);
void generate_NOT(quad * quad);
void generate_OR(quad * quad);
void generate_CALL(quad* quad);
void generate_GETRETVAL(quad* quad);
void generate_FUNCSTART(quad* quad);
void generate_RETURN(quad* quad);
void generate_FUNCEND(quad* quad);
void generate(void);
void printConsts();