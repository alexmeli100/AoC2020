parser grammar  AParser2;

options {
    tokenVocab = ALexer;
}

prog: expression EOF;

expression: NUMBER #Number
    | OpenPar expression ClosePar #Parens
    | expression Plus expression #Plus
    | expression Mul expression #Mul
    ;