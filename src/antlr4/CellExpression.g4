grammar CellExpression;

/** The start rule; begin parsing here. */ 

/* Rules (nonterminal) */

start: expr NEWLINE?;

expr: op=(ADD|SUB) expr                            #UnaryMinOrPlus
      |<assoc=right> left=expr POW right=expr      #Pow
      | left=expr op=(MUL|DIV) right=expr         #MulOrDiv
      | left=expr op=(ADD|SUB) right=expr         #AddOrSub
      | op=(MMAX|MMIN) '(' expr (','expr)*')'     #MmaxOrMmin
      | CELL_NAME                                 #CellName
      | NUMBER                                    #Number
      | '(' inner=expr ')'                        #Paranthesis
      ;

/* Tokens (terminal) */

NEWLINE:'\r'? '\n' ;     
WS :   [ \t]+ -> skip ; // toss out whitespace
MMAX: 'mmax';
MMIN: 'mmin';
POW:  '^';
MUL:  '*';
DIV:  '/';
ADD:  '+';
SUB:  '-';
CELL_NAME: [A-Z]+[0-9]+;
NUMBER: (R_INT|R_FLOAT);

fragment R_INT: DIGIT+;
fragment R_FLOAT: DIGIT+'.'DIGIT*
                | '.' DIGIT+
                ;
fragment DIGIT: [0-9];
