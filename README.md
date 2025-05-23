# compilerdesign_project
Simple Recursive Descent Parser and Evaluator in C

This repository contains two versions of a simple recursive descent parser and evaluator for a tiny language supporting:

Variable assignment (:=)
Arithmetic expressions with +, *, and parentheses
write statement to print evaluated results
Files

slr(1).c — Original parser implementation (filename includes parentheses; may require escaping or renaming on some systems)
parser.c — Cleaned-up and improved version with expression evaluation and variable storage
Features

Lexical analysis with tokenization and keyword detection
Recursive descent parsing for statements and expressions
Symbol table to manage variable values
Expression evaluation during parsing
Basic error detection and reporting
Usage

Rename or escape files with parentheses in their names before compiling, e.g.:
mv "slr(1).c" parser.c
Compile:
gcc parser.c -o parser
Run:
./parser
Modify the input string inside main() to test different programs.
Example

Input program (in input string):

x := 2 + 3 * 4; write x;
Output:

14
Parsing completed successfully.
Notes

Designed as an educational example of parsing and interpretation in C.
Parentheses in filenames require shell escaping or renaming to avoid issues.
Feel free to extend with new language features!
