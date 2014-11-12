%{
#include <iostream>
#include <fstream>
#include "FrontEnd/ProcessLog.hpp"
#include "FrontEnd/FrontEnd.hpp"

extern int yylex();
void yyerror(const char*);
%}

%union
{
  char * str_val;
  int int_val;
  char char_val;
}

%error-verbose
%token ARRAYSY 
%token ASSIGNSY 
%token BEGINSY 
%token CHARCONSTSY 
%token CHRSY 
%token COLONSY 
%token COMMASY 
%token CONSTSY 
%token DIVSY 
%token DOSY 
%token DOTSY 
%token DOWNTOSY 
%token ELSEIFSY 
%token ELSESY 
%token ENDSY 
%token EQSY 
%token FORSY 
%token FORWARDSY 
%token FUNCTIONSY 
%token GTESY 
%token GTSY 
%token IDENTSY
%token IFSY 
%token INTSY 
%token LBRACKETSY 
%token LPARENSY 
%token LTESY 
%token LTSY 
%token MINUSSY 
%token MODSY 
%token MULTSY
%token NOTSY 
%token OFSY
%token ORDSY 
%token PLUSSY 
%token PREDSY 
%token PROCEDURESY 
%token RBRACKETSY
%token READSY 
%token RECORDSY 
%token REPEATSY 
%token RETURNSY 
%token RPARENSY 
%token SCOLONSY 
%token STOPSY 
%token STRINGSY 
%token SUCCSY
%token THENSY 
%token TOSY 
%token TYPESY 
%token UNTILSY 
%token VARSY 
%token WHILESY 
%token WRITESY

%left ANDSY ORSY
%right NOTSY
%nonassoc EQSY LTESY GTESY GTSY LTSY NEQSY 
%left PLUSSY MINUSSY 
%left DIVSY MODSY MULTSY
%right UMINUSSY 

%type <char_val> CHARCONSTSY
%type <int_val> Arguments 
%type <int_val> ArrayType 
%type <int_val> Assignment
%type <int_val> Block 
%type <int_val> Body  
%type <int_val> ElseClause 
%type <int_val> ElseIfHead 
%type <int_val> ElseIfList 
%type <int_val> Expression 
%type <int_val> FSignature 
%type <int_val> FieldDecl 
%type <int_val> FieldDecls
%type <int_val> ForHead 
%type <int_val> ForStatement 
%type <int_val> FormalParameter
%type <int_val> FormalParameters  
%type <int_val> FunctionCall 
%type <int_val> INTSY 
%type <int_val> IdentList 
%type <int_val> IfHead 
%type <int_val> IfStatement 
%type <int_val> LValue 
%type <int_val> OptArguments 
%type <int_val> OptFormalParameters  
%type <int_val> PSignature 
%type <int_val> ProcedureCall
%type <int_val> ReadArgs
%type <int_val> ReadStatement 
%type <int_val> RecordType 
%type <int_val> RepeatStatement 
%type <int_val> ReturnStatement 
%type <int_val> SimpleType 
%type <int_val> Statement 
%type <int_val> StatementList 
%type <int_val> StopStatement 
%type <int_val> ThenPart 
%type <int_val> ToHead 
%type <int_val> Type 
%type <int_val> WhileHead 
%type <int_val> WhileStatement 
%type <int_val> WriteArgs 
%type <int_val> WriteStatement  
%type <str_val> IDENTSY 
%type <str_val> STRINGSY 

%%
Program : ProgramHead Block DOTSY {cs6300::AddMain($2);}
				;

ProgramHead : OptConstDecls OptTypeDecls OptVarDecls PFDecls
            ;
OptConstDecls : CONSTSY ConstDecls
							|
							;

ConstDecls : ConstDecls ConstDecl
					 | ConstDecl
					 ;

ConstDecl : IDENTSY EQSY Expression SCOLONSY {cs6300::AddConstant($1,$3);}
					;

PFDecls : PFDecls ProcedureDecl
        | PFDecls FunctionDecl
        |
        ;

ProcedureDecl : PSignature SCOLONSY FORWARDSY SCOLONSY {cs6300::AddFunction($1);}
              | PSignature SCOLONSY Body SCOLONSY {cs6300::AddFunction($1,$3);}
				    	;

PSignature : PROCEDURESY IDENTSY LPARENSY OptFormalParameters RPARENSY {$$ = cs6300::Signature($2,$4,-1);}
           ;

FunctionDecl : FSignature SCOLONSY FORWARDSY SCOLONSY {cs6300::AddFunction($1);}
						 | FSignature SCOLONSY Body SCOLONSY {cs6300::AddFunction($1,$3);}
						 ;

FSignature : FUNCTIONSY IDENTSY LPARENSY OptFormalParameters RPARENSY COLONSY Type {$$ = cs6300::Signature($2,$4,$7);}
           ;

OptFormalParameters : FormalParameters {$$ = $1;}
                    | {$$ = -1;}
                    ;

FormalParameters : FormalParameters SCOLONSY FormalParameter {$$ = cs6300::ParameterList($1,$3);}
                 | FormalParameter {$$ = $1;}
                 ;

FormalParameter : OptVar IdentList COLONSY Type {$$ = cs6300::Parameter($2,$4);}
                ;

OptVar : VARSY
       |
       ;


Body : OptConstDecls OptTypeDecls OptVarDecls Block {$$ = $4;}
     ;

Block : BEGINSY StatementList ENDSY {$$ = $2;}
      ;

StatementList : StatementList SCOLONSY Statement {$$ = cs6300::StatementList($1,$3);}
              | Statement {$$ = cs6300::StatementList(-1,$1);}
              ;

OptTypeDecls : TYPESY TypeDecls
             |
             ;

TypeDecls    : TypeDecls TypeDecl
             | TypeDecl
             ;

TypeDecl : IDENTSY EQSY Type SCOLONSY {cs6300::AddType($1,$3);}
         ;

Type : SimpleType {$$ = $1;}
     | RecordType {$$ = $1;}
     | ArrayType {$$ = $1;}
     ;

SimpleType : IDENTSY {$$ = cs6300::LookupType($1);}
           ;

RecordType : RECORDSY FieldDecls ENDSY {$$ = $2;}
           ;

FieldDecls : FieldDecls FieldDecl {$$ = cs6300::FieldList($1, $2);}
           | {$$ = -1;}
           ;

FieldDecl : IdentList COLONSY Type SCOLONSY {$$ = cs6300::AddField($1,$3);}
          ;

IdentList : IdentList COMMASY IDENTSY {$$ = cs6300::IdentList($1,$3);}
          | IDENTSY {$$ = cs6300::IdentList(-1,$1);}
          ;

ArrayType : ARRAYSY LBRACKETSY Expression COLONSY Expression RBRACKETSY OFSY Type {$$ = cs6300::BuildArrayType($3,$5,$8);}
          ;

OptVarDecls : VARSY VarDecls
            |
            ;

VarDecls    : VarDecls VarDecl
            | VarDecl
            ;

VarDecl : IdentList COLONSY Type SCOLONSY {cs6300::AddVariables($1,$3);}
        ;

Statement : Assignment {$$ = $1;}
          | IfStatement {$$ = $1;}
          | WhileStatement {$$ = $1;}
          | RepeatStatement {$$ = $1;}
          | ForStatement {$$ = $1;}
          | StopStatement {$$ = $1;}
          | ReturnStatement {$$ = $1;}
          | ReadStatement {$$ = $1;}
          | WriteStatement {$$ = $1;}
          | ProcedureCall {$$ = $1;}
          | {$$ = -1;}
          ;

Assignment : LValue ASSIGNSY Expression {$$ = cs6300::Assign($1,$3);}
           ;

IfStatement : IfHead ThenPart ElseIfList ElseClause ENDSY {$$ = cs6300::If($1,$2,$3,$4);}
            ;

IfHead : IFSY Expression {$$ = $2;}
       ;

ThenPart : THENSY StatementList {$$ = $2;}
         ;

ElseIfList : ElseIfList ElseIfHead ThenPart {$$ =cs6300::AppendElseList($1,$2,$3);}
           |{$$ =-1;}
           ;

ElseIfHead : ELSEIFSY Expression {$$ = $2;}
           ;

ElseClause : ELSESY StatementList {$$ = $2;}
           | {$$ = -1;}
           ;

WhileStatement : WhileHead DOSY StatementList ENDSY {$$ = cs6300::While($1,$3);}
               ;

WhileHead : WHILESY Expression {$$ = $2;}
          ;

RepeatStatement : REPEATSY StatementList UNTILSY Expression {$$=cs6300::Repeat($2,$4);}

ForStatement : ForHead ToHead DOSY StatementList ENDSY{$$ = cs6300::For($1,$2,$4);}
             ;

ForHead : FORSY IDENTSY ASSIGNSY Expression {$$ = cs6300::ForHead($2,$4);}
        ;

ToHead : TOSY Expression {$$ = cs6300::To($2);}
       | DOWNTOSY Expression {$$ = cs6300::DownTo($2);}
       ;

StopStatement : STOPSY {$$ = cs6300::Stop();}
              ;

ReturnStatement : RETURNSY Expression {$$ = cs6300::Return($2);}
                | RETURNSY {$$ = cs6300::Return();}
                ;


ReadStatement : READSY LPARENSY ReadArgs RPARENSY {$$ = $3;}
              ;

ReadArgs : ReadArgs COMMASY LValue {$$ = cs6300::ReadValue($1,$3);}
         | LValue                  {$$ = cs6300::ReadValue($1);}
         ;

WriteStatement : WRITESY LPARENSY WriteArgs RPARENSY {$$ = $3;}
               ;

WriteArgs : WriteArgs COMMASY Expression {$$ = cs6300::WriteExpr($1,$3);}
          | Expression                   {$$ = cs6300::WriteExpr($1);}
          ;

ProcedureCall : IDENTSY LPARENSY OptArguments RPARENSY {$$ = cs6300::CallProc($1,$3);}
              ;
OptArguments : Arguments {$$ = $1;}
             |           {$$ = -1;}
             ;
Arguments : Arguments COMMASY Expression {$$ = cs6300::ArgumentList($1,$3);}
          | Expression                   {$$ = cs6300::ArgumentList(-1,$1);}
          ;

Expression : CHARCONSTSY                         {$$ = cs6300::CharExpr($1);}
           | CHRSY LPARENSY Expression RPARENSY  {$$ = cs6300::ChrExpr($3);}
           | Expression ANDSY Expression         {$$ = cs6300::AndExpr($1,$3);}
           | Expression DIVSY Expression         {$$ = cs6300::DivExpr($1,$3);}
           | Expression EQSY Expression          {$$ = cs6300::EqExpr($1,$3);}
           | Expression GTESY Expression         {$$ = cs6300::GteExpr($1,$3);}
           | Expression GTSY Expression          {$$ = cs6300::GtExpr($1,$3);}
           | Expression LTESY Expression         {$$ = cs6300::LteExpr($1,$3);}
           | Expression LTSY Expression          {$$ = cs6300::LtExpr($1,$3);}
           | Expression MINUSSY Expression       {$$ = cs6300::SubExpr($1,$3);}
           | Expression MODSY Expression         {$$ = cs6300::ModExpr($1,$3);}
           | Expression MULTSY Expression        {$$ = cs6300::MultExpr($1,$3);}
           | Expression NEQSY Expression         {$$ = cs6300::NeqExpr($1,$3);}
           | Expression ORSY Expression          {$$ = cs6300::OrExpr($1,$3);}
           | Expression PLUSSY Expression        {$$ = cs6300::AddExpr($1,$3);}
           | FunctionCall                        {$$ = $1;}
           | INTSY                               {$$ = cs6300::IntExpr($1);}
           | LPARENSY Expression RPARENSY        {$$ = $2;}
           | LValue                              {$$ = cs6300::LoadExpr($1);}
           | MINUSSY Expression %prec UMINUSSY   {$$ = cs6300::UnMinusExpr($2);}
           | NOTSY Expression                    {$$ = cs6300::NotExpr($2);}
           | ORDSY LPARENSY Expression RPARENSY  {$$ = cs6300::OrdExpr($3);}
           | PREDSY LPARENSY Expression RPARENSY {$$ = cs6300::PredExpr($3);}
           | STRINGSY                            {$$ = cs6300::StrExpr($1);delete($1);}
           | SUCCSY LPARENSY Expression RPARENSY {$$ = cs6300::SuccExpr($3);}
           ;

FunctionCall : IDENTSY LPARENSY OptArguments RPARENSY {$$ = cs6300::CallExpr($1,$3);}
             ;

LValue : LValue DOTSY IDENTSY {$$ = cs6300::LoadMember($1,$3);}
       | LValue LBRACKETSY Expression RBRACKETSY {$$ = cs6300::LoadArray($1,$3);}
       | IDENTSY {$$ = cs6300::LoadId($1);}
       ;
%%

void yyerror(const char* msg)
{
  ProcessLog::getInstance()->logError(msg);
}
