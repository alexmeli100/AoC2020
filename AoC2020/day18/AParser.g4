parser grammar  AParser;

options {
    tokenVocab = ALexer;
}

prog: (expression)+ EOF;

expression: NUMBER #Number
    | OpenPar expression ClosePar #Parens
    | expression (Mul | Plus) expression #PlusMult
    ;