%{
#include <stdio.h>
#include <string.h>
%}

%option noyywrap

%%

[ \t\r\n]		;
[0-9]+			{ printf("found %s: %d long!\n", yytext, (int) strlen(yytext)); }

%%

int main(){
	printf("Hello world!\n");
	yylex();
	return(0);
}
