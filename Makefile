all: builddir pagerank eval

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
	clang++ ${CPP_STD} ${OS_SPECFIC_FLAGS} -c $< -o $@ ${FLAGS} ${OS_SPECIFIC_INCLUDES}

pagerank: build/prank_parallel.o
	${CLANG_EXEC} build/prank_parallel.o -o pagerank

eval: build/measure.o
	${CLANG_EXEC} ${<} -o $@

clean:
	rm -rf build 2>/dev/null
