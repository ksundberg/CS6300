%{
#include <iostream>
#include <fstream>
#include "ProcessLog.hpp"
#include "Expressions.hpp"
#include "SymbolTable.hpp"

extern int yylex();
extern std::ofstream cpslout;
void yyerror(const char*);
%}

%union
{
  char * str_val;
  int int_val;
  char char_val;
  Expr * expr;
  MemoryLocation * location;
  IdList * idlist;
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
%type <str_val>  STRINGSY IDENTSY Type SimpleType
%type <expr> Expression
%type <location> LValue
%type <idlist> IdentList

%%
Program : ProgramHead Block DOTSY
				;

ProgramHead : OptConstDecls OptTypeDecls OptVarDecls PFDecls {cpslout << "program:";}
            ;
OptConstDecls : CONSTSY ConstDecls
							|
							;

ConstDecls : ConstDecls ConstDecl
					 | ConstDecl
					 ;

ConstDecl : IDENTSY EQSY Expression SCOLONSY {delete($3);}
					;

PFDecls : PFDecls ProcedureDecl
        | PFDecls FunctionDecl
        |
        ;

ProcedureDecl : PSignature SCOLONSY FORWARDSY SCOLONSY {SymbolTable::getInstance()->exitScope();}
              | PSignature SCOLONSY Body SCOLONSY {SymbolTable::getInstance()->exitScope();}
				    	;

PSignature : PROCEDURESY IDENTSY LPARENSY OptFormalParameters RPARENSY {SymbolTable::getInstance()->enterScope();}
           ;

FunctionDecl : FSignature SCOLONSY FORWARDSY SCOLONSY {SymbolTable::getInstance()->exitScope();}
						 | FSignature SCOLONSY Body SCOLONSY {SymbolTable::getInstance()->exitScope();}
						 ;

FSignature : FUNCTIONSY IDENTSY LPARENSY OptFormalParameters RPARENSY COLONSY Type {SymbolTable::getInstance()->enterScope();}
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

Type : SimpleType {$$ = $1;}
     | RecordType {$$ = nullptr;}
     | ArrayType {$$ = nullptr;}
     ;

SimpleType : IDENTSY {$$ = $1;}
           ;

RecordType : RECORDSY FieldDecls ENDSY
           ;

FieldDecls : FieldDecls FieldDecl
           |
           ;

FieldDecl : IdentList COLONSY Type SCOLONSY
          ;

IdentList : IdentList COMMASY IDENTSY{$$= new IdList($3,$1);}
          | IDENTSY {$$=new IdList($1,nullptr);}
          ;

ArrayType : ARRAYSY LBRACKETSY Expression COLONSY Expression RBRACKETSY OFSY Type{delete($3);delete($5);}
          ;

OptVarDecls : VARSY VarDecls
            |
            ;

VarDecls    : VarDecls VarDecl
            | VarDecl
            ;

VarDecl : IdentList COLONSY Type SCOLONSY {auto type = SymbolTable::getInstance()->lookupType($3);SymbolTable::getInstance()->addVariable($1,type);}
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

Assignment : LValue ASSIGNSY Expression {store($1,$3);}
           ;

IfStatement : IfHead ThenPart ElseIfList ElseClause ENDSY
            ;

IfHead : IFSY Expression {delete($2);}
       ;

ThenPart : THENSY StatementList
         ;

ElseIfList : ElseIfList ElseIfHead ThenPart
           |
           ;

ElseIfHead : ELSEIFSY Expression {delete($2);}
           ;

ElseClause : ELSESY StatementList
           |
           ;

WhileStatement : WhileHead DOSY StatementList ENDSY
               ;

WhileHead : WHILESY Expression {delete($2);}
          ;

RepeatStatement : REPEATSY StatementList UNTILSY Expression {delete($4);}

ForStatement : ForHead ToHead DOSY StatementList ENDSY
             ;

ForHead : FORSY IDENTSY ASSIGNSY Expression {delete($4);}
        ;

ToHead : TOSY Expression {delete($2);}
       | DOWNTOSY Expression{delete($2);}
       ;

StopStatement : STOPSY
              ;

ReturnStatement : RETURNSY Expression{delete($2);}
                | RETURNSY
                ;


ReadStatement : READSY LPARENSY ReadArgs RPARENSY
              ;

ReadArgs : ReadArgs COMMASY LValue{read($3);}
         | LValue {read($1);}
         ;

WriteStatement : WRITESY LPARENSY WriteArgs RPARENSY
               ;

WriteArgs : WriteArgs COMMASY Expression {write($3);}
          | Expression {write($1);}
          ;

ProcedureCall : IDENTSY LPARENSY OptArguments RPARENSY
              ;
OptArguments : Arguments
             |
             ;
Arguments : Arguments COMMASY Expression {delete($3);}
          | Expression {delete($1);}
          ;

Expression : Expression ORSY Expression          {$$ =emitOr($1,$3);}
           | Expression ANDSY Expression         {$$ =emitAnd($1,$3);}
           | Expression EQSY Expression          {$$ =emitEq($1,$3);}
           | Expression NEQSY Expression         {$$ =emitNeq($1,$3);}
           | Expression LTESY Expression         {$$ =emitLte($1,$3);}
           | Expression GTESY Expression         {$$ =emitGte($1,$3);}
           | Expression LTSY Expression          {$$ =emitLt($1,$3);}
           | Expression GTSY Expression          {$$ =emitGt($1,$3);}
           | Expression PLUSSY Expression        {$$ =emitAdd($1,$3);}
           | Expression MINUSSY Expression       {$$ =emitSub($1,$3);}
           | Expression MULTSY Expression        {$$ =emitMult($1,$3);}
           | Expression DIVSY Expression         {$$ =emitDiv($1,$3);}
           | Expression MODSY Expression         {$$ =emitMod($1,$3);}
           | NOTSY Expression                    {$$ =emitNot($2);}
           | MINUSSY Expression %prec UMINUSSY   {$$ =emitNeg($2);}
           | LPARENSY Expression RPARENSY        {$$ = $2;}
           | FunctionCall                        {$$ = nullptr;}
           | CHRSY LPARENSY Expression RPARENSY  {$$ = chr($3);}
           | ORDSY LPARENSY Expression RPARENSY  {$$ = ord($3);}
           | PREDSY LPARENSY Expression RPARENSY {$$ = pred($3);}
           | SUCCSY LPARENSY Expression RPARENSY {$$ = succ($3);}
           | LValue                              {$$ = load($1);}
           | INTSY                               {$$ = load($1);}
           | CHARCONSTSY                         {$$ = load($1);}
           | STRINGSY                            {$$ = load($1);}
           ;

FunctionCall : IDENTSY LPARENSY Arguments RPARENSY
             ;

LValue : LValue DOTSY IDENTSY {$$=nullptr;}
       | LValue LBRACKETSY Expression RBRACKETSY {$$=nullptr;}
       | IDENTSY {$$=SymbolTable::getInstance()->lookupVariable($1).get();}
       ;
%%

void yyerror(const char* msg)
{
  ProcessLog::getInstance()->logError(msg);
}
