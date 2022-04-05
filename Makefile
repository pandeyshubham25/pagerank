all: regular_mode_cookie builddir pagerank eval

regular_mode_cookie:
	ls
	make clean
	touch regular_mode_cookie

test_mode_cookie:
	make clean
	touch test_mode_cookie

#.PHONY: build_for_test

build_for_test:
	make TEST_FLAGS=-DTEST=1 

fake_regular_cookie: clean_cookie
	touch regular_mode_cookie

correct_test_cookie: clean_cookie
	touch test_mode_cookie


.PHONY: test
test:
	make test_mode_cookie
	make fake_regular_cookie
	make build_for_test
	make correct_test_cookie

builddir:
	mkdir -p build

CPP_STD=--std=c++20


ifeq ($(shell uname -s),Darwin)
OS_SPECIFIC_INCLUDES=-I /opt/homebrew/opt/libomp/include
OS_SPECFIC_FLAGS=-Xpreprocessor -fopenmp
OS_SPECIFIC_LIBS= -L/opt/homebrew/opt/libomp/lib
endif

CLANG_EXEC=clang++ ${CPP_STD} ${OS_SPECFIC_FLAGS} ${OS_SPECIFIC_LIBS} -lomp

build/%.o: src/%.cpp
	clang++ ${CPP_STD} ${OS_SPECFIC_FLAGS} -c $< -o $@ ${FLAGS} ${OS_SPECIFIC_INCLUDES} $(TEST_FLAGS)

pagerank: build/prank_parallel.o
	${CLANG_EXEC} build/prank_parallel.o -o pagerank

eval: build/measure.o
	${CLANG_EXEC} ${<} -o $@

clean: clean_cookie
	rm -rf build 2>/dev/null

.PHONY: clean_cookie
clean_cookie:
	rm -rf *_cookie 2>/dev/null