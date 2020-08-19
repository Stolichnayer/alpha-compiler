%{
			#include <string.h>
			#include <stdlib.h>
			#include <stdio.h>
			#include "parser.h"


			typedef struct alpha_token_t {

				unsigned int    numline;
				unsigned int	numToken;
				char *content;
				char *type;
				char *macro;
				char *wtf;

				struct alpha_token_t * next;

			}alpha_token_t;

			char * buffered_string = NULL;
			char * old_buffered_string = NULL;
			char * undefined_string = NULL;
			char * old_undefined_string = NULL;
			int nested_comment = 0;
			int * start_comments;
			int array_index = 0;
			int yytokenno = 0;
			int lineno = 1;
      int illegal_char_flag = 0;

			alpha_token_t *head = NULL;
			alpha_token_t *tail = NULL;

			void insert_token(unsigned int numline, unsigned int numToken, char* content, char* type, char* macro, char* wtf){

				alpha_token_t *new = malloc(sizeof(alpha_token_t));

				new->numline = numline;
				new->numToken = numToken;
				new->content = strdup(content);
				new->type = strdup(type);
				new->macro = strdup(macro);
				new->wtf = strdup(wtf);
				new->next = NULL;

				if(head == NULL){
					head = new;
					tail = new;
					head->next = NULL;
					return;
				}

				tail->next = new;
				tail = new;
			}

			void print_tokens(){
				alpha_token_t *tmp = head;

				while(tmp != NULL){
					if(strcmp(tmp->type,"STRING") == 0 || strcmp(tmp->type,"ID") == 0){
						printf("%d: #%d \"%s\" %s \"%s\" <-%s\n", tmp->numline,tmp->numToken,tmp->content,tmp->type,tmp->macro,tmp->wtf);
					}else{
						printf("%d: #%d \"%s\" %s %s <-%s\n", tmp->numline,tmp->numToken,tmp->content,tmp->type,tmp->macro,tmp->wtf);
					}

					tmp = tmp->next;
				}
			}

%}

%option header-file="./scanner.h"
%option noyywrap
%option yylineno
%x string
%x blockcom
%x UNDEFINED

if          "if"
else        "else"
while       "while"
for         "for"
function    "function"
return      "return"
break       "break"
continue    "continue"
and         "and"
not         "not"
or          "or"
local       "local"
true        "true"
false       "false"
nil         "nil"

ass         "="
add         "+"
sub         "-"
mul         "*"
div         "/"
mod         "%"
eq          "=="
neq         "!="
inc         "++"
dec         "--"
gr          ">"
less        "<"
greq        ">="
leq         "<="

integer     [0-9]+
real        [0-9]+\.[0-9]+

lcurbr      "{"
rcurbr      "}"
lbr         "["
rbr         "]"
lpar        "("
rpar        ")"
semic       ";"
comma       ","
colon       ":"
dcolon      "::"
dot         "."
ddot        ".."

id          [a-zA-Z][a-zA-Z_0-9]*
linecom     "//".*


whitespace  [\t' '\r]*
newline	    ['\n']
OTHER	    	.

%%



{if}				{return IF;}

{else}   		{return ELSE;}

{while}			{return WHILE;}

{for}				{return FOR;}

{function}	{return FUNCTION;}

{return}    {return RETURN;}

{break}     {return BREAK;}

{continue}  {return CONTINUE;}

{and}    		{return AND;}

{not}       {return NOT;}

{or}        {return OR;}

{local}     {return LOCALL;}

{true}      {return TRUE;}

{false}     {return FALSE;}

{nil}       {return NIL;}

{ass}       {return ASS;}

{add}       {return ADD;}

{sub}       {return SUB;}

{mul}       {return MUL;}

{div}       {return DIV;}

{mod}       {return MOD;}

{eq}        {return EQ;}

{neq}       {return NEQ;}

{inc}       {return INC;}

{dec}       {return DEC;}

{gr}        {return GR;}

{less}      {return LESS;}

{greq}      {return GREQ;}

{leq}       {return LEQ;}

{integer}   {yylval.intValue = atoi(yytext); return INTEGER;}

{real}      {yylval.realValue = atof(yytext); return REAL;}

{lcurbr} 		{return LCURBR;}

{rcurbr} 		{return RCURBR;}

{lbr} 			{return LBR;}

{rbr} 			{return RBR;}

{lpar} 			{return LPAR;}

{rpar}			{return RPAR;}

{semic} 		{return SEMIC;}

{comma} 		{return COMMA;}

{colon} 		{return COLON;}

{dcolon} 		{return DCOLON;}

{dot} 			{return DOT;}

{ddot}			{return DDOT;}

{id}     		{yylval.stringValue = strdup(yytext); return ID;}

{linecom} 	{insert_token(lineno,++yytokenno,"","LINE_COMMENT","","enumerated");}

{whitespace}	{}

{newline}		{lineno++;}


\"          			{
                    BEGIN(string);
                    buffered_string = NULL;
                    old_buffered_string = NULL;
                  }

<string>[^"\\]*   {
                  	if(old_buffered_string == NULL){
                    	buffered_string = malloc(strlen(yytext) + 1);
                    	strcpy(buffered_string, yytext);
                    	old_buffered_string = malloc(strlen(buffered_string));
                    	strcpy(old_buffered_string, buffered_string);
                  	}else{
                    	buffered_string = realloc(buffered_string, strlen(old_buffered_string) + strlen(yytext) + 1);
                    	strcat(buffered_string, yytext);
                    	old_buffered_string = realloc(old_buffered_string, strlen(buffered_string) + 1);
                  		strcpy(old_buffered_string, buffered_string);
                  	}
									}

<string>\\t				{
                  	if(old_buffered_string == NULL){
                    	buffered_string = malloc(2);
                    	char c = '\t';
											strncat(buffered_string, &c, 1);
                    	old_buffered_string = malloc(strlen(buffered_string) + 1);
                    	strcpy(old_buffered_string, buffered_string);
                    }else{
                    	buffered_string = realloc(buffered_string, strlen(old_buffered_string) + 2);
                      char c = '\t';
											strncat(buffered_string, &c, 1);
                      old_buffered_string = realloc(old_buffered_string, strlen(buffered_string) + 1);
                      strcpy(old_buffered_string, buffered_string);
                    }
									}

<string>\\n				{
                    if(old_buffered_string == NULL){
                      buffered_string = malloc(2);
                    	char c = '\n';
											strncat(buffered_string, &c, 1);
                      old_buffered_string = malloc(strlen(buffered_string) + 1);
                      strcpy(old_buffered_string, buffered_string);
										}else{
                    	buffered_string = realloc(buffered_string, strlen(old_buffered_string) + 2);
                      char c = '\n';
                      strncat(buffered_string, &c, 1);
                      old_buffered_string = realloc(old_buffered_string, strlen(buffered_string) + 1);
                      strcpy(old_buffered_string, buffered_string);
                    }
									}

<string>\\\"			{
                    if(old_buffered_string == NULL){
                    	buffered_string = malloc(2);
                    	char c = '\"';
											strncat(buffered_string, &c, 1);
                      old_buffered_string = malloc(strlen(buffered_string) + 1);
                      strcpy(old_buffered_string, buffered_string);
                    }else{
                      buffered_string = realloc(buffered_string, strlen(old_buffered_string) + 2);
                      char c = '\"';
									    strncat(buffered_string, &c, 1);
                      old_buffered_string = realloc(old_buffered_string, strlen(buffered_string) + 1);
                      strcpy(old_buffered_string, buffered_string);
                    }
									}

<string>\\\\			{
              			if(old_buffered_string == NULL){
                    	buffered_string = malloc(2);
                      char c = '\\';
										  strncat(buffered_string, &c, 1);
                      old_buffered_string = malloc(strlen(buffered_string) + 1);
                      strcpy(old_buffered_string, buffered_string);
                    }else{
                      buffered_string = realloc(buffered_string, strlen(old_buffered_string) + 2);
                      char c = '\\';
										  strncat(buffered_string, &c, 1);
                      old_buffered_string = realloc(old_buffered_string, strlen(buffered_string) + 1);
                    	strcpy(old_buffered_string, buffered_string);
                    }
									}

<string>\"        {
                    if(buffered_string == NULL){
                    	insert_token(lineno,++yytokenno,"","STRING","","char*");
                    }else if(!illegal_char_flag){
                      insert_token(lineno,++yytokenno,buffered_string,"STRING",buffered_string,"char*");
                    }
										illegal_char_flag = 0;
				            
					yylval.stringValue = strdup(buffered_string);
					BEGIN(INITIAL);
					return STRING;
	               	}

<string>\\.{1}    {
                    printf("Illegal escape character in string.\n");
                    illegal_char_flag = 1;
									}

<string><<EOF>>   {printf("unterminated string\n"); BEGIN(INITIAL);}

"/*"                    {
													BEGIN(blockcom);
                          start_comments = realloc(start_comments, (array_index + 1) * sizeof(int));
                         	start_comments[array_index] = lineno;
                          array_index++;
												}

<blockcom>"*/"          {
													char str[100];
													array_index--;
													sprintf(str, "%d - %d", start_comments[array_index], lineno);

													if(nested_comment == 0){
														insert_token(lineno, ++yytokenno, str,"COMMENT", "BLOCK_COMMENT", "enumerated");
														BEGIN(INITIAL);
													}else{
														nested_comment--;
														insert_token(lineno, ++yytokenno, str,"COMMENT", "NESTED_COMMENT", "enumerated");
													}
												}

<blockcom>"/*"          {
													nested_comment++;
                          start_comments = realloc(start_comments, (array_index + 1) * sizeof(int));
													start_comments[array_index] = lineno;
													array_index++;
												}


<blockcom>"\n"          {lineno++;}

<blockcom>^"//"         {insert_token(lineno, ++yytokenno, "","LINE_COMMENT", "", "enumerated");}

<blockcom><<EOF>>				{
                          printf("unterminated comments\n");
                          BEGIN(INITIAL);
                        }

<blockcom>.							{}


{OTHER}									{BEGIN(UNDEFINED);
													old_undefined_string = realloc(old_undefined_string,strlen(yytext) + 1);
													strcpy(old_undefined_string,yytext);

													undefined_string = realloc(undefined_string,strlen(yytext) + 1);
													strcpy(undefined_string,yytext);
												}

<UNDEFINED>[\t' '\r\n]	{
													BEGIN(INITIAL);

														if(strcmp(yytext," ") == 0){
															//printf("break\n");
														}

													if(undefined_string != NULL){
														if(strcmp(yytext,"\n") == 0){
															lineno++;
														}

														printf("undefined character %s in line %d\n", undefined_string, lineno);
													}
												}

<UNDEFINED>[^id]			{ //[+-/=\(\)\[\]<>\;\%\*\:\,\.\/]
													BEGIN(INITIAL);
														if(undefined_string != NULL){
															char c = yytext[0];
															unput(c);
															printf("undefined character %s in line %d\n", undefined_string, lineno);
													}
												}

<UNDEFINED>[a-zA-Z_0-9]*				{
													undefined_string = realloc(undefined_string, strlen(old_undefined_string) + strlen(yytext));

													strcpy(undefined_string, old_undefined_string);

													strcat(undefined_string, yytext);

													old_undefined_string = realloc(old_undefined_string, strlen(undefined_string));
													strcpy(old_undefined_string, undefined_string);
												}

%%

