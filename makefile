all:
	make syntax
	make lexical

lexical: results/compile/lexical.o
	gcc -o results/lexical $^

results/compile/lexical.o: lexical/main.c
	gcc -c $< -o $@

syntax: results/compile/declare_part.o \
                results/compile/main.o \
                results/compile/program_body_part.o \
                results/compile/token.o \
                results/compile/type_declare_part.o \
                results/compile/utils.o \
                results/compile/var_declare_part.o
	gcc -o results/syntax $^

results/compile/declare_part.o: syntax/declare_part.c
	gcc -c $< -o $@

results/compile/main.o: syntax/main.c
	gcc -c $< -o $@

results/compile/program_body_part.o: syntax/program_body_part.c
	gcc -c $< -o $@

results/compile/token.o: syntax/token.c
	gcc -c $< -o $@

results/compile/type_declare_part.o: syntax/type_declare_part.c
	gcc -c $< -o $@

results/compile/utils.o: syntax/utils.c
	gcc -c $< -o $@

results/compile/var_declare_part.o: syntax/var_declare_part.c
	gcc -c $< -o $@

clean:
	rm -rf results/compile/*.o results/syntax results/lexical