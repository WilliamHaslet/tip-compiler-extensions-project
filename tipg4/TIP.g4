grammar TIP;
// Grammar for Moeller and Schwartzbach's Tiny Imperative Language (TIP)

////////////////////// TIP Programs ////////////////////////// 

program : (function)+
;

function : nameDeclaration 
           '(' (nameDeclaration (',' nameDeclaration)*)? ')' 
           '{' (declaration*) (statement*) returnStmt '}' 
;

////////////////////// TIP Declarations ///////////////////////// 

declaration : KVAR nameDeclaration (',' nameDeclaration)* ';' ;

nameDeclaration : IDENTIFIER ;

////////////////////// TIP Expressions //////////////////////////

// Expressions in TIP are ordered to capture precedence.
// We adopt the C convention that orders operators as:
//   postfix, unary, multiplicative, additive, relational, and equality 
//
// NB: # creates rule label that can be accessed in visitor
//
// ANTLR4 can automatically refactor direct left-recursion so we
// place all recursive rules as options in a single rule.  This
// means that we have some complex rules here that might otherwise
// be separated out, e.g., funAppExpr, and that we can't factor out
// other useful concepts, e.g., defining a rule for the subset of
// expressions that can be used as an l-value.  We prefer a clean 
// grammar, which simplifies AST construction, and work around these
// issues elsewhere in the compiler, e.g.,  introducing an assignable expr
// weeding pass. 
//
expr : expr '(' (expr (',' expr)*)? ')' 	#funAppExpr
     | expr '.' IDENTIFIER 			#accessExpr

     | 'not' expr 		                #notExpr

     | '*' expr 				        #deRefExpr
     | SUB NUMBER				        #negNumber
     | SUB expr				            #negationExpr
     | '&' expr					        #refExpr
     | expr op=(MUL | DIV | MOD) expr 		#multiplicativeExpr
     | expr op=(ADD | SUB) expr 		    #additiveExpr
     | expr op=(GT | GTE | LT | LTE) expr 	#relationalExpr
     | expr op=(EQ | NE) expr 			    #equalityExpr

     | expr 'and' expr                  #andExpr
     | expr 'or' expr 	                #orExpr

     | expr '?' expr ':' expr 	        #ternaryExpr

     | IDENTIFIER				        #varExpr
     | NUMBER					        #numExpr
     | KINPUT					        #inputExpr
     | KALLOC expr				        #allocExpr
     | KNULL					        #nullExpr
     | recordExpr				        #recordRule
     | '(' expr ')'				        #parenExpr

     | 'true'				            #trueExpr
     | 'false'				            #falseExpr
     | '[' expr 'of' expr ']'           #ofArrayExpr
     | '[' (expr (',' expr)*)? ']'      #arrayExpr
     | '#' expr                         #arrayLengthExpr
     | expr '[' expr ']'                #elementRefrenceOperatorExpr
;

recordExpr : '{' (fieldExpr (',' fieldExpr)*)? '}' ;

fieldExpr : IDENTIFIER ':' expr ;

////////////////////// TIP Statements ////////////////////////// 

statement : blockStmt
    | assignStmt
    | whileStmt
    | ifStmt
    | outputStmt
    | errorStmt
    | forIterStmt
    | forRangeStmt
    | incrementStmt
    | decrementStmt
;

assignStmt : expr '=' expr ';' ;

blockStmt : '{' (statement*) '}' ;

whileStmt : KWHILE '(' expr ')' statement ;

forIterStmt : KFOR '(' expr ':' expr ')' statement ;

forRangeStmt : KFOR '(' expr ':' expr '..' expr ('by' expr)? ')' statement ;

ifStmt : KIF '(' expr ')' statement (KELSE statement)? ;

outputStmt : KOUTPUT expr ';'  ;

errorStmt : KERROR expr ';'  ;

returnStmt : KRETURN expr ';'  ;

incrementStmt : expr '++' ';'  ;

decrementStmt : expr '--' ';'  ;

////////////////////// TIP Lexicon ////////////////////////// 

// By convention ANTLR4 lexical elements use all caps

MUL : '*' ;
DIV : '/' ;
MOD  : '%' ;
ADD : '+' ;
SUB : '-' ;
GT  : '>' ;
LT  : '<' ;
GTE  : '>=' ;
LTE  : '<=' ;
EQ  : '==' ;
NE  : '!=' ;

NUMBER : [0-9]+ ;

// Placing the keyword definitions first causes ANTLR4 to prioritize
// their matching relative to IDENTIFIER (which comes later).
KALLOC  : 'alloc' ;
KINPUT  : 'input' ;
KWHILE  : 'while' ;
KFOR    : 'for' ;
KIF     : 'if' ;
KELSE   : 'else' ;
KVAR    : 'var' ;
KRETURN : 'return' ;
KNULL   : 'null' ;
KOUTPUT : 'output' ;
KERROR  : 'error' ;

IDENTIFIER : [a-zA-Z_][a-zA-Z0-9_]* ;

// ANTLR4 has a nice mechanism for specifying the characters that should
// skipped during parsing.  You write "-> skip" after the pattern and
// let ANTLR4s pattern matching do the rest.

// Ignore whitespace
WS : [ \t\n\r]+ -> skip ;

// This does not handle nested block comments.
BLOCKCOMMENT: '/*' .*? '*/' -> skip ;

COMMENT : '//' ~[\n\r]* -> skip ;

// All the things that need to be added
// true
// false
// not 
// and 
// or
// [E1, ..., En]
// [E1 of E2]
// E1[E2]
// #[]
// %
// -(E1)
// >=
// <
// <=
// E1 ? E2 : E3
// E1++
// E1--
// for (E1 : E2)
// for (E1 : E2 .. E3 by E4)
