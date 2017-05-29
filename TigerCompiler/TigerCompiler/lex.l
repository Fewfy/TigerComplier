%{
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int charPos = 1;
int commentDepth = 0;


#define BUFSIZE 8192
char strbuf[BUFSIZE+1];
char *strptr = NULL;
unsigned int strlength = 0;

void setup(void){
	*strbuf = '\0';
	strlength = 0;
}

char *teardown(){
	char *s = checked_malloc(strlen(strbuf)+1);
	strcpy(s,strbuf);
	return s;
}

void appendstr(char *str){
	if((strlength + strlen(str)) < BUFSIZE){
		strcat(strbuf,str);
		strlength += strlen(str);
	}
}

int yywrap(void){
	charPos = 1;
	return 1;
}

void adjust(void){
	EM_tokPos = charPos;
	charPos += yyleng;
}


}%

%x COMMENT STR
%token TIMES, DIVIDE, COMMENT, INITIAL, WHILE, FOR, TO, BREAK, LET, IN, END, FUNCTION,VAR, TYPE,
ARRAY,IF, THEN, ELSE, DO, OF, NIL, ID, COMMA, COLON, SEMICOLON,LPAREN,RPAREN,LBRACK,RBRACK,LBRAGE,
RBRAGE,DOT,PLUS,MINUS,EQ,NEQ,LT,LE,GT,GE,ASSIGN,AND,OR,INT;
%%
[ \t] 			{continue;}
(\n|\r\n) 		{strcat(strbuf,"NEWLINE");continue;}
"*"				{strcat(strbuf,"TIMES")}
"/"				{strcat(strbuf,"DIVIDE")}
"/*"			{adjust(); BEGIN(COMMENT);commentDepth++;}
<COMMENT>{
	"/*"	{adjust(); commentDepth++;}
	"*/"	{adjust(); if(--commentDepth == 0)BEGIN(INITIAL);}
	[^\n]	{adjust();}
	(\n|\r\n)	{adjust();EM_newline();}
}
"while"		{strcat(strbuf,"WHILE");}
"for"		{adjust(); return FOR;}
"to"		{adjust(); return TO;}
"break"		{adjust(); return BREAK;}
"let"		{adjust(); return LET;}
"in"		{adjust(); return IN;}
"end"		{adjust(); return END;}
"function"	{adjust(); return FUNCTION;}
"var"		{adjust(); return VAR;}
"type"		{adjust(); return TYPE;}
"array"		{adjust(); return ARRAY;}
"if"		{adjust(); return IF;}
"then"		{adjust(); return THEN;}
"else"		{adjust(); return ELSE;}
"do"		{adjust(); return DO;}
"of"		{adjust(); return OF;}
"nil"		{adjust(); return NIL;}
[a-zA-Z][a-zA-Z0-9]*	{adjust();yylval.sval = yytext;return ID;}
","			{adjust(); return COMMA;}
":"			{adjust(); return COLON;}
";"			{adjust(); return SEMICOLON;}
"("			{adjust(); return LPAREN;}
")"			{adjust(); return RPAREN;}
"["			{adjust(); return LBRACK;}
"]"			{adjust(); return RBRACK;}
"{"			{adjust(); return LBRAGE;}
"}"			{adjust(); return RBRAGE;}
"."			{adjust(); return DOT;}
"+"			{adjust(); return PLUS;}
"-"			{adjust(); return MINUS;}
"="			{adjust(); return EQ;}
"<>"		{adjust(); return NEQ;}
"<"			{adjust(); return LT;}
"<="		{adjust(); return LE;}
">"			{adjust(); return GT;}
">="		{adjust(); return GE;}
":="		{adjust(); return ASSIGN;}
"&"			{adjust(); return AND;}
"|"			{adjust(); return OR;}
[0-9]+		{adjust(); yylval.ival = atoi(yytext);return INT;}


%%

main(){

	int n = yylex();
	
}

