ANTLR=/shares/public/tp/ANTLR4-CPP/bin/antlr4
ANTLRRUNTIME=/shares/public/tp/ANTLR4-CPP

default: grammar comp

grammar: CodeC.g4
	$(ANTLR) -visitor -no-listener -Dlanguage=Cpp -o "Code_genere" CodeC.g4

comp:
	clang++ -DTRACE -g -std=c++11   -I $(ANTLRRUNTIME)/antlr4-runtime/ *.cpp -o exe $(ANTLRRUNTIME)/lib/libantlr4-runtime.a

