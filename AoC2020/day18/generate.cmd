@echo off
:: Created 2016, Mike Lischke (public domain)

:: This script is used to generate source files from the test grammars in the same folder. The generated files are placed
:: into a subfolder "generated" which the demo project uses to compile a demo binary.

:: Download the ANLTR jar and place it in the same folder as this script (or adjust the LOCATION var accordingly).

set LOCATION=antlr-4.9-complete.jar
java -jar %LOCATION% -Dlanguage=Cpp -no-listener -visitor -o generated/ -package antlrcpp ALexer.g4 AParser.g4
java -jar %LOCATION% -Dlanguage=Cpp -no-listener -visitor -o generated/ -package antlrcpp ALexer.g4 AParser2.g4