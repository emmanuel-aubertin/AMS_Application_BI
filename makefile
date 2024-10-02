all: test

clean:
	@printf "\n\e[32m--------| \e[1;32mCLEANING\e[0m\e[32m |--------\e[0m\n\n"
	rm -f out.bin *.gch 
	@printf "\e[32m\tDONE\e[0m\n"

compiler: 
	@printf "\e[32m--------| \e[1;32mCompilation of all your .cpp\e[0m\e[32m |--------\e[0m\n\n"
	clang++ -std=c++23 src/main.cpp src/parser.cpp -o out.bin
	@printf "\e[32m\tDONE\e[0m\n"

test: compiler
	@printf "\e[32m--------| \e[1mTest of the binary file\e[0m\e[32m |--------\e[0m\n\n"
	./out.bin
	@printf "\e[32m\tDONE\e[0m\n"

.PHONY: all clean test
