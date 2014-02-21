clang++ -c p31.cpp `llvm-config --cxxflags`;
clang++ -shared -o pass.so p31.o `llvm-config --ldflags`
opt -load ./pass.so -p31 <../../Test/hello.bc> /dev/null

