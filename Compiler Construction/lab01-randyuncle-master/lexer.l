/*
    filename: lexer.l
*/

/* Definition section */
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #define YY_NO_UNPUT
    #define YY_NO_INPUT
    #define print_token(token) \
        printf("%-8s \t %s\n", strcmp(token, "NEWLINE")? yytext: "", token)

    int line_num = 0;
    int comment_num = 0;
    int last_comment_line = -1;
%}


/* Define regular expression label */
letter [a-zA-Z_]
digit [0-9]


/* Rules section */
%%
"//"(.*)    { comment_num++; last_comment_line = line_num; print_token("COMMENT"); }
"/*"([^*]|"*"+[^*/])*"*/"   {   /* multi-comment rulings */
                                if (last_comment_line != line_num)
                                    comment_num++; /* the last comment line is different than the current command line */
                                for (int i = 0 ; i < yyleng ; i++) {
                                    if (yytext[i] == '\n') {
                                        line_num++;
                                        comment_num++; 
                                    }
                                }
                                last_comment_line = line_num;
                                printf("%s       \t %s\n", yytext, "MUTI_LINE_COMMENT");
                            }
\"([^"\n]|\\\/|[^"/])*\"    {   /* string rulings */
                                printf("%-8s \t %s\n", "\"", "QUOTA"); 
                                char out[yyleng];
                                for (int i = 0 ; i < yyleng ; i++) out[i] = '\0';
                                for (int i = 1 ; i < yyleng-1 ; i++)
                                    out[i - 1] = yytext[i];
                                if (out[0] != '\0') /* check if the string is NULL */
                                    printf("%-8s \t %s\n", out, "STRING_LIT"); 
                                printf("%-8s \t %s\n", "\"", "QUOTA"); 
                            }
"fn"        { print_token("FUNC"); }
"print"     { print_token("PRINT"); }
"println"   { print_token("PRINTLN"); }
"if"        { print_token("IF"); }
"else"      { print_token("ELSE"); }
"for"       { print_token("FOR"); }
"i32"       { print_token("INT"); }
"f32"       { print_token("FLOAT"); }
"bool"      { print_token("BOOL"); }
"true"      { print_token("TRUE"); }
"false"     { print_token("FALSE"); }
"let"       { print_token("LET"); }
"mut"       { print_token("MUT"); }
"return"    { print_token("RETURN"); }
"break"     { print_token("BREAK"); }
"as"        { print_token("AS"); }
"in"        { print_token("IN"); }
"while"     { print_token("WHILE"); }
"loop"      { print_token("LOOP"); }
{letter}({letter}|{digit})* { print_token("IDENT"); }
"("         { print_token("LPAREN"); }
")"         { print_token("RPAREN"); }
"["         { print_token("LBRACK"); }
"]"         { print_token("RBRACK"); }
"{"         { print_token("LBRACE"); }
"}"         { print_token("RBRACE"); }
"+="        { print_token("ADD_ASSIGN"); }
"-="        { print_token("SUB_ASSIGN"); }
"*="        { print_token("MUL_ASSIGN"); }
"/="        { print_token("QUO_ASSIGN"); }
"%="        { print_token("REM_ASSIGN"); }
"="         { print_token("ASSIGN"); }
"+"         { print_token("ADD"); }
"-"         { print_token("SUB"); }
"*"         { print_token("MUL"); }
"/"         { print_token("QUO"); }
"%"         { print_token("REM"); }
">"         { print_token("GTR"); }
"<"         { print_token("LSS"); }
">="        { print_token("GEQ"); }
"<="        { print_token("LEQ"); }
"=="        { print_token("EQL"); }
"!="        { print_token("NEQ"); }
"&"         { print_token("BAND"); }
"|"         { print_token("BOR"); }
"~"         { print_token("BNOT"); }
">>"        { print_token("RSHIFT"); }
"&&"        { print_token("LAND"); }
"||"        { print_token("LOR"); }
"!"         { print_token("NOT"); }
".."        { print_token("DOTDOT"); }
";"         { print_token("SEMICOLON"); }
","         { print_token("COMMA"); }
":"         { print_token("COLON"); }
"->"        { print_token("ARROW"); }
"<<"        { print_token("LSHIFT"); }
"\""        { print_token("QUOTA"); }
"\n"        { line_num++; print_token("NEWLINE"); }
{digit}+\.{digit}+  { print_token("FLOAT_LIT"); }
{digit}+    { print_token("INT_LIT"); }
<<EOF>>     { line_num++; yyterminate(); }
[ \t]+      {;}
.           {;}
%%

/*  C Code section */
int yywrap(void)
{
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc == 2) {
        yyin = fopen(argv[1], "r");
    } else {
        yyin = stdin;
    }
    yylex();
    printf("\nFinish scanning,\n");
    printf("total line: %d\n", line_num);
    printf("comment line: %d\n", comment_num);
    fclose(yyin);
    return 0;
}