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

    /* Used to generate code */
    /* As printf; the usage: CODEGEN("%d - %s\n", 100, "Hello world"); */
    /* We do not enforce the use of this macro */
    #define CODEGEN(...) \
        do { \
            for (int i = 0; i < g_indent_cnt; i++) { \
                fprintf(fout, "\t"); \
            } \
            fprintf(fout, __VA_ARGS__); \
        } while (0)

    /* Symbol table function - you can add new functions if needed. */
    /* parameters and return type can be changed */
    static void create_symbol();
    static void insert_symbol();
    static char* lookup_symbol(char* id, char* command);
    static void dump_symbol();
    void type_check(char* id1, char* id2, char* id_main, char* op);

    /* Global variables */
    bool g_has_error = false;
    FILE *fout = NULL;
    int g_indent_cnt = 0;

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
    symbolpointer look_up_symbol(char* id, char* command);
    int stack_size = -1;
    int address = -1;

    int is_array = 0;
    int init_array = 0;
    char print_type[100];
    char print_slice[100];

    int cond_count = 0;
    int local_cond_count = 0;
    int loop_count = 0;
    int local_loop_count = 0;
    int jump_count = 0;
%}

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
%type <type> Type Literal AssignOperator ReturnAnnotation ArrayElementOp CompareOperator SliceOperation
%type <type> LogicalOrExpression LogicalAndExpression CompareExpression ArrayExpression SliceExpression ShiftExpression AdditiveExpression MultiplicativeExpression UnaryExpression PrimaryExpression AsExpression
%type <type> ExpressionStatement StatementList Statement FunctionDeclStmt FuncNameDeclStmt FuncStartStmt

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
    : FunctionDeclStmt FuncStartStmt 
        { 
            if ($1) 
            {
                if (!strcmp($1, "i32"))
                {
                    printf("ireturn\n");
                    CODEGEN("ireturn\n");
                }
                else if (!strcmp($1, "f32"))
                {
                    printf("freturn\n");
                    CODEGEN("freturn\n");
                }
                else if (!strcmp($1, "bool"))
                {
                    printf("breturn\n");
                    CODEGEN("ireturn\n");
                }
                else if (!strcmp($1, "str"))
                {
                    printf("sreturn\n");
                    CODEGEN("areturn\n");
                }
            }
            else 
                CODEGEN("return\n");
            

            g_indent_cnt--;
            CODEGEN(".end method\n");

            dump_symbol(); 
        }
    | Statement
    | NEWLINE 
;

FunctionDeclStmt
    : FuncNameDeclStmt '(' ParameterList ')' ReturnAnnotation
        { 
            $$ = $5;
            strcat(print_type, ")");
            if ($5) 
            {
                if (!strcmp($5, "i32"))
                {
                    strcat(print_type, "I");
                }
                else if (!strcmp($5, "f32"))
                {
                    strcat(print_type, "F");
                }
                else if (!strcmp($5, "bool"))
                {
                    strcat(print_type, "I");
                }
                else if (!strcmp($5, "str"))
                {
                    strcat(print_type, "S");
                }
            }

            char* out = (char*)malloc(sizeof(char) * 20);
            strcpy(out, "()V");
            if (strcmp(print_type, "()")) 
            {
                char* signal = lookup_symbol($<type>1, "update func_sig");
                strcpy(out, print_type);
            }

            if (strcmp($1, "main"))
            {
                CODEGEN(".method public static %s%s ; Define %s function\n", $1, out, $1);
                CODEGEN(".limit stack 20\n");
                CODEGEN(".limit locals 20\n");
                g_indent_cnt++;
            }
            else 
            {
                CODEGEN(".method public static main([Ljava/lang/String;)V\n");
                CODEGEN(".limit stack 100\n");
                CODEGEN(".limit locals 100\n");
                g_indent_cnt++;
            }
        } 
;

FuncNameDeclStmt
    : FUNC ID
        { 
            $$ = $<s_val>2;
            address = -1;
            strcpy(print_type, "(");
            printf("func: %s\n",  $<s_val>2); 
            insert_symbol($<s_val>2, -1, "func", yylineno + 1, "()V");
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
                strcat(print_type, "I");
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
    | ';' { $$ = NULL; }
    | NEWLINE
;

LetStatement
    : LET ID LetAssignOperation Statement 
        { 
            insert_symbol($2, 0, $4, yylineno + 1, "-"); 
            if (!strcmp(curr->type, "i32") || !strcmp(curr->type, "bool"))
                CODEGEN("istore %d\n", curr->Addr);
            else if (!strcmp(curr->type, "f32"))
                CODEGEN("fstore %d\n", curr->Addr);
            else if (!strcmp(curr->type, "str"))
                CODEGEN("astore %d\n", curr->Addr);
        }
    | LET ID ':' AddressOperation Type LetAssignOperation Statement 
        { 
            insert_symbol($<s_val>2, 0, $5, yylineno + 1, "-"); 
            
            if (!$7) 
            {
                if (!strcmp(curr->type, "i32"))
                    CODEGEN("ldc 0\n");
                else if (!strcmp(curr->type, "bool"))
                    CODEGEN("ldc iconst_0\n"); // not sure the default is true or false
                else if (!strcmp(curr->type, "f32"))
                    CODEGEN("ldc 0.000000\n");
                else if (!strcmp(curr->type, "str"))
                    CODEGEN("ldc \"\"");
            }

            if (!strcmp(curr->type, "i32") || !strcmp(curr->type, "bool"))
                CODEGEN("istore %d\n", curr->Addr);
            else if (!strcmp(curr->type, "f32"))
                CODEGEN("fstore %d\n", curr->Addr);
            else if (!strcmp(curr->type, "str"))
                CODEGEN("astore %d\n", curr->Addr);
        }
        
    | LET ID ':' '[' Type ';' PrimaryExpression ']' LetAssignOperation 
        { 
            insert_symbol($<s_val>2, 0, "array", yylineno + 1, "-"); 
            strcpy(front->table_front->array_type, $<type>5);

            if (!strcmp($<type>5, "i32") || !strcmp($<type>5, "bool"))
                CODEGEN("newarray %s\n", "int");
            else if (!strcmp($<type>5, "f32"))
                CODEGEN("newarray %s\n", "float");
            else if (!strcmp($<type>5, "str"))
                CODEGEN("newarray %s\n", "string");

            CODEGEN("astore %d\n", curr->Addr);
            init_array = 0;
        }
      Statement
        
    | LET MUT ID LetAssignOperation Statement 
        { 
            insert_symbol($3, 1, $5, yylineno + 1, "-");
            if (!strcmp(curr->type, "i32") || !strcmp(curr->type, "bool"))
                CODEGEN("istore %d\n", curr->Addr);
            else if (!strcmp(curr->type, "f32"))
                CODEGEN("fstore %d\n", curr->Addr);
            else if (!strcmp(curr->type, "str"))
                CODEGEN("astore %d\n", curr->Addr);
        }
    | LET MUT ID ':' AddressOperation Type LetAssignOperation Statement 
        { 
            insert_symbol($3, 1, $6, yylineno + 1, "-");

            if (!$8) 
            {
                if (!strcmp(curr->type, "i32"))
                    CODEGEN("ldc 0\n");
                else if (!strcmp(curr->type, "bool"))
                    CODEGEN("ldc iconst_0\n"); // not sure the default is true or false
                else if (!strcmp(curr->type, "f32"))
                    CODEGEN("ldc 0.000000\n");
                else if (!strcmp(curr->type, "str"))
                    CODEGEN("ldc \"\"");
            }

            if (!strcmp(curr->type, "i32") || !strcmp(curr->type, "bool"))
                CODEGEN("istore %d\n", curr->Addr);
            else if (!strcmp(curr->type, "f32"))
                CODEGEN("fstore %d\n", curr->Addr);
            else if (!strcmp(curr->type, "str"))
                CODEGEN("astore %d\n", curr->Addr);
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
    : IfStmt Statement
        {
            CODEGEN("goto L_if_exit_%d\n", cond_count - local_cond_count);
            g_indent_cnt--;
            CODEGEN("L_if_false_%d:\n", cond_count - local_cond_count);
            CODEGEN("L_if_exit_%d:\n", cond_count - local_cond_count);
            g_indent_cnt++;
            local_cond_count++;
        }
    | IfElseStmt Statement
        {
            g_indent_cnt--;
            CODEGEN("L_if_exit_%d:\n", cond_count - local_cond_count);
            g_indent_cnt++;
            local_cond_count++;
        }
;

IfStmt
    : IF ConditionStmt
        {
            local_cond_count = 1;
            CODEGEN("ifeq L_if_false_%d\n", cond_count++);
        }
;

IfElseStmt
    : IfStmt Statement ELSE
        {
            CODEGEN("goto L_if_exit_%d\n", cond_count - local_cond_count);
            g_indent_cnt--;
            CODEGEN("L_if_false_%d:\n", cond_count - local_cond_count);
            g_indent_cnt++;
        }
;

ConditionStmt
    : ExpressionStatement
        {
            // type check
            if (!g_has_error)
            {
                if (strcmp($1, "bool"))
                    printf("error:%d: non-bool (type %s) used as for condition\n", yylineno + 1, $1);
            }
        }
;

IterationStatement
    : ForeachStatement
        {
            dump_symbol();
            CODEGEN("goto L_loop_start_%d\n", loop_count - local_loop_count);
            g_indent_cnt--;
            CODEGEN("L_loop_exit_%d:\n", loop_count - local_loop_count);
            g_indent_cnt++;
            local_loop_count++;
        }
    | WHILE
        {
            local_loop_count = 1;
            g_indent_cnt--;
            CODEGEN("L_loop_start_%d:\n", loop_count++);
            g_indent_cnt++;
        } 
      ConditionStmt 
        {
            CODEGEN("ifeq L_loop_exit_%d\n", loop_count - local_loop_count);
        }
      Statement
        {
            CODEGEN("goto L_loop_start_%d\n", loop_count - local_loop_count);
            g_indent_cnt--;
            CODEGEN("L_loop_exit_%d:\n", loop_count - local_loop_count);
            g_indent_cnt++;
            local_loop_count++;
        }
    | LOOP
        {
            local_loop_count = 1;
            g_indent_cnt--;
            CODEGEN("L_loop_start_%d:\n", loop_count++);
            g_indent_cnt++;
        }
      Statement
        {
            CODEGEN("goto L_loop_start_%d\n", loop_count - local_loop_count);
            g_indent_cnt--;
            CODEGEN("L_loop_exit_%d:\n", loop_count - local_loop_count);
            g_indent_cnt++;
            local_loop_count++;
        }
;

ForeachStatement
    : FOR ID IN ArrayElementOp
        {
            symbolpointer find = look_up_symbol($<s_val>4, "variable_find_no_re");
            create_symbol();
            insert_symbol($<s_val>2, 0, find->array_type, yylineno + 1, "-");

            CODEGEN("arraylength\n");
            CODEGEN("istore %d\n", curr->Addr + 1); // array length
            
            CODEGEN("ldc 1\n");
            CODEGEN("ineg\n");
            CODEGEN("istore %d\n", curr->Addr + 2); // counting

            local_loop_count = 1;
            g_indent_cnt--;
            CODEGEN("L_loop_start_%d:\n", loop_count++);
            g_indent_cnt++;
        }
      '{'
        {
            symbolpointer find = look_up_symbol($<s_val>4, "variable_find_no_re");
            /* make it be like the for loop structure in java */
            CODEGEN("iinc %d 1\n", curr->Addr + 2);
            CODEGEN("iload %d\n", curr->Addr + 1);
            CODEGEN("iload %d\n", curr->Addr + 2);
            CODEGEN("isub\n");

            CODEGEN("ifeq L_loop_exit_%d\n", loop_count - local_loop_count);
            
            CODEGEN("aload %d\n", find->Addr);
            CODEGEN("iload %d\n", curr->Addr + 2);

            if (!strcmp(find->array_type, "i32") || !strcmp(find->array_type, "bool"))
                CODEGEN("iaload\n");
            else if (!strcmp(find->array_type, "f32"))
                CODEGEN("faload\n");
            else if (!strcmp(find->array_type, "str"))
                CODEGEN("aaload\n");

            if (!strcmp(curr->type, "i32") || !strcmp(curr->type, "bool"))
                CODEGEN("istore %d\n", curr->Addr);
            else if (!strcmp(curr->type, "f32"))
                CODEGEN("fstore %d\n", curr->Addr);
            else if (!strcmp(curr->type, "str"))
                CODEGEN("astore %d\n", curr->Addr);
        } 
      StatementList '}'
;

ArrayElementOp
    : ID 
        {
            char* out = lookup_symbol($<s_val>1, "variable_find");
            $$ = $1;
        }
;

JumpStatement
    : RETURN ExpressionStatement
        {
            if (!strcmp($2, "i32"))
            {
                printf("ireturn\n");
                CODEGEN("ireturn\n");
            }
            else if (!strcmp($2, "f32"))
            {
                printf("freturn\n");
                CODEGEN("freturn\n");
            }
            else if (!strcmp($2, "bool"))
            {
                printf("breturn\n");
                CODEGEN("ireturn\n");
            }
            else if (!strcmp($2, "str"))
            {
                printf("sreturn\n");
                CODEGEN("areturn\n");
            }
        }
    | BREAK Statement
        {
            CODEGEN("goto L_loop_exit_%d\n", loop_count - local_loop_count);
        }
    | BREAK
        {
            CODEGEN("goto L_loop_exit_%d\n", loop_count - local_loop_count);
        }
;

FunctionCallStmt
    : ID '(' ParameterList ')' 
        { 
            symbolpointer out = look_up_symbol($<s_val>1, "function_call");
            CODEGEN("invokestatic Main/%s%s ; invoke `%s` method in `Main` class\n", out->name, out->Func_sig, out->name);
        }
;

ArrayExpression
    : AddressOperation PrimaryExpression
        {
            strcpy(print_slice, "");
        }
      '[' SliceExpression ']'
        {
            CODEGEN("invokevirtual java/lang/String.substring(%s)Ljava/lang/String;\n", print_slice);
        }
    | PrimaryExpression '[' Literal ']' 
        { 
            if (!strcmp($1, "i32") || !strcmp($1, "bool"))
                CODEGEN("iaload\n");
            else if (!strcmp($1, "f32"))
                CODEGEN("faload\n");
            else if (!strcmp($1, "str"))
                CODEGEN("aaload\n");
        }
    | PrimaryExpression
        {
            strcpy(print_slice, "");
        }
      '[' SliceExpression ']'
        {
            CODEGEN("invokevirtual java/lang/String.substring(%s)Ljava/lang/String;\n", print_slice);
        }
    | '[' ElementList ']'
;

ElementList
    : ElementList ','
        {
            CODEGEN("aload %d\n", curr->Addr);
            CODEGEN("ldc %d\n", init_array++);
        }
      PrimaryExpression
        {
            if (!strcmp($4, "i32") || !strcmp($4, "bool"))
                CODEGEN("iastore\n");
            else if (!strcmp($4, "f32"))
                CODEGEN("fastore\n");
            else if (!strcmp($4, "str"))
                CODEGEN("aastore\n");
        }
    | 
        {
            CODEGEN("aload %d\n", curr->Addr);
            CODEGEN("ldc %d\n", init_array++);
        }
      PrimaryExpression
        {
            if (!strcmp($2, "i32") || !strcmp($2, "bool"))
                CODEGEN("iastore\n");
            else if (!strcmp($2, "f32"))
                CODEGEN("fastore\n");
            else if (!strcmp($2, "str"))
                CODEGEN("aastore\n");
        }
;

SliceExpression
    : SliceExpression SliceOperation
    | SliceOperation
        {
            if (!strcmp($1, "DOTDOT"))
            {
                strcat(print_slice, "I");
                CODEGEN("ldc 0\n");
            }
        }
;

SliceOperation
    : DOTDOT { printf("DOTDOT\n"); $$ = "DOTDOT"; }
    | PrimaryExpression
        {
            $$ = $1;
            if (!strcmp($1, "i32") || !strcmp($1, "bool"))
                strcat(print_slice, "I");
            else if (!strcmp($1, "f32"))
                strcat(print_slice, "F");
            else if (!strcmp($1, "str"))
                strcat(print_slice, "S");
        }
;

AssignExpression
    : ID AssignOperator
        { 
            symbolpointer id = look_up_symbol($<s_val>1, "variable_find_no_re");
            if (strcmp($<type>2, "ASSIGN")) 
            {                
                if (!strcmp(id->type, "i32") || !strcmp(id->type, "bool")) 
                    CODEGEN("iload %d ; load %s\n", id->Addr, id->name);
                else if (!strcmp(id->type, "f32")) 
                    CODEGEN("fload %d ; load %s\n", id->Addr, id->name);
                else if (!strcmp(id->type, "str")) 
                    CODEGEN("aload %d ; load %s\n", id->Addr, id->name);
            }
        }
      AssignExpression
        {
            symbolpointer id = look_up_symbol($<s_val>1, "variable_find_no_re");
            if (strcmp($<type>2, "ASSIGN")) 
                type_check(id->type, $<type>4, $<type>2, "ASSIGNMENT");
            
            if (id != NULL)
            {
                printf("%s\n", $<type>2);
                //printf("%s\n", id);
                if (!strcmp(id->type, "i32")) 
                {
                    if (!strcmp($<type>2, "ADD_ASSIGN")) 
                        CODEGEN("iadd\n");
                    else if (!strcmp($<type>2, "SUB_ASSIGN")) 
                        CODEGEN("isub\n");
                    else if (!strcmp($<type>2, "MUL_ASSIGN")) 
                        CODEGEN("imul\n");
                    else if (!strcmp($<type>2, "DIV_ASSIGN")) 
                        CODEGEN("idiv\n");
                    else if (!strcmp($<type>2, "REM_ASSIGN")) 
                        CODEGEN("irem\n");
                    CODEGEN("istore %d ; store %s\n", id->Addr, id->name);
                }
                else if (!strcmp(id->type, "f32")) 
                {
                    if (!strcmp($<type>2, "ADD_ASSIGN")) 
                        CODEGEN("fadd\n");
                    else if (!strcmp($<type>2, "SUB_ASSIGN")) 
                        CODEGEN("fsub\n");
                    else if (!strcmp($<type>2, "MUL_ASSIGN")) 
                        CODEGEN("fmul\n");
                    else if (!strcmp($<type>2, "DIV_ASSIGN")) 
                        CODEGEN("fdiv\n");
                    CODEGEN("fstore %d ; store %s\n", id->Addr, id->name);
                }
                else if (!strcmp(id->type, "str")) 
                    CODEGEN("astore %d ; store %s\n", id->Addr, id->name);
                else if (!strcmp(id->type, "bool")) 
                    CODEGEN("istore %d ; store %s\n", id->Addr, id->name);
            }
        }
    //| 
    | LogicalOrExpression
;

LogicalOrExpression 
    : LogicalAndExpression { $$ = $1; }
    | LogicalOrExpression LOR LogicalAndExpression 
        { 
            $$ = "bool"; 
            printf("LOR\n");
            CODEGEN("ior\n"); 
        }
    ;

LogicalAndExpression
    : CompareExpression { $$ = $1; }
    | LogicalAndExpression LAND CompareExpression 
        { 
            $$ = "bool"; 
            printf("LAND\n"); 
            CODEGEN("iand\n"); 
        }
    ;

CompareExpression
    : ShiftExpression { $$ = $1; }
    | CompareExpression CompareOperator ShiftExpression 
        { 
            $$ = "bool"; 
            type_check($<type>1, $<type>3, $2, "COMPARE");
            if ($<type>1 != NULL && $<type>3 != NULL)
            {
                if (!strcmp($1, "i32"))
                    CODEGEN("isub\n");
                else if (!strcmp($1, "f32"))
                    CODEGEN("fcmpg\n");
                
                if (!strcmp($2, "EQL")) 
                {
                    printf("EQL\n"); 
                    CODEGEN("ifeq L_cmp_%d\n", jump_count);
                }
                else if (!strcmp($2, "NEQ")) 
                {
                    printf("NEQ\n"); 
                    CODEGEN("ifne L_cmp_%d\n", jump_count);
                }
                else if (!strcmp($2, "<")) 
                {
                    printf("LSS\n"); 
                    CODEGEN("iflt L_cmp_%d\n", jump_count);
                }
                else if (!strcmp($2, ">")) 
                {
                    printf("GTR\n"); 
                    CODEGEN("ifgt L_cmp_%d\n", jump_count);
                }
                else if (!strcmp($2, "GEQ")) 
                {
                    printf("GEQ\n"); 
                    CODEGEN("ifge L_cmp_%d\n", jump_count);
                }
                else if (!strcmp($2, "LEQ")) 
                {
                    printf("LEQ\n"); 
                    CODEGEN("ifle L_cmp_%d\n", jump_count);
                }
                
                CODEGEN("iconst_0\n");
                CODEGEN("goto L_cmp_%d\n", jump_count + 1);

                g_indent_cnt--;
                CODEGEN("L_cmp_%d:\n", jump_count);
                g_indent_cnt++;
                CODEGEN("iconst_1\n");
                g_indent_cnt--;
                CODEGEN("L_cmp_%d:\n", jump_count + 1);
                g_indent_cnt++;

                jump_count += 2;
            }
        }
;

CompareOperator
    : EQL { $$ = "EQL"; }
    | NEQ { $$ = "NEQ"; }
    | '<' { $$ = "<"; }
    | '>' { $$ = ">"; }
    | GEQ { $$ = "GEQ"; }
    | LEQ { $$ = "LEQ"; }
;

ShiftExpression
    : AdditiveExpression { $$ = $1; }
    | ShiftExpression LSHIFT AdditiveExpression 
        { 
            $$ = $1;
            type_check($<type>1, $<type>3, "LSHIFT", "ARITHMETIC"); 
            printf("LSHIFT\n");
            CODEGEN("ishl\n");
        }
    | ShiftExpression RSHIFT AdditiveExpression 
        {
            $$ = $1;
            type_check($<type>1, $<type>3, "RSHIFT", "ARITHMETIC"); 
            printf("RSHIFT\n"); 
            CODEGEN("iushr\n");
        }
;

AdditiveExpression
    : MultiplicativeExpression { $$ = $1; }
    | AdditiveExpression '+' MultiplicativeExpression 
        { 
            $$ = $1;
            type_check($1, $3, "ADD", "ARITHMETIC"); 

            if (!strcmp($<type>1, "i32"))
                CODEGEN("iadd\n");
            else if (!strcmp($<type>1, "f32"))
                CODEGEN("fadd\n");
            printf("ADD\n"); 
        }
    | AdditiveExpression '-' MultiplicativeExpression 
        {
            $$ = $1;
            type_check($<type>1, $<type>3, "SUB", "ARITHMETIC");  
            
            if (!strcmp($<type>1, "i32"))
                CODEGEN("isub\n");
            else if (!strcmp($<type>1, "f32"))
                CODEGEN("fsub\n");
            printf("SUB\n"); 
        }
    ;

MultiplicativeExpression
    : UnaryExpression { $$ = $1; }
    | MultiplicativeExpression '*' UnaryExpression 
        { 
            $$ = $1;
            type_check($<type>1, $<type>3, "MUL", "ARITHMETIC"); 
            
            if (!strcmp($<type>1, "i32"))
                CODEGEN("imul\n");
            else if (!strcmp($<type>1, "f32"))
                CODEGEN("fmul\n");
            printf("MUL\n"); 
        }
    | MultiplicativeExpression '/' UnaryExpression 
        { 
            $$ = $1;
            type_check($<type>1, $<type>3, "DIV", "ARITHMETIC"); 
            
            if (!strcmp($<type>1, "i32"))
                CODEGEN("idiv\n");
            else if (!strcmp($<type>1, "f32"))
                CODEGEN("fdiv\n");
            printf("DIV\n"); 
        }
    | MultiplicativeExpression '%' UnaryExpression 
        { 
            $$ = $1;
            type_check($<type>1, $<type>3, "REM", "ARITHMETIC"); 
            printf("REM\n"); 
            CODEGEN("irem\n");
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
                {
                    printf("i2f\n");
                    CODEGEN("i2f\n");
                } 
                else
                {
                    printf("f2i\n");
                    CODEGEN("f2i\n");
                }
            }
        }
    | '-' UnaryExpression 
        { 
            $$ = $2; 
            printf("NEG\n"); 
            if (!strcmp($2, "i32"))
                CODEGEN("ineg\n"); 
            else if (!strcmp($2, "f32"))
                CODEGEN("fneg\n"); 
        }
    | '!' UnaryExpression { $$ = $2; printf("NOT\n"); CODEGEN("iconst_1\n"); CODEGEN("ixor\n"); }
    ;

PrimaryExpression
    : Literal { $$ = $1; }
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
        { 
            printf("PRINT %s\n", $<type>3); 
            if (!strcmp($<type>3, "i32")) 
            {
                CODEGEN("getstatic java/lang/System/out Ljava/io/PrintStream;\n");
                CODEGEN("swap\n");
                CODEGEN("invokevirtual java/io/PrintStream/print(I)V\n");
            }
            else if (!strcmp($<type>3, "f32"))
            {
                CODEGEN("getstatic java/lang/System/out Ljava/io/PrintStream;\n");
                CODEGEN("swap\n");
                CODEGEN("invokevirtual java/io/PrintStream/print(F)V\n");
            }
            else if (!strcmp($<type>3, "str"))
            {
                CODEGEN("getstatic java/lang/System/out Ljava/io/PrintStream;\n");
                CODEGEN("swap\n");
                CODEGEN("invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V\n");
            }
            else if(!strcmp($<type>3, "bool"))
            {
                CODEGEN("ifeq L_cmp_%d\n", jump_count);
                CODEGEN("ldc \"true\"\n");
                CODEGEN("goto L_cmp_%d\n", jump_count + 1);
                
                g_indent_cnt--;
                CODEGEN("L_cmp_%d:\n", jump_count);
                g_indent_cnt++;
                CODEGEN("ldc \"false\"\n");

                g_indent_cnt--;
                CODEGEN("L_cmp_%d:\n", jump_count + 1);
                g_indent_cnt++;
                jump_count += 2;

                CODEGEN("getstatic java/lang/System/out Ljava/io/PrintStream;\n");
                CODEGEN("swap\n");
                CODEGEN("invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V\n");
            }
        }
    | PRINTLN '(' ExpressionStatement ')'
        { 
            printf("PRINTLN %s\n", $<type>3); 
            if (!strcmp($<type>3, "i32")) 
            {
                CODEGEN("getstatic java/lang/System/out Ljava/io/PrintStream;\n");
                CODEGEN("swap\n");
                CODEGEN("invokevirtual java/io/PrintStream/println(I)V\n");
            }
            else if (!strcmp($<type>3, "f32"))
            {
                CODEGEN("getstatic java/lang/System/out Ljava/io/PrintStream;\n");
                CODEGEN("swap\n");
                CODEGEN("invokevirtual java/io/PrintStream/println(F)V\n");
            }
            else if (!strcmp($<type>3, "str"))
            {
                CODEGEN("getstatic java/lang/System/out Ljava/io/PrintStream;\n");
                CODEGEN("swap\n");
                CODEGEN("invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V\n");
            }
            else if(!strcmp($<type>3, "bool"))
            {
                CODEGEN("ifeq L_cmp_%d\n", jump_count);
                CODEGEN("ldc \"true\"\n");
                CODEGEN("goto L_cmp_%d\n", jump_count + 1);
                
                g_indent_cnt--;
                CODEGEN("L_cmp_%d:\n", jump_count);
                g_indent_cnt++;
                CODEGEN("ldc \"false\"\n");

                g_indent_cnt--;
                CODEGEN("L_cmp_%d:\n", jump_count + 1);
                g_indent_cnt++;
                jump_count += 2;

                CODEGEN("getstatic java/lang/System/out Ljava/io/PrintStream;\n");
                CODEGEN("swap\n");
                CODEGEN("invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V\n");
            }
        }
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
    : INT_LIT { $$ = "i32"; printf("INT_LIT %d\n", $<i_val>1); CODEGEN("ldc %d\n", $<i_val>1); }
    | FLOAT_LIT {  $$ = "f32"; printf("FLOAT_LIT %.6f\n", $<f_val>1); CODEGEN("ldc %.6f\n", $<f_val>1); }
    | TRUE { $$ = "bool"; printf("bool TRUE\n"); CODEGEN("iconst_1\n"); }
    | FALSE { $$ = "bool"; printf("bool FALSE\n"); CODEGEN("iconst_0\n"); }
    | '"' STRING_LIT '"' { $$ = "str"; printf("STRING_LIT \"%s\"\n", $<s_val>2); CODEGEN("ldc \"%s\"\n", $<s_val>2); }
    | '"' '"'  { $$ = "str"; printf("STRING_LIT \"\"\n"); CODEGEN("ldc \"\"\n"); }
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
    if (!yyin) {
        printf("file `%s` doesn't exists or cannot be opened\n", argv[1]);
        exit(1);
    }

    /* Codegen output init */
    char *bytecode_filename = "hw3.j";
    fout = fopen(bytecode_filename, "w");
    CODEGEN(".source hw3.j\n");
    CODEGEN(".class public Main\n");
    CODEGEN(".super java/lang/Object\n");

    /* Symbol table init */
    // Add your code

    create_symbol();
    yylineno = 0;
    yyparse();
    dump_symbol();

    /* Symbol table dump */
    // Add your code

	printf("Total lines: %d\n", yylineno);
    fclose(fout);
    fclose(yyin);

    if (g_has_error) {
        remove(bytecode_filename);
    }
    yylex_destroy();
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
                if(!strcmp(command, "update func_sig"))
                {
                    strcpy(find->Func_sig, print_type);
                    return NULL;
                }
                else if (!strcmp(command, "variable_find")) 
                {
                    printf("IDENT (name=%s, address=%d)\n", id, find->Addr);

                    if (!strcmp(find->type, "i32") || !strcmp(find->type, "bool")) 
                        CODEGEN("iload %d ; load %s\n", find->Addr, find->name);
                    else if (!strcmp(find->type, "f32"))
                        CODEGEN("fload %d ; load %s\n", find->Addr, find->name);
                    else if (!strcmp(find->type, "str"))
                        CODEGEN("aload %d ; load %s\n", find->Addr, find->name);
                    else if (!strcmp(find->type, "array"))
                    {
                        is_array = 1;
                        CODEGEN("aload %d ; load %s\n", find->Addr, find->name);
                        return find->array_type;
                    }

                    return find->type;
                } 
                else if (!strcmp(command, "array_type_find"))
                {
                    printf("IDENT (name=%s, address=%d)\n", id, find->Addr);
                    //CODEGEN("aload %d ; load %s\n", find->Addr, find->name);
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
        g_has_error = true;
    }

    return NULL;
}

symbolpointer look_up_symbol(char* id, char* command) {
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
                    return find;
                } 
                else if (!strcmp(command, "variable_find_no_re")) 
                {
                    return find;
                }
                is_find = 1;
                break;
            }
        }
    }

    if (!is_find)
    {
		printf("error:%d: undefined: %s\n", yylineno+1, id);
        g_has_error = true;
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
        {
            printf("error:%d: invalid operation: %s (mismatched types undefined and undefined)\n", yylineno + 1, id_main);
            g_has_error = true;
        }
        else if(!id1)
        {
            printf("error:%d: invalid operation: %s (mismatched types undefined and %s)\n", yylineno + 1, id_main, id2);
            g_has_error = true;
        }
        else
        {
            printf("error:%d: invalid operation: %s (mismatched types %s and undefined)\n", yylineno + 1, id_main, id1);
            g_has_error = true;
        }
    }
    else if (!strcmp(op, "COMPARE"))
    {
        if ((!strcmp(id1, "str") && strcmp(id2, "str")) || (strcmp(id1, "str") && !strcmp(id2, "str")))
        {
            printf("error:%d: invalid operation: %s (mismatched types %s and %s)\n", yylineno + 1, id_main, id1, id2);
            g_has_error = true;
        }
    }
    else 
    {
        if (!strcmp(id_main, "REM"))
        {
            if (!strcmp(id1, "f32") || !strcmp(id2, "f32"))
            {
                printf("error:%d: invalid operation: %s (mismatched types %s and %s)\n", yylineno + 1, id_main, id1, id2);
                g_has_error = true;
            }
        }
        else if (strcmp(id1, id2))
        {
            printf("error:%d: invalid operation: %s (mismatched types %s and %s)\n", yylineno + 1, id_main, id1, id2);
            g_has_error = true;
        }
    }
}