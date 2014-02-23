clang++ -c p11.cpp `llvm-config --cxxflags`;
clang++ -shared -o pass.so p11.o `llvm-config --ldflags`
opt -load ./pass.so -p11 <../../../Test/hello.bc> /dev/null

