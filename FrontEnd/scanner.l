%{
#include <cstring>
#include <cstdlib>
#include "parser.hpp"
#include "FrontEnd/ProcessLog.hpp"
%}

%option nounput

%%

array|ARRAY         {return ARRAYSY;}
begin|BEGIN         {return BEGINSY;}
chr|CHR             {return CHRSY;}
const|CONST         {return CONSTSY;} 
downto|DOWNTO       {return DOWNTOSY;} 
do|DO               {return DOSY;} 
else|ELSE           {return ELSESY;}
elseif|ELSEIF       {return ELSEIFSY;}
end|END             {return ENDSY;} 
forward|FORWARD     {return FORWARDSY;} 
for|FOR             {return FORSY;} 
function|FUNCTION   {return FUNCTIONSY;} 
if|IF               {return IFSY;}
of|OF               {return OFSY;}
ord|ORD             {return ORDSY;} 
pred|PRED           {return PREDSY;} 
procedure|PROCEDURE {return PROCEDURESY;}
read|READ           {return READSY;} 
record|RECORD       {return RECORDSY;}
repeat|REPEAT       {return REPEATSY;}
return|RETURN       {return RETURNSY;} 
stop|STOP           {return STOPSY;}
succ|SUCC           {return SUCCSY;}
then|THEN           {return THENSY;} 
to|TO               {return TOSY;}
type|TYPE           {return TYPESY;}
until|UNTIL         {return UNTILSY;}
var|VAR             {return VARSY;}
while|WHILE         {return WHILESY;}
write|WRITE         {return WRITESY;} 

"%"  {return MODSY;}
"&"  {return ANDSY;}  
"("  {return LPARENSY;} 
")"  {return RPARENSY;} 
"*"  {return MULTSY;} 
"+"  {return PLUSSY;}
","  {return COMMASY;}
"-"  {return MINUSSY;} 
"."  {return DOTSY;}
"/"  {return DIVSY;}
":"  {return COLONSY;}
":=" {return ASSIGNSY;}
";"  {return SCOLONSY;} 
"<"  {return LTSY;} 
"<=" {return LTESY;} 
"<>" {return NEQSY;} 
"="  {return EQSY;}
">"  {return GTSY;}
">=" {return GTESY;} 
"["  {return LBRACKETSY;} 
"]"  {return RBRACKETSY;}
"|"  {return ORSY;}
"~"  {return NOTSY;}

[a-zA-Z][a-zA-Z0-9_]* {yylval.str_val = strdup(yytext); return IDENTSY;}

[0][0-7]+          {yylval.int_val = strtol(yytext,nullptr,0);return INTSY;}
[0][x][0-9a-fA-F]+ {yylval.int_val = strtol(yytext,nullptr,0);return INTSY;}
[0-9]+[0-9]*       {yylval.int_val = strtol(yytext,nullptr,0);return INTSY;}

'\\n'               {yylval.char_val = '\n';return CHARCONSTSY;}
'\\t'               {yylval.char_val = '\t';return CHARCONSTSY;}
'\\r'               {yylval.char_val = '\r';return CHARCONSTSY;}
'\\?.'             {yylval.char_val = yytext[1];return CHARCONSTSY;}

\"[a-zA-Z0-9~`!@'#$%^&*()_+=\-\[\]{}\\\/><,.:;| ]*\" {yylval.str_val = strdup(yytext); return STRINGSY;}

\$.*$ {ProcessLog::getInstance()->nextLine();}
\n {ProcessLog::getInstance()->nextLine();}
[ \t]+ {}
. {ProcessLog::getInstance()->logError(std::string("Unexpected token ")+yytext);}

%%
