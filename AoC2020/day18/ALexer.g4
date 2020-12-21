lexer grammar ALexer;

OpenPar: '(';
ClosePar: ')';
Mul: '*';
Plus: '+';

NUMBER: [0-9]+;
WS: [ \t\n]+ -> skip;