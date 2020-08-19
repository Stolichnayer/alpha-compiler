%{
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
%}

%start program

%union {
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
}

%token <intValue> INTEGER
%token <realValue> REAL
%token <stringValue> STRING ID BREAK CONTINUE FUNCTION WHILE IF FOR RETURN ELSE AND OR NOT TRUE FALSE NIL ADD SUB MUL MOD DIV EQ NEQ GR GREQ LESS LEQ INC DEC LBR RBR LCURBR RCURBR LPAR RPAR COMMA SEMIC COLON DCOLON DOT DDOT LOCALL

%type <intValue> funcbody
%type <stringValue> ifstmt whilestmt forstmt returnstmt block blocknext forkeyword
%type <stringValue> idlist idlistnext funcname
%type <expression> lvalue member primary assignexpr call term objectdef const expr elist indexedelem indexed
%type <symbol> funcdef funcprefix
%type <calls> methodcall normcall callsuffix
%type <unsignedvalue> ifprefix elseprefix whilestart whilecond N M
%type <forstct> forprefix
%type <forstmt_t> stmt loopstmt

%right  ASS
%left  OR AND
%nonassoc  EQ NEQ
%nonassoc LESS GREQ LEQ GR
%left ADD SUB
%left MUL DIV MOD
%right NOT INC DEC UMINUS
%left DOT DDOT
%left LBR RBR
%left LPAR RPAR



%%

program:    stmt program        {////printf("program -> stmt program \n");
                                }
            |                   {////printf("program -> empty \n");
                                }
            ;

//stmts:      stmt                {////printf("ppapappa\n");}

stmt:       expr SEMIC          {////printf("stmt -> expr SEMIC \n"); resettemp();
                                }
            | ifstmt            {////printf("stmt -> ifstmt \n"); resettemp();
                                }
            | whilestmt         {////printf("stmt -> whilestmt \n"); resettemp();
            }
            | forstmt           {////printf("stmt -> forstmt \n"); resettemp();
            }
            | returnstmt        {////printf("stmt -> returnstmt \n"); resettemp();
            }
            | BREAK SEMIC       {
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
            | CONTINUE SEMIC    {
                                    if(loopcounter == 0)
                                        printf("Error at line %d: use of \"continue\" while not in a loop\n", lineno);
                                    else {

                                        emit(jump, NULL, NULL, NULL, s.contList, lineno);
                                        s.contList = nextquad();
                                    }
                                    //printf("stmt -> CONTINUE SEMIC \n");
                                    resettemp();
                                }
            | block             {//printf("stmt -> block \n");resettemp();
            }
            | funcdef           {//printf("stmt -> funcdef \n");resettemp();
            }
            | SEMIC             {//printf("stmt -> SEMIC \n");resettemp();
            }
            ;



expr:       assignexpr          {//printf("expr -> assignexpr \n");$<expression>$ = $<expression>1;
}
            | expr ADD expr     {
                                    $<expression>$ = newexpr(arithexpr_e);
                                    $<expression>$->sym = newtemp();

                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    emit(add, $<expression>$, $<expression>1, $<expression>3, -1, lineno);
                                    //printf("expr ADD expr\n");

                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
            | expr SUB expr     {
                                    $<expression>$ = newexpr(arithexpr_e);
                                    $<expression>$->sym = newtemp();

                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    
                                    emit(sub, $<expression>$, $<expression>1, $<expression>3, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
            | expr MUL expr     {
                                    $<expression>$ = newexpr(arithexpr_e);
                                    $<expression>$->sym = newtemp();
                                                                        
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    
                                    emit(mul, $<expression>$, $<expression>1, $<expression>3, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
            | expr DIV expr     {
                                    $<expression>$ = newexpr(arithexpr_e);
                                    $<expression>$->sym = newtemp();
                                                                        
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    
                                    emit(divide, $<expression>$, $<expression>1, $<expression>3, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
            | expr MOD expr     {
                                    $<expression>$ = newexpr(arithexpr_e);
                                    $<expression>$->sym = newtemp();
                                                                        
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    
                                    emit(mod, $<expression>$, $<expression>1, $<expression>3, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
            | expr GR expr      {
                                    $<expression>$ = newexpr(boolexpr_e);
                                    $<expression>$->sym = newtemp();
                                                                        
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(if_greater, NULL, $<expression>1, $<expression>3, nextquad() + 3, lineno);
                                    emit(assign, $<expression>$, newexpr_constbool(0), NULL, -1, lineno);
                                    emit(jump, NULL, NULL, NULL, nextquad() + 2, lineno);
                                    emit(assign, $<expression>$, newexpr_constbool(1), NULL, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
            | expr GREQ expr    {
                                    $<expression>$ = newexpr(boolexpr_e);
                                    $<expression>$->sym = newtemp();
                                                                        
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(if_greatereq, NULL, $<expression>1, $<expression>3, nextquad() + 3, lineno);
                                    emit(assign, $<expression>$, newexpr_constbool(0), NULL, -1, lineno);
                                    emit(jump, NULL, NULL, NULL, nextquad() + 2, lineno);
                                    emit(assign, $<expression>$, newexpr_constbool(1), NULL, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
            | expr LESS expr    {
                                    $<expression>$ = newexpr(boolexpr_e);
                                    $<expression>$->sym = newtemp();
                                                                        
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(if_less, NULL, $<expression>1, $<expression>3, nextquad() + 3, lineno);
                                    emit(assign, $<expression>$, newexpr_constbool(0), NULL, -1, lineno);
                                    emit(jump, NULL, NULL, NULL, nextquad() + 2, lineno);
                                    emit(assign, $<expression>$, newexpr_constbool(1), NULL, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                            globalsSum++;
                                }
            | expr LEQ expr     {
                                    $<expression>$ = newexpr(boolexpr_e);
                                    $<expression>$->sym = newtemp();
                                                                        
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(if_lesseq, NULL, $<expression>1, $<expression>3, nextquad() + 3, lineno);
                                    emit(assign, $<expression>$, newexpr_constbool(0), NULL, -1, lineno);
                                    emit(jump, NULL, NULL, NULL, nextquad() + 2, lineno);
                                    emit(assign, $<expression>$, newexpr_constbool(1), NULL, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
            | expr EQ expr      {
                                    $<expression>$ = newexpr(boolexpr_e);
                                    $<expression>$->sym = newtemp();
                                                                        
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(if_eq, NULL, $<expression>1, $<expression>3, nextquad() + 3, lineno);
                                    emit(assign, $<expression>$, newexpr_constbool(0), NULL, -1, lineno);
                                    emit(jump, NULL, NULL, NULL, nextquad() + 2, lineno);
                                    emit(assign, $<expression>$, newexpr_constbool(1), NULL, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
            | expr NEQ expr     {
                                    $<expression>$ = newexpr(boolexpr_e);
                                    $<expression>$->sym = newtemp();
                                                                        
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(if_noteq, NULL, $<expression>1, $<expression>3, nextquad() + 3, lineno);
                                    emit(assign, $<expression>$, newexpr_constbool(0), NULL, -1, lineno);
                                    emit(jump, NULL, NULL, NULL, nextquad() + 2, lineno);
                                    emit(assign, $<expression>$, newexpr_constbool(1), NULL, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
            | expr AND expr     {
                                    $<expression>$ = newexpr(boolexpr_e);
                                    $<expression>$->sym = newtemp();
                                                                        
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(and, $<expression>$, $<expression>1, $<expression>3, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
            | expr OR expr      {
                                    $<expression>$ = newexpr(boolexpr_e);
                                    $<expression>$->sym = newtemp();
                                                                        
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    emit(or, $<expression>$, $<expression>1, $<expression>3, -1, lineno);
                                
                                    if(currscopespace() == programvar)
                                        globalsSum++;
                                }
            | term              {
                                    //printf("expr -> term \n");
                                    $<expression>$ = $<expression>1;
                                }
            ;

term:       LPAR expr RPAR      {
                                    //printf("term -> LPAR expr RPAR \n");

                                    $<expression>$ = $<expression>2;

                                }
            | SUB expr %prec UMINUS {

                                        //printf("term -> UMINUS expr \n");

                                        check_arith($<expression>2);
                                        $<expression>$ = newexpr(arithexpr_e);
                                        $<expression>$->sym = istempexpr($<expression>2) ? $<expression>2->sym : newtemp();

                                                                            
                                        $<expression>$->sym->offset = currscopeoffset();
                                        $<expression>$->sym->space = currscopespace();
                                        incurrscopeoffset();

                                    
                                        emit(uminus, $<expression>$, NULL, $<expression>2, -1, lineno);
                                    }
            | NOT expr          {
                                    //printf("term -> NOT expr \n");
                                    $<expression>$ = newexpr(boolexpr_e);
                                    $<expression>$->sym = newtemp();
                                                                        
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    
                                    emit(not, $<expression>$, $<expression>2, NULL, -1, lineno);
                                }
            | INC lvalue        {
                                    //printf("term -> INC lvalue \n");

                                    SymbolTableEntry * tmp;
                                    tmp = lookUpSymbol(-1, $<stringValue>2, currentScope);

                                    if(tmp != NULL){
                                        if(tmp->type == 3 || tmp->type == 4){
                                            if(tmp->isActive)
                                                printf("Error at line %d: cannot increase value of a function\n", lineno);
                                            else if(tmp->functionIntervene)
                                                printf("Error at line %d: cannot access symbol %s\n", lineno , tmp->value.funcVal->name);
                                        }
                                    }

                                    check_arith($<expression>2);


                                    if($<expression>2->type == tableitem_e){
                                        $<expression>$ = emit_iftableitem($<expression>2);
                                        emit(add, $<expression>$, newexpr_constnum(1), $<expression>$, -1, lineno);
                                        emit(tablesetelem, $<expression>2, $<expression>2->index, $<expression>$, -1, lineno);
                                    }
                                    else {
                                        emit(add, $<expression>2, newexpr_constnum(1), $<expression>2, -1, lineno);
                                        $<expression>$ = newexpr(arithexpr_e);
                                        $<expression>$->sym = newtemp();

                                                                            
                                        $<expression>$->sym->offset = currscopeoffset();
                                        $<expression>$->sym->space = currscopespace();
                                        incurrscopeoffset();

                                    
                                        emit(assign, $<expression>$, NULL, $<expression>2, -1, lineno);
                                    }

                                }
            | lvalue INC        {

                                    //printf("term -> lvalue INC \n");

                                    SymbolTableEntry * tmp;
                                    tmp = lookUpSymbol(-1, $<stringValue>1, currentScope);

                                    if(tmp != NULL){
                                        if(tmp->type == 3 || tmp->type == 4){
                                            if(tmp->isActive)
                                                printf("Error at line %d: cannot increase value of a function\n", lineno);
                                            else if(tmp->functionIntervene)
                                                printf("Error at line %d: cannot access symbol %s\n", lineno, tmp->value.funcVal->name);
                                        }
                                    }

                                    check_arith($<expression>1);
                                    $<expression>$ = newexpr(var_e);
                                    $<expression>$->sym = newtemp();

                                    
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    
                                    if($<expression>1->type == tableitem_e){
                                        expr * val = emit_iftableitem($<expression>1);
                                        emit(assign, $<expression>$, NULL, val, -1, lineno);
                                        emit(add, val, newexpr_constnum(1), val, -1, lineno);
                                        emit(tablesetelem, $<expression>1, $<expression>1->index, val, -1, lineno);
                                    }
                                    else {
                                        emit(assign, $<expression>$, NULL, $<expression>1, -1, lineno);
                                        emit(add, $<expression>1, newexpr_constnum(1), $<expression>1, -1, lineno);
                                    }
                                }
            | DEC lvalue        {

                                    //printf("term -> DEC lvalue \n");

                                    SymbolTableEntry * tmp;
                                    tmp = lookUpSymbol(-1, $<stringValue>2, currentScope);

                                    if(tmp != NULL){
                                        if(tmp->type == 3 || tmp->type == 4){
                                            if(tmp->isActive)
                                                printf("Error at line %d: cannot decrease value of a function\n", lineno);
                                            else if(tmp->functionIntervene)
                                                printf("Error at line %d: cannot access symbol %s\n", lineno, tmp->value.funcVal->name);
                                        }
                                    }

                                    check_arith($<expression>2);

                                    if($<expression>2->type == tableitem_e){
                                        $<expression>$ = emit_iftableitem($<expression>2);
                                        emit(sub, $<expression>$, newexpr_constnum(1), $<expression>$, -1, lineno);
                                        emit(tablesetelem, $<expression>2, $<expression>2->index, $<expression>$, -1, lineno);
                                    }
                                    else {

                                        emit(sub, $<expression>2, newexpr_constnum(1), $<expression>2, -1, lineno);
                                        $<expression>$ = newexpr(arithexpr_e);
                                        $<expression>$->sym = newtemp();

                                                                            
                                        $<expression>$->sym->offset = currscopeoffset();
                                        $<expression>$->sym->space = currscopespace();
                                        incurrscopeoffset();

                                    
                                        emit(assign, $<expression>$, NULL, $<expression>2, -1, lineno);

                                    }
                                }
            | lvalue DEC        {

                                    //printf("term -> lvalue DEC \n");

                                    SymbolTableEntry * tmp;
                                    tmp = lookUpSymbol(-1, $<stringValue>1, currentScope);

                                    if(tmp != NULL){
                                        if(tmp->type == 3 || tmp->type == 4){
                                            if(tmp->isActive)
                                                printf("Error at line %d: cannot decrease value of a function\n", lineno);
                                            else if(tmp->functionIntervene)
                                                printf("Error at line %d: cannot access symbol %s\n", lineno, tmp->value.funcVal->name);
                                        }
                                    }

                                    check_arith($<expression>1);
                                    $<expression>$ = newexpr(var_e);
                                    $<expression>$->sym = newtemp();

                                                                        
                                    $<expression>$->sym->offset = currscopeoffset();
                                    $<expression>$->sym->space = currscopespace();
                                    incurrscopeoffset();

                                    

                                    if($<expression>1->type == tableitem_e){
                                        expr * val = emit_iftableitem($<expression>1);
                                        emit(assign, $<expression>$, NULL, val, -1, lineno);
                                        emit(sub, val, newexpr_constnum(1), val, -1, lineno);
                                        emit(tablesetelem, $<expression>1, $<expression>1->index, val, -1, lineno);
                                    }
                                    else {
                                        emit(assign, $<expression>$, NULL, $<expression>1, -1, lineno);
                                        emit(sub, $<expression>1, newexpr_constnum(1), $<expression>1, -1, lineno);
                                    }
                                }
            | primary           {//printf("term -> primary \n");

                                    $<expression>$ = $<expression>1;

                                }
            ;

assignexpr: lvalue ASS expr     {
                                    //printf("assignexpr -> lvalue ASS expr \n");

                                    SymbolTableEntry * tmp = NULL;
                                    tmp = lookUpSymbol(-1, $<stringValue>1, currentScope);

                                    if(tmp != NULL){
                                        if((tmp->type == 3 || tmp->type == 4) && isMember == 0){
                                            printf("Error at line %d: cannot assign value to a function\n", lineno);

                                        }
                                        else if((tmp->type == 0 || tmp->type == 1 || tmp->type == 2) && isMember == 0){
                                            if(tmp->functionIntervene)
                                                printf("Error at line %d: cannot access symbol %s\n", lineno, tmp->value.funcVal->name);

                                        }
                                    }

                                    if($<expression>1->type == tableitem_e){
                                        emit(tablesetelem, $<expression>1, $<expression>1->index, $<expression>3, -1, lineno);
                                        $<expression>$ = emit_iftableitem($<expression>1);
                                        $<expression>$->type = assignexpr_e;
                                    }
                                    else {
                                        emit(assign, $<expression>1, NULL, $<expression>3, -1, lineno);
                                        $<expression>$ = newexpr(assignexpr_e);
                                        $<expression>$->sym = newtemp();

                                        $<expression>$->sym->offset = currscopeoffset();
					                    $<expression>$->sym->space = currscopespace();
					                    incurrscopeoffset();


                                        
                                        emit(assign, $<expression>$, NULL, $<expression>1, -1, lineno);
                                    
                                        if(currscopespace() == programvar)
                                            globalsSum++;
                                    }
                                }
            | ASS expr          {
                                    printf("Error at line %d: cannot assign expr to a function call\n", lineno);
                                    //printf("assignexpr -> ASS expr\n");
                                }
            ;

primary:    lvalue              {//printf("primary -> lvalue \n"); $<expression>$ = emit_iftableitem($<expression>1);
}
            | call              {//printf("primary -> call \n"); //$<stringValue>$ = $<stringValue>1;
                                }
            | call assignexpr   {//printf("primary -> call assignexpr\n");
            }
            | objectdef         {//printf("primary -> objectdef \n");
            }
            | LPAR funcdef RPAR {
                                    //printf("primary -> LPAR funcdef RPAR \n");

                                    $<expression>$ = newexpr(programfunc_e);
                                    $<expression>$->sym = $<symbol>2;
                                }
            | const             {//printf("primary -> const \n");
                                    $<expression>$ = $<expression>1;
                                }
            ;

lvalue:     ID                  {
                                    SymbolTableEntry * tmp;

                                    int scopeTmp;
                                    for(scopeTmp = currentScope; scopeTmp >= 0; scopeTmp--){

                                        tmp = lookUpSymbol(scopeTmp, $1, currentScope);

                                        if(tmp != NULL){
                                            if(tmp->isActive){
                                                break;
                                            }
                                        }
                                    }

                                    if(tmp != NULL){
                                        if(!tmp->isActive){
                                            tmp = lookUpSymbol(-1, $1, currentScope);
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
                                            tmp = insert($1, currentScope, lineno, NULL, var_s);
                                        }
                                        else {
                                            tmp = insert($1, currentScope, lineno, NULL, var_s);
                                        }
                                        tmp->space = currscopespace();
                                        tmp->offset = currscopeoffset();
                                    }

                                    $<expression>$ = lvalue_expr(tmp);
                                    //printf("lvalue -> ID \n");
                                }
            | LOCALL ID          {
                                    int value = lookUp(currentScope, $2, currentScope);
                                    SymbolTableEntry * tmp;

                                    if(value == 0){
                                        if(currentScope == 0){
                                            if(currscopespace() == programvar)
                                                globalsSum++;
                                                tmp = insert($2, currentScope, lineno, NULL, var_s);
                                        }                                            
                                        else
                                            tmp = insert($2, currentScope, lineno, NULL, var_s);

                                        tmp->space = currscopespace();
                                        tmp->offset = currscopeoffset();
                                        incurrscopeoffset();
                                    }
                                    else if(value == -1){
                                        printf("Error at line %d: collision with library function.\n", lineno);
                                    }
                                    tmp->type = var_s;
                                    $<expression>$ = lvalue_expr(tmp);
                                    //printf("lvalue -> LOCAL ID \n");
                                }
            | DCOLON ID           {
                                    int value = lookUp(0, $2, currentScope);

                                    if(value == 0){
                                        printf("Error at line %d: There is no variable on global scope with id \"%s\".\n", lineno, $<stringValue>2);
                                    }
                                    $<expression>$ = $<expression>2;
                                    //printf("lvalue -> DCOLON ID \n");

                                }
            | member            {$<expression>$ = $<expression>1; isMember = 1;}
            ;

member:     lvalue DOT ID           {$<expression>$ = member_item($<expression>1, $<stringValue>3);}
            | lvalue LBR expr RBR   {

                                        $<expression>1 = emit_iftableitem($<expression>1);
                                        $<expression>$ = newexpr(tableitem_e);
                                        $<expression>$->sym = $<expression>1->sym;
                                        $<expression>$->index = $<expression>3;
                                    }
            | call DOT ID           {//printf("member -> call DOT ID \n");
            }
            | call LBR expr RBR     {//printf("member -> call LBR expr RBR \n");
            }
            ;

call:       call LPAR elist RPAR    {
                                        //printf("call -> call LPAR elist RPAR \n");
                                        $<expression>$ = make_call($<expression>1, $<expression>3);
                                    }
            | lvalue callsuffix     {//printf("call -> lvalue callsuffix \n");
                                        if($2->method==1){
                                            expr* self = $1;
                                            $1 = emit_iftableitem(member_item(self,$<calls>2->name));
                                            $<calls>2->elist=insertToExprList($<calls>2->elist,self);
                                        }

                                        $<expression>$ = make_call($<expression>1, $<calls>2->elist);
                                    }
            | LPAR funcdef RPAR LPAR elist RPAR {
                                                    //printf("call -> LPAR funcdef RPAR LPAR elist RPAR \n");
                                                    expr * func = newexpr(programfunc_e);
                                                    func->sym = $<symbol>2;
                                                    $<expression>$ = make_call(func, $<expression>5);
                                                }
            ;

callsuffix: normcall                {//printf("callsuffix -> normcall \n"); $$ = $1;
}
            | methodcall            {//printf("callsuffix -> methodcall \n"); $$ = $1;
            }
            ;

normcall:   LPAR elist RPAR         {
                                        //printf("normcall -> LPAR elist RPAR \n");
                                        $<calls>$ = malloc(sizeof(callstr));
                                        $<calls>$->elist = $2;
                                        $<calls>$->method = 0;
                                        $<calls>$->name = NULL;
                                    }
            ;

methodcall: DDOT ID LPAR elist RPAR {
                                        //printf("methodcall -> DDOT ID LPAR elist RPAR \n");
                                        $<calls>$ = malloc(sizeof(callstr));
                                        $<calls>$->elist = $4;
                                        $<calls>$->method = 1;
                                        $<calls>$->name = strdup($<stringValue>2);
                                    }
            ;

elist:      expr                    {//printf("elist -> expr \n"); $<expression>$ = insertToExprList(NULL, $<expression>1);
}
            | elist COMMA expr      {//printf("elist -> elist COMMA expr \n");
                                        $<expression>$ = insertToExprList($<expression>1, $<expression>3);
                                    }
            |                       {//printf("elist -> empty \n"); $$ = NULL;
            }
            ;

objectdef:  LBR elist RBR           {
                                        //printf("objectdef -> LBR elist RBR \n");

                                        expr * t = newexpr(newtable_e);
                                        t->sym = newtemp();
                                        emit(tablecreate, t, NULL, NULL, -1, lineno);

                                        int i = 0;
                                        expr * tmp = NULL;
                                        tmp = $<expression>2;

                                        if(currscopespace() == programvar)
                                            globalsSum++;

                                        while(tmp != NULL){

                                            emit(tablesetelem, t, newexpr_constnum(i++), tmp, -1, lineno);
                                            tmp = tmp->next;
                                        }


                                        $<expression>$ = t;
                                    }
            | LBR indexed RBR       {
                                        //printf("objectdef -> LBR indexed RBR \n");


                                        expr * t = newexpr(newtable_e);
                                        t->sym = newtemp();
                                        emit(tablecreate, t, NULL, NULL, -1, lineno);

                                        if(currscopespace() == programvar)
                                            globalsSum++;

                                        int i;
                                        expr * tmp = NULL;
                                        tmp = $<expression>2;
                                        while(tmp != NULL){
                                            emit(tablesetelem, t, tmp, tmp->index, -1, lineno);

                                            tmp = tmp->next;
                                        }
                                        $<expression>$ = t;
                                    }
            ;

indexed:    indexedelem                 {//printf("indexed -> indexedelem \n");
                                            $<expression>$ = $<expression>1;
                                        }
            | indexed COMMA indexedelem {//printf("indexed -> indexed COMMA indexedelem \n");
                                            $<expression>$ = $<expression>1;
                                            $<expression>$->next = $<expression>3;
                                        }
            ;

indexedelem: LCURBR expr COLON expr RCURBR  {
                                                //printf("indexedelem -> LCURBR expr COLON expr RCURBR \n");

                                                $<expression>$ = $<expression>2;
                                                $<expression>$->index = $<expression>4;
                                            }
             ;

blocknext:  stmt blocknext          {//printf("blocknext -> stmt blocknext \n");
}
            |                       {//printf("blocknext -> empty \n");
            }
            ;

lcbrcode:   LCURBR  {
                        currentScope++;
                        if(maxScope < currentScope){
                            maxScope = currentScope;
                        }

                    }
            ;

block:      lcbrcode blocknext RCURBR {
                                        hide(currentScope);
                                        currentScope--;

                                        //printf("block -> LCURBR blocknext RCURBR \n");
                                     }
            ;


funcname:   ID      {
                        $<stringValue>$ = $<stringValue>1;
                    }
            |       {

                        anonFuncCounter++;

                        char name[10];
                        sprintf(name, "$f%d", anonFuncCounter);

                        insert(name, currentScope, lineno, idlistarray, programfunc_s);

                        $<stringValue>$ = strdup(name);
                    }
            ;


funcprefix: FUNCTION funcname   {
                                    insideFunction++;

                                    $<symbol>$ = lookUpSymbol(currentScope, $<stringValue>2, currentScope);
                                    if($<symbol>$ == NULL){
                                        $<symbol>$ = insert($<stringValue>2, currentScope, lineno, NULL, programfunc_s);
                                    }
                                    $<symbol>$->iaddress = nextquadlabel();

                                    expr * tmp;
                                    tmp = lvalue_expr($<symbol>$);



                                    emit(jump, NULL, NULL, NULL, 0, lineno);



                                    emit(funcstart, tmp, NULL, NULL, -1, lineno);

                                    push(scopeoffsetStack, currscopeoffset());

                                    enterscopespace();
                                    resetformalargsoffset();
                                }
            ;

funcargs:   LPAR idlist RPAR    {
                                    enterscopespace();
                                    resetfunctionlocalsoffset();
                                }
            ;

funcbody:   block               {
                                    $<intValue>$ = currscopeoffset();
                                    exitscopespace();
                                }
            ;

funcblockstart:                 {
                                    push_loop(loopcounterstack, loopcounter);
                                    loopcounter = 0;

                                }
            ;

funcblockend:                   {
                                    loopcounter = pop_and_top_loop(loopcounterstack);
                                }
            ;

funcdef:    funcprefix funcargs funcblockstart funcbody funcblockend    {
                                                                            exitscopespace();
                                                                            $<symbol>1->totalLocals = $<intValue>3;
                                                                            int offset;
                                                                            offset = pop_and_top(scopeoffsetStack);
                                                                            restorecurrscopeoffset(offset);
                                                                            $<symbol>$ = $<symbol>1;

                                                                            expr * tmp;
                                                                            tmp = lvalue_expr($<symbol>1);
                                                                            emit(funcend, tmp, NULL, NULL, -1, lineno);


                                                                            patchlabel($<symbol>$->iaddress, nextquad());

                                                                            firstFunction = 0;
                                                                            secondFunction = 0;

                                                                        }
            ;

const:      INTEGER                 {//printf("const -> INTEGER \n");

                                        $<expression>$ = newexpr_constint($1);
                                    }
            | REAL                  {//printf("const -> REAL \n");

                                        $<expression>$ = newexpr_constnum($1);
                                    }
            | STRING                {
                                        //printf("const -> STRING \n");
                                        $<expression>$ = newexpr_conststring($1);
                                    }
            | NIL                   {//printf("const -> NIL \n");
                                        $<expression>$ = newexpr(nil_e);
                                    }
            | TRUE                  {//printf("const -> TRUE \n");
                                        $<expression>$ = newexpr_constbool(1);
                                    }
            | FALSE                 {//printf("const -> FALSE \n");
                                        $<expression>$ = newexpr_constbool(0);
                                    }
            ;

idlistnext:  COMMA idlist           {//printf("idlistnext -> COMMA idlist \n");
}
            |                       {//printf("idlistnext -> empty \n");
            }
            ;

idlist:     ID idlistnext           {
                                        if(idlistarray == NULL){
                                            idlistarray = malloc(strlen($<stringValue>1) + 1);
                                            strcpy(idlistarray, $<stringValue>1);
                                        }
                                        else {

                                            tmptok = malloc(strlen(idlistarray) + strlen($<stringValue>1) + 1);

                                            strcpy(tmptok, $<stringValue>1);
                                            strcat(tmptok, ",");
                                            strcat(tmptok, idlistarray);

                                            idlistarray = tmptok;
                                        }

                                        //printf("idlist -> ID idlistnext \n");
                                    }
            |                       {//printf("idlist -> empty \n");
            }
            ;

ifprefix:   IF LPAR expr RPAR       {
                                        emit(if_eq, $<expression>3, newexpr_constbool(1), NULL, nextquad() + 2, lineno);

                                        $<unsignedvalue>$ = nextquad();
                                        emit(jump, NULL, NULL, NULL, 0, lineno);
                                    }
            ;

elseprefix: ELSE                    {
                                        $<unsignedvalue>$ = nextquad();
                                        emit(jump, NULL, NULL, NULL, 0, lineno);
                                    }
            ;

ifstmt:     ifprefix stmt                   {
                                                patchlabel($<unsignedvalue>1, nextquad());
                                            }
            | ifprefix stmt elseprefix stmt {
                                                patchlabel($<unsignedvalue>1, $<unsignedvalue>3 + 1);
                                                patchlabel($<unsignedvalue>3, nextquad());
                                            }
            ;

whilestart: WHILE           {
                                breakflag++;
                                continueflag++;
                                $<unsignedvalue>$ = nextquad();
                            }
            ;

whilecond:  LPAR expr RPAR  {
                                emit(if_eq, $<expression>2, newexpr_constbool(1), NULL, nextquad() + 2, lineno);
                                $<unsignedvalue>$ = nextquad();
                                emit(jump, NULL, NULL, NULL, 0, lineno);
                            }
            ;

loopstart:  {++loopcounter;}
            ;

loopend:    {--loopcounter;}
            ;

loopstmt:   loopstart stmt loopend  {
                                        $<forstmt_t>$ = malloc(sizeof(stmt));
                                        $<forstmt_t>2 = malloc(sizeof(stmt));

                                        $<forstmt_t>$ = $<forstmt_t>2;
                                    }
            ;

whilestmt:  whilestart whilecond loopstmt   {
                                            emit(jump, NULL, NULL, NULL, $<unsignedvalue>1, lineno);
                                            patchlabel($<unsignedvalue>2, nextquad());


                                            patchlist(s.breakList, nextquad());
                                            patchlist(s.contList, $<unsignedvalue>1);

                                            s.breakList = 0;
                                            s.contList = 0;

                                            breakflag--;
                                            continueflag--;
                                        }
            ;

N:          {
                $<unsignedvalue>$ = nextquad();
                emit(jump, NULL, NULL, NULL, 0, lineno);
            }
            ;

M:          {
                $<unsignedvalue>$ = nextquad();
            }
            ;

forkeyword: FOR     {
                        breakflag = 1; continueflag = 1;
                    }
            ;

forprefix:  forkeyword LPAR elist SEMIC M expr SEMIC    {
                                                            $<forstct>$ = malloc(sizeof(forstruct));
                                                            $<forstct>$->test = $<unsignedvalue>M;
                                                            $<forstct>$->enter = nextquad();
                                                            emit(if_eq, $<expression>6, newexpr_constbool(1), NULL, 0, lineno);
                                                        }
            ;

forstmt:    forprefix N elist RPAR N loopstmt N         {
                                                            breakflag = 0; continueflag = 0;

                                                            patchlabel($<forstct>1->enter, $<unsignedvalue>5 + 1);
                                                            patchlabel($<unsignedvalue>2, nextquad());
                                                            patchlabel($<unsignedvalue>5, $<forstct>1->test);
                                                            patchlabel($<unsignedvalue>7, $<unsignedvalue>2);

                                                            patchlist($<forstmt_t>6->breakList, nextquad());
                                                            patchlist($<forstmt_t>6->contList, $<unsignedvalue>2 + 1);
                                                        }
            ;

returnstmt: RETURN SEMIC                        {
                                                    if(insideFunction == 0)
                                                        printf("Error at line %d: use of \"return\" while not in a function\n", lineno);

                                                    emit(ret, NULL, NULL, NULL, -1, lineno);
                                                    //printf("returnstmt -> RETURN SEMIC \n");
                                                }
            | RETURN expr SEMIC                 {
                                                    if(insideFunction == 0)
                                                        printf("Error at line %d: use of \"return\" while not in a function\n", lineno);

                                                    emit(ret, $<expression>2, NULL, NULL, -1, lineno);
                                                    //printf("return -> RETURN expr SEMIC \n");
                                                }
            ;

%%

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
