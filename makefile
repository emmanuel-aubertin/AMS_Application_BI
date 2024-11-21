ifeq ($(OS),Windows_NT)
	PRINT=@echo -e
	COMPILER=g++
    COLOR=\033
else
	UNAME_S := $(shell uname -s)
    COLOR=\e
	ifeq ($(UNAME_S),Darwin)
		PRINT=@printf
		COMPILER=clang++
	endif
	ifeq ($(UNAME_S),Linux)
		PRINT=@printf
		COMPILER=g++
	endif
endif

all: test

clean:
	$(PRINT) "\n\e[32m--------| \e[1;32mCLEANING\e[0m\e[32m |--------\e[0m\n\n"
	rm -f ams-BI out.bin *.gch 
	@printf "\e[32m\tDONE\e[0m\n"

compiler: 
	@printf "\e[32m--------| \e[1;32mCompilation of all your .cpp\e[0m\e[32m |--------\e[0m\n\n"
	$(COMPILER) -std=c++23 src/*.cpp -o ams-BI
	@printf "\e[32m\tDONE\e[0m\n"


test: compiler
	$(PRINT) "\e[32m--------| \e[1mTest of the binary file\e[0m\e[32m |--------\e[0m\n\n"
	./ams-BI --help
	$(PRINT) "\e[32m\tDONE\e[0m\n"

.PHONY: all clean test