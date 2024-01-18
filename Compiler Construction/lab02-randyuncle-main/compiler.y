/* Please feel free to modify any content */

/* Definition section */
%{
    #include "compiler_common.h" //Extern variables that communicate with lex
    // #define YYDEBUG 1
    // int yydebug = 1;

    extern int yylineno;
    extern int yylex();
    extern FILE *yyin;

    int yylex_destroy ();
    void yyerror (char const *s)
    {
        printf("error:%d: %s\n", yylineno, s);
    }

    extern int yylineno;
    extern int yylex();
    extern FILE *yyin;

    /* Symbol table function - you can add new functions if needed. */
    /* parameters and return type can be changed */
    static void create_symbol();
    static void insert_symbol();
    static char* lookup_symbol();
    static void dump_symbol();
    void type_check(char* id1, char* id2, char* id_main, char* op);

    /* Global variables */
    bool HAS_ERROR = false;

    /* The symbol table linked list */
    typedef struct symbol_table* symbolpointer;
    typedef struct symbol_table {
        int index;
        char* name;
        int mut;
        char type[10];
        char array_type[10];
        int Addr;
        int Lineno;
        char* Func_sig;
        symbolpointer next;
    };

    /* The stack for the symbol table */
    typedef struct stack* stackpointer;
    typedef struct stack {
        int size;
        symbolpointer table_front;
        symbolpointer table_rear;
        stackpointer next;
        stackpointer prev;
    };
    stackpointer front, rear;
    symbolpointer curr;
    int stack_size = -1;
    int address = -1;

    int is_array = 0;
    char print_type[100];
%}
//%define parse.error verbose
%error-verbose

/* Use variable or self-defined structure to represent
 * nonterminal and token type
 *  - you can add new fields if needed.
 */
%union {
    int i_val;
    float f_val;
    char *s_val;
    /* ... */
    char *type;
    char *name;
    char *id;
}

/* Token without return */
%token LET MUT NEWLINE
//%token INT FLOAT BOOL STR
%token TRUE FALSE
%token GEQ LEQ EQL NEQ LOR LAND
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN REM_ASSIGN
%token IF ELSE FOR WHILE LOOP
%token PRINT PRINTLN
%token FUNC RETURN BREAK
%token ARROW AS IN DOTDOT RSHIFT LSHIFT

/* Token with return, which need to sepcify type */
%token <i_val> INT_LIT
%token <f_val> FLOAT_LIT
%token <s_val> STRING_LIT ID
%token <type> INT FLOAT BOOL STR

/* Nonterminal with return, which need to sepcify type */
%type <type> Type Literal AssignOperator ReturnAnnotation ArrayElementOp
%type <type> LogicalOrExpression LogicalAndExpression EqualityExpression RelationalExpression ShiftExpression AdditiveExpression MultiplicativeExpression UnaryExpression PrimaryExpression AsExpression
%type <type> ExpressionStatement StatementList Statement FuncNameDeclStmt

/* Yacc will start at this nonterminal */
%start Program

/* Grammar section */
%%

Program
    : GlobalStatementList
;

GlobalStatementList 
    : GlobalStatementList GlobalStatement
    | GlobalStatement
;

GlobalStatement
    : FunctionDeclStmt
    | Statement
    | NEWLINE 
;

FunctionDeclStmt
    : FuncNameDeclStmt '(' ParameterList ')' ReturnAnnotation FuncStartStmt
        { 
            strcat(print_type, ")");
            if ($5) 
            {
                if (!strcmp($5, "i32"))
                {
                    strcat(print_type, "I");
                    printf("ireturn\n");
                }
                else if (!strcmp($5, "f32"))
                {
                    strcat(print_type, "F");
                    printf("freturn\n");
                }
                else if (!strcmp($5, "bool"))
                {
                    strcat(print_type, "B");
                    printf("breturn\n");
                }
                else if (!strcmp($5, "str"))
                {
                    strcat(print_type, "S");
                    printf("sreturn\n");
                }
            }

            if (strcmp(print_type, "()")) 
            {
                char* out = lookup_symbol($<type>1, "update func_sig");
            }

            dump_symbol(); 
        } 
;

FuncNameDeclStmt
    : FUNC ID
        { 
            $$ = $<s_val>2;
            address = -1;
            strcpy(print_type, "(");
            printf("func: %s\n",  $<s_val>2); 
            insert_symbol($<s_val>2, -1, "func", yylineno + 1, "(V)V");
            create_symbol(); 
        } 
;

ParameterList 
    : Parameter
    | ParameterList ',' Parameter
;

Parameter
    : /* Empty */
    | ID ':' Type
        {
            insert_symbol($<s_val>1, 0, $3, yylineno + 1, "-");
            if (!strcmp($3, "i32"))
                strcat(print_type, "I");
            else if (!strcmp($3, "f32"))
                strcat(print_type, "F");
            else if (!strcmp($3, "bool"))
                strcat(print_type, "B");
            else if (!strcmp($3, "str"))
                strcat(print_type, "S");
        }
    | UnaryExpression
;

ReturnAnnotation
    : /* Empty */ { $$ = NULL; }
    | ARROW Type { $$ = $2; }
;

FuncStartStmt
    : '{'
      StatementList 
      '}'
;

CompoundStatement
    : '{' { create_symbol(); }
      StatementList 
      '}' { dump_symbol(); }
;

StatementList
    : StatementList Statement
    | Statement
;

Statement
    : LetStatement
    | CompoundStatement
    | ExpressionStatement
    | SelectionStatement
    | IterationStatement
    | JumpStatement
    | ';'
    | NEWLINE
;

LetStatement
    : LET ID LetAssignOperation Statement { insert_symbol($2, 0, $4, yylineno + 1, "-"); }
    | LET ID ':' AddressOperation Type LetAssignOperation Statement 
        { 
            insert_symbol($<s_val>2, 0, $5, yylineno + 1, "-"); 
        }
        
    | LET ID ':' '[' Type ';' PrimaryExpression ']' LetAssignOperation Statement 
        { 
            insert_symbol($<s_val>2, 0, "array", yylineno + 1, "-"); 
            strcpy(front->table_front->array_type, $<type>5);
        }
        
    | LET MUT ID LetAssignOperation Statement { insert_symbol($3, 1, $5, yylineno + 1, "-"); }
    | LET MUT ID ':' AddressOperation Type LetAssignOperation Statement 
        { 
            insert_symbol($3, 1, $6, yylineno + 1, "-"); 
        }
;

LetAssignOperation
    : '='
    |

AddressOperation
    : '&'
    |
;

ExpressionStatement
    : AssignExpression
    | PrintStmt
    | ArrayExpression
    | FunctionCallStmt
;

SelectionStatement
    : IF ConditionStmt Statement
    | IF ConditionStmt Statement ELSE Statement
;

IterationStatement
    : ForeachStatement StatementList '}'
        {
            dump_symbol();
        }
    | WHILE ConditionStmt Statement
    | LOOP Statement
;

ForeachStatement
    : FOR ID IN ArrayElementOp '{'
        {
            create_symbol();
            insert_symbol($<s_val>2, 0, $<type>4, yylineno + 1, "-");
        }
;

ArrayElementOp
    : ID 
        {
            $$ = lookup_symbol($<s_val>1, "array_type_find");
        }
;

ConditionStmt
    : ExpressionStatement
;

JumpStatement
    : RETURN ExpressionStatement
        {
            if (!strcmp($2, "i32"))
                printf("ireturn\n");
            else if (!strcmp($2, "f32"))
                printf("freturn\n");
            else if (!strcmp($2, "bool"))
                printf("breturn\n");
            else if (!strcmp($2, "str"))
                printf("sreturn\n");
        }
    | BREAK Statement
    | BREAK
;

FunctionCallStmt
    : ID '(' ParameterList ')' 
        { 
            char* out = lookup_symbol($<s_val>1, "function_call");
        }
;

ArrayExpression
    : AddressOperation PrimaryExpression '[' SliceExpression ']'
    | PrimaryExpression '[' SliceExpression ']'
    | '[' ElementList ']'
;

ElementList
    : ElementList ',' PrimaryExpression
    | PrimaryExpression
;

SliceExpression
    : SliceExpression SliceOperation
    | SliceOperation
;

SliceOperation
    : DOTDOT { printf("DOTDOT\n"); }
    | PrimaryExpression
;

AssignExpression
    : ID AssignOperator AssignExpression
    { 
        char* id = lookup_symbol($<s_val>1, "variable_find_no_re");
        if (strcmp($<type>2, "ASSIGN")) 
        {
            type_check(id, $<type>3, $<type>2, "ASSIGNMENT");
        }

        if (id)
            printf("%s\n", $<type>2);
    }
    | LogicalOrExpression
;

LogicalOrExpression 
    : LogicalAndExpression
    | LogicalOrExpression LOR LogicalAndExpression { $$ = "bool"; printf("LOR\n"); }
    ;

LogicalAndExpression
    : EqualityExpression
    | LogicalAndExpression LAND EqualityExpression { $$ = "bool"; printf("LAND\n"); }
    ;

EqualityExpression
    : RelationalExpression { $$ = $1; }
    | EqualityExpression EQL RelationalExpression 
        { 
            $$ = "bool"; 
            type_check($<type>1, $<type>3, "EQL", "ASSIGNMENT");
            printf("EQL\n");     
        }
    | EqualityExpression NEQ RelationalExpression 
        { 
            $$ = "bool"; 
            type_check($<type>1, $<type>3, "NEQ", "ASSIGNMENT");
            printf("NEQ\n"); 
        }
    ;

RelationalExpression
    : ShiftExpression { $$ = $1; }
    | RelationalExpression '<' ShiftExpression 
        { 
            $$ = "bool"; 
            type_check($<type>1, $<type>3, "LSS", "COMPARE");
            printf("LSS\n"); 
        }
    | RelationalExpression '>' ShiftExpression 
        { 
            $$ = "bool"; 
            type_check($1, $3, "GTR", "COMPARE");
            printf("GTR\n"); 
        }
    | RelationalExpression GEQ ShiftExpression 
        { 
            $$ = "bool"; 
            type_check($<type>1, $<type>3, "GEQ", "COMPARE");
            printf("GEQ\n"); 
        }
    | RelationalExpression LEQ ShiftExpression 
        { 
            $$ = "bool"; 
            type_check($<type>1, $<type>3, "LEQ", "COMPARE");
            printf("LEQ\n"); 
        }
    ;

ShiftExpression
    : AdditiveExpression { $$ = $1; }
    | ShiftExpression LSHIFT AdditiveExpression 
        { 
            type_check($<type>1, $<type>3, "LSHIFT", "ARITHMETIC"); 
            printf("LSHIFT\n");
        }
    | ShiftExpression RSHIFT AdditiveExpression 
        {
            type_check($<type>1, $<type>3, "RSHIFT", "ARITHMETIC"); 
            printf("RSHIFT\n"); 
        }
;

AdditiveExpression
    : MultiplicativeExpression { $$ = $1; }
    | AdditiveExpression '+' MultiplicativeExpression 
        { 
            type_check($1, $3, "ADD", "ARITHMETIC"); 
            printf("ADD\n"); 
        }
    | AdditiveExpression '-' MultiplicativeExpression 
        {
            type_check($<type>1, $<type>3, "SUB", "ARITHMETIC");  
            printf("SUB\n"); 
        }
    ;

MultiplicativeExpression
    : UnaryExpression { $$ = $1; }
    | MultiplicativeExpression '*' UnaryExpression 
        { 
            type_check($<type>1, $<type>3, "MUL", "ARITHMETIC"); 
            printf("MUL\n"); 
        }
    | MultiplicativeExpression '/' UnaryExpression 
        { 
            type_check($<type>1, $<type>3, "DIV", "ARITHMETIC"); 
            printf("DIV\n"); 
        }
    | MultiplicativeExpression '%' UnaryExpression 
        { 
            type_check($<type>1, $<type>3, "REM", "ARITHMETIC"); 
            printf("REM\n"); 
        }
    ;

UnaryExpression
    : PrimaryExpression AsExpression 
        {
            $$ = $1;
            if ($2)
            {
                $$ = $2;
                if (!strcmp($1, "i32") && !strcmp($2, "f32"))
                    printf("i2f\n");
                else
                    printf("f2i\n");
            }
        }
    | '-' UnaryExpression { $$ = $2; printf("NEG\n"); }
    | '!' UnaryExpression { $$ = $2; printf("NOT\n"); }
    ;

PrimaryExpression
    : Literal { $$ = $1;}
    | ID
    {
        $$ = lookup_symbol($1, "variable_find");
    }
    | '(' ExpressionStatement ')' { $$ = $2; }
    ;

AsExpression
    : AS Type { $$ = $2; }
    | { $$ = NULL; }
;

PrintStmt
    : PRINT '(' ExpressionStatement ')'
        { printf("PRINT %s\n", $<type>3); }
    | PRINTLN '(' ExpressionStatement ')'
        { printf("PRINTLN %s\n", $<type>3); }
;

AssignOperator
    : '=' { $$ = "ASSIGN"; }
    | ADD_ASSIGN { $$ = "ADD_ASSIGN"; }
    | SUB_ASSIGN { $$ = "SUB_ASSIGN"; }
    | DIV_ASSIGN { $$ = "DIV_ASSIGN"; }
    | MUL_ASSIGN { $$ = "MUL_ASSIGN"; }
    | REM_ASSIGN { $$ = "REM_ASSIGN"; }
;

Type
    : INT { $$ = "i32"; }
    | FLOAT { $$ = "f32"; }
    | STR { $$ = "str"; }
    | BOOL { $$ = "bool"; }
;

Literal
    : INT_LIT { $$ = "i32"; printf("INT_LIT %d\n", $<i_val>1); }
    | FLOAT_LIT {  $$ = "f32"; printf("FLOAT_LIT %.6f\n", $<f_val>1); }
    | TRUE { $$ = "bool"; printf("bool TRUE\n"); }
    | FALSE { $$ = "bool"; printf("bool FALSE\n"); }
    | '"' STRING_LIT '"' { $$ = "str"; printf("STRING_LIT \"%s\"\n", $<s_val>2); }
    | '"' '"'  { $$ = "str"; printf("STRING_LIT \"\"\n"); }
;

%%

/* C code section */
int main(int argc, char *argv[])
{
    if (argc == 2) {
        yyin = fopen(argv[1], "r");
    } else {
        yyin = stdin;
    }

    yylineno = 0;
    create_symbol(); 
    yyparse();
    dump_symbol();

	printf("Total lines: %d\n", yylineno);
    fclose(yyin);
    return 0;
}

static void create_symbol() {
    stackpointer node = (stackpointer)malloc(sizeof(struct stack));
    node->size = 0;
    node->table_front = NULL;
    node->table_rear = NULL;
    node->next = NULL;
    node->prev = NULL;

    if (!front) 
    {
        front = node;
        rear = front;
    } else 
    {
        front->next = node;
        front->next->prev = front;
        front = front->next;
    }

    printf("> Create symbol table (scope level %d)\n", ++stack_size);
}

static void insert_symbol(char* name, int mut, char* type, int lineno, char* func_sig) {
    /**
    * having problems with (char*) and mut
    */
    symbolpointer node = (symbolpointer)malloc(sizeof(struct symbol_table));
    node->index = front->size++;
    node->name = (char*)malloc(sizeof(name));
    strcpy(node->name, name);
    node->mut = mut;
    //node->type = (char*)malloc(sizeof(char) * sizeof(type));
    strcpy(node->type, type);
    node->Addr = address;
    node->Lineno = lineno;
    node->Func_sig = (char*)malloc(sizeof(char) * sizeof(func_sig));
    strcpy(node->Func_sig, func_sig);
    node->next = NULL;

    if (!front->table_front) 
    {
        front->table_front = node;
        front->table_rear = node;
    } else 
    {
        front->table_front->next = node;
        front->table_front = front->table_front->next;
    }

    curr = node;
    printf("> Insert `%s` (addr: %d) to scope level %d\n", node->name, address++, stack_size);
}

static char* lookup_symbol(char* id, char* command) {
    // For return function ?
    int is_find = 0;
    for (stackpointer p = front ; p != NULL && !is_find ; p = p->prev) 
    {
        for (symbolpointer find = p->table_rear ; find != NULL ; find = find->next)
        {
            if (!strcmp(find->name, id)) 
            {
                if (!strcmp(command, "function_call")) 
                {
                    printf("call: %s%s\n", id, find->Func_sig);
                    return NULL;
                } 
                else if(!strcmp(command, "update func_sig"))
                {
                    strcpy(find->Func_sig, print_type);
                    return NULL;
                }
                else if (!strcmp(command, "variable_find")) 
                {
                    printf("IDENT (name=%s, address=%d)\n", id, find->Addr);
                    return find->type;
                } 
                else if (!strcmp(command, "variable_find_no_re")) 
                {
                    return find->type;
                }
                else if (!strcmp(command, "array_type_find"))
                {
                    printf("IDENT (name=%s, address=%d)\n", id, find->Addr);
                    return find->array_type;
                }
                is_find = 1;
                break;
            }
        }
    }

    if (!is_find)
    {
		printf("error:%d: undefined: %s\n", yylineno+1, id);
    }

    return NULL;
}

static void dump_symbol() {
    printf("\n> Dump symbol table (scope level: %d)\n", stack_size--);
    printf("%-10s%-10s%-10s%-10s%-10s%-10s%-10s\n",
        "Index", "Name", "Mut","Type", "Addr", "Lineno", "Func_sig");
    
    for (symbolpointer p = front->table_rear ; p ; p = p->next) 
    {
        printf("%-10d%-10s%-10d%-10s%-10d%-10d%-10s\n",
                p->index, p->name, p->mut, p->type, p->Addr, p->Lineno, p->Func_sig);
    }

    if (front->prev) 
    {
        //stackpointer temp = front;
        front = front->prev;
    } else 
    {
        front = NULL;
    }
}

void type_check(char* id1, char* id2, char* id_main, char* op) {
    if (!id1 || !id2) 
    {
        if (!id1 && !id2)
            printf("error:%d: invalid operation: %s (mismatched types undefined and undefined)\n", yylineno + 1, id_main);
        else if(!id1)
            printf("error:%d: invalid operation: %s (mismatched types undefined and %s)\n", yylineno + 1, id_main, id2);
        else
            printf("error:%d: invalid operation: %s (mismatched types %s and undefined)\n", yylineno + 1, id_main, id1);
    }
    else if (!strcmp(op, "COMPARE"))
    {
        if ((!strcmp(id1, "str") && strcmp(id2, "str")) || (strcmp(id1, "str") && !strcmp(id2, "str")))
        {
            printf("error:%d: invalid operation: %s (mismatched types %s and %s)\n", yylineno + 1, id_main, id1, id2);
        }
    }
    else 
    {
        if (!strcmp(id_main, "REM"))
        {
            if (!strcmp(id1, "f32") || !strcmp(id2, "f32"))
            {
                printf("error:%d: invalid operation: %s (mismatched types %s and %s)\n", yylineno + 1, id_main, id1, id2);
            }
        }
        else if (strcmp(id1, id2))
        {
            printf("error:%d: invalid operation: %s (mismatched types %s and %s)\n", yylineno + 1, id_main, id1, id2);
        }
    }
}