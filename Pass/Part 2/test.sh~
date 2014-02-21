clang++ -c p2.cpp `llvm-config --cxxflags`;
clang++ -shared -o pass.so p2.o `llvm-config --ldflags`
opt -load ./pass.so -p2 <../../Test/hello.bc> /dev/null

