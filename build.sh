#!/bin/bash
g++ -Wall -std=gnu++11 -O2 Token.cpp Tokenizer.cpp Parser.cpp AST.cpp dump.cpp TypeCheck.cpp OffsetCalc.cpp CodeGen.cpp SyntaxTest.cpp -o compiler
