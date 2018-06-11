%{
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

%}



%%
\n {fprintf(yyout, "%s", "\n") ;}
[\(] {fprintf(yyout,"%s %s ","keyword",yytext);}
[\)] {fprintf(yyout,"%s %s ","keyword",yytext);}
[\{] {fprintf(yyout,"%s %s ","keyword",yytext);}
[\}] {fprintf(yyout,"%s %s ","keyword",yytext);}
"void" {fprintf(yyout,"%s %s ","type",yytext);}
"int" {fprintf(yyout,"%s %s ","type",yytext);}
"double" {fprintf(yyout,"%s %s ","type",yytext);}
"string" {fprintf(yyout,"%s %s ","type",yytext);}
"char" {fprintf(yyout,"%s %s ","type",yytext);}
"float" {fprintf(yyout,"%s %s ","type",yytext);}
"bool" {fprintf(yyout,"%s %s ","type",yytext);}
"for" {fprintf(yyout,"%s %s ","keyword",yytext);}
"if" {fprintf(yyout,"%s %s ","keyword",yytext);}
"else" {fprintf(yyout,"%s %s ","keyword",yytext);}
"return" {fprintf(yyout,"%s %s ","keyword",yytext);}
[a-zA-Z_0-9]+[\(] {fprintf(yyout,"%s %s ","function_name",yytext) ;}
[\"][a-zA-Z_0-9\+\-\*\/\ \<\>\,\&]*[\"] {fprintf(yyout,"%s %s ","string_value",yytext) ;}
[0-9]+ {fprintf(yyout,"%s %s ","value",yytext);}
[a-zA-Z_0-9]+ {fprintf(yyout,"%s %s ","var",yytext);}
[\++][a-zA-Z_0-9]+ {fprintf(yyout,"%s %s ","unary_op_f",yytext) ;}
[a-zA-Z_0-9]+[\++] {fprintf(yyout,"%s %s ","unary_op_b",yytext) ;}
[\*][a-zA-Z_0-9]+ {fprintf(yyout,"%s %s ","unary_op_f2",yytext) ;}
"=" {fprintf(yyout,"%s %s ","assign_op",yytext);}
"+" {fprintf(yyout,"%s %s ","binary_op",yytext);}
"-" {fprintf(yyout,"%s %s ","binary_op",yytext);}
"*" {fprintf(yyout,"%s %s ","binary_op",yytext);}
"/" {fprintf(yyout,"%s %s ","binary_op",yytext);}

. {}

%%
main()
{
        yyin = fopen("source.txt","r");
        yyout = fopen("result.txt","w");
        yylex();
        fclose(yyin);
        fclose(yyout);
}
   