#include "targetCode.h"

#define AVM_STACKENV_SIZE 		4
#define AVM_MAX_INSTRUCTIONS	(unsigned) nop_v
#define	AVM_SAVEDTOP_OFFSET		2
#define	AVM_SAVEDPC_OFFSET 		3
#define	AVM_SAVEDTOPSP_OFFSET 	1
#define AVM_NUMACTUALS_OFFSET 	4

#define execute_add execute_arithmetic
#define execute_sub execute_arithmetic
#define execute_mul execute_arithmetic
#define execute_div execute_arithmetic
#define execute_mod execute_arithmetic

#define execute_jle execute_jWithsomething
#define execute_jge execute_jWithsomething
#define execute_jlt execute_jWithsomething
#define execute_jgt execute_jWithsomething


typedef enum avm_memcell_t {
    number_m = 0,
    string_m = 1,
    bool_m = 2,
    table_m = 3,
    userfunc_m = 4,
    libfunc_m = 5,
    nil_m = 6,
    undef_m = 7
}avm_memcell_t;

#define AVM_TABLE_HASHSIZE 211

typedef struct avm_table avm_table;

typedef struct avm_memcell {
    avm_memcell_t type;
    union {
        double  numVal;
        char *  strVal;
        unsigned char boolVal;
        avm_table *   tableVal;
        unsigned      funcVal;
        char *        libfuncVal;
    }data;
}avm_memcell;

typedef struct avm_table_bucket {
    avm_memcell key;
    avm_memcell value;
    struct avm_table_bucket * next;
}avm_table_bucket;

typedef struct avm_table {
    unsigned refCounter;
    avm_table_bucket * strIndexed[AVM_TABLE_HASHSIZE];
    avm_table_bucket * numIndexed[AVM_TABLE_HASHSIZE];
    unsigned total;
}avm_table;

#define AVM_STACKSIZE 4096
#define AVM_WIPEOUT(m) memset(&(m), 0, sizeof(m))

avm_memcell stack[AVM_STACKSIZE];

typedef void (*execute_func_t) (instruction*);
typedef void (*library_func_t)(void);
typedef double (*arithmetic_func_t)(double x, double y);
typedef unsigned char (*tobool_func_t)(avm_memcell*);
typedef unsigned char (*jumps_t)(double x, double y);
typedef char* (*tostring_func_t) (avm_memcell*);


static void avm_initstack(void);
void avm_tableincrefcounter(avm_table * t);
void avm_tabledecrefcounter(avm_table * t);
void avm_tablebucketinit(avm_table_bucket ** p);
avm_table * avm_tablenew(void);
void avm_memcellclear(avm_memcell * m);
void avm_tablebucketdestroy(avm_table_bucket** p);
void avm_tabledestroy(avm_table * t);
void avm_push_envvalue(unsigned i);
void avm();
int readBinary();