%{
#include <iostream>
#include "ProcessLog.hpp"
extern int yylex();
void yyerror(const char*);
%}

%union
{
  char * char_ptr;
  int int_val;
  char char_val;
}

%error-verbose
%token ARRAYSY ELSESY IFSY RECORDSY TOSY BEGINSY ELSEIFSY OFSY
%token REPEATSY TYPESY CHRSY ENDSY ORDSY RETURNSY UNTILSY CONSTSY 
%token FORSY PREDSY STOPSY VARSY DOSY FORWARDSY PROCEDURESY SUCCSY
%token WHILESY DOWNTOSY FUNCTIONSY READSY THENSY WRITESY
%token LPARENSY RPARENSY SCOLONSY COLONSY LBRACKETSY RBRACKETSY
%token COMMASY ASSIGNSY 
%token INTSY CHARCONSTSY STRINGSY IDENTSY

%token EQSY LTESY GTESY LTSY GTSY PLUSSY MINUSSY MULTSY
%token DIVSY MODSY NOTSY DOTSY 

%left ANDSY ORSY
%right NOTSY
%nonassoc EQSY LTESY GTESY GTSY LTSY NEQSY 
%left PLUSSY MINUSSY 
%left DIVSY MODSY MULTSY
%right UMINUSSY 


%type <int_val> INTSY
%type <char_val> CHARCONSTSY
%type <char_ptr>  STRINGSY IDENTSY

%%
Program : OptConstDecls OptTypeDecls OptVarDecls PFDecls Block DOTSY
				;
OptConstDecls : CONSTSY ConstDecls
							|
							;

ConstDecls : ConstDecls ConstDecl
					 | ConstDecl
					 ;

ConstDecl : IDENTSY EQSY Expression SCOLONSY 
					;

PFDecls : PFDecls ProcedureDecl
        | PFDecls FunctionDecl
        |
        ;

ProcedureDecl : PSignature SCOLONSY FORWARDSY SCOLONSY
              | PSignature SCOLONSY Body SCOLONSY
				    	;

PSignature : PROCEDURESY IDENTSY LPARENSY OptFormalParameters RPARENSY
           ;

FunctionDecl : FSignature SCOLONSY FORWARDSY SCOLONSY
						 | FSignature SCOLONSY Body SCOLONSY
						 ;

FSignature : FUNCTIONSY IDENTSY LPARENSY OptFormalParameters RPARENSY COLONSY Type
           ;

OptFormalParameters : FormalParameters
                    |
                    ;

FormalParameters : FormalParameters SCOLONSY FormalParameter
                 | FormalParameter
                 ;

FormalParameter : OptVar IdentList COLONSY Type
                ;

OptVar : VARSY
       |
       ;


Body : OptConstDecls OptTypeDecls OptVarDecls Block
     ;

Block : BEGINSY StatementList ENDSY
      ;

StatementList : StatementList SCOLONSY Statement
              | Statement
              ;

OptTypeDecls : TYPESY TypeDecls
             |
             ;

TypeDecls    : TypeDecls TypeDecl
             | TypeDecl
             ;

TypeDecl : IDENTSY EQSY Type SCOLONSY;
         ;

Type : SimpleType
     | RecordType
     | ArrayType
     ;

SimpleType : IDENTSY
           ;

RecordType : RECORDSY FieldDecls ENDSY
           ;

FieldDecls : FieldDecls FieldDecl
           |
           ;

FieldDecl : IdentList COLONSY Type SCOLONSY
          ;

IdentList : IdentList COMMASY IDENTSY
          | IDENTSY
          ;

ArrayType : ARRAYSY LBRACKETSY Expression COLONSY Expression RBRACKETSY OFSY Type
          ;

OptVarDecls : VARSY VarDecls
            |
            ;

VarDecls    : VarDecls VarDecl
            | VarDecl
            ;

VarDecl : IdentList COLONSY Type SCOLONSY
        ;

Statement : Assignment
          | IfStatement
          | WhileStatement
          | RepeatStatement
          | ForStatement
          | StopStatement
          | ReturnStatement
          | ReadStatement
          | WriteStatement
          | ProcedureCall
          | 
          ;

Assignment : LValue ASSIGNSY Expression
           ;

IfStatement : IfHead ThenPart ElseIfList ElseClause ENDSY
            ;

IfHead : IFSY Expression
       ;

ThenPart : THENSY StatementList
         ;

ElseIfList : ElseIfList ElseIfHead ThenPart
           |
           ;

ElseIfHead : ELSEIFSY Expression
           ;

ElseClause : ELSESY StatementList
           |
           ;

WhileStatement : WhileHead DOSY StatementList ENDSY
               ;

WhileHead : WHILESY Expression
          ;

RepeatStatement : REPEATSY StatementList UNTILSY Expression

ForStatement : ForHead ToHead DOSY StatementList ENDSY
             ;

ForHead : FORSY IDENTSY ASSIGNSY Expression
        ;

ToHead : TOSY Expression
       | DOWNTOSY Expression
       ;

StopStatement : STOPSY
              ;

ReturnStatement : RETURNSY Expression
                | RETURNSY
                ;


ReadStatement : READSY LPARENSY ReadArgs RPARENSY
              ;

ReadArgs : ReadArgs COMMASY LValue
         | LValue
         ;

WriteStatement : WRITESY LPARENSY WriteArgs RPARENSY
               ;

WriteArgs : WriteArgs COMMASY Expression
          | Expression
          ;

ProcedureCall : IDENTSY LPARENSY OptArguments RPARENSY
              ;
OptArguments : Arguments
             |
             ;
Arguments : Arguments COMMASY Expression
          | Expression
          ;

Expression : Expression ORSY Expression
           | Expression ANDSY Expression
           | Expression EQSY Expression
           | Expression NEQSY Expression
           | Expression LTESY Expression
           | Expression GTESY Expression
           | Expression LTSY Expression
           | Expression GTSY Expression
           | Expression PLUSSY Expression
           | Expression MINUSSY Expression
           | Expression MULTSY Expression
           | Expression DIVSY Expression
           | Expression MODSY Expression
           | NOTSY Expression
           | MINUSSY Expression %prec UMINUSSY
           | LPARENSY Expression RPARENSY
           | FunctionCall
           | CHRSY LPARENSY Expression RPARENSY
           | ORDSY LPARENSY Expression RPARENSY
           | PREDSY LPARENSY Expression RPARENSY
           | SUCCSY LPARENSY Expression RPARENSY
           | LValue
           | INTSY
           | CHARCONSTSY
           | STRINGSY
           ;

FunctionCall : IDENTSY LPARENSY Arguments RPARENSY
             ;

LValue : LValue DOTSY IDENTSY
       | LValue LBRACKETSY Expression RBRACKETSY
       | IDENTSY
       ;
%%

void yyerror(const char* msg)
{
  ProcessLog::getInstance()->logError(msg);
}
