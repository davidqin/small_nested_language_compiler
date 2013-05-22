snl: results/compile/lexical.o \
     results/compile/procedure_declare_part.o \
     results/compile/declare_part.o \
     results/compile/main.o \
     results/compile/program_body_part.o \
     results/compile/token.o \
     results/compile/type_declare_part.o \
     results/compile/utils.o \
     results/compile/var_declare_part.o
	gcc -g -o $@ $^

results/compile/lexical.o: lexical/main.c
	gcc -g -c $< -o $@

results/compile/procedure_declare_part.o: syntax/procedure_declare_part.c
	gcc -g -c $< -o $@

results/compile/declare_part.o: syntax/declare_part.c
	gcc -g -c $< -o $@

results/compile/main.o: syntax/main.c
	gcc -g -c $< -o $@

results/compile/program_body_part.o: syntax/program_body_part.c
	gcc -g -c $< -o $@

results/compile/token.o: syntax/token.c
	gcc -g -c $< -o $@

results/compile/type_declare_part.o: syntax/type_declare_part.c
	gcc -g -c $< -o $@

results/compile/utils.o: syntax/utils.c
	gcc -g -c $< -o $@

results/compile/var_declare_part.o: syntax/var_declare_part.c
	gcc -g -c $< -o $@

clean:
	rm -rf results/compile/*.o syntax_analyst lexical_analyst