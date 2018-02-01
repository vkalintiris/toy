all: binary

binary: lexer
	clang \
	  -I/usr/local/opt/flex/include \
	  -L/usr/local/opt/flex/lib -lfl \
	  -o lexer lex.yy.c

lexer: lexer.l
	flex lexer.l

clean:
	rm -f lexer lex.yy.c
