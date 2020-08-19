#include <assert.h>

typedef struct Variable {
    const char *name;
    unsigned int scope;
    unsigned int line;
} Variable;

typedef struct Function {
    char *name;
    const char *arguments;
    unsigned int scope;
    unsigned int line;
} Function;

enum SymbolType {
    GLOBAL, LOCAL, FORMAL,
    USERFUNC, LIBFUNC
};

typedef enum scopespace {
  programvar,
  functionlocal,
  formalarg
}scopespace_t;

enum symbol_t {
  var_s,
  programfunc_s,
  libraryfunc_s
};

typedef struct SymbolTableEntry {
  int isActive;
  union {
      Variable *varVal;
      Function *funcVal;
  } value;
  enum symbol_t type;

  int functionIntervene;

  scopespace_t space;
  unsigned offset;
  unsigned iaddress;
  unsigned totalLocals;

  struct SymbolTableEntry * next;
  struct SymbolTableEntry * prev;
} SymbolTableEntry;
