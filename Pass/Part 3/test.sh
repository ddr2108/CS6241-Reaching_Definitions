clang++ -c p3.cpp `llvm-config --cxxflags`;
clang++ -shared -o pass.so p3.o `llvm-config --ldflags`
opt -load ./pass.so -p3 <../../Test/hello.bc> /dev/null

