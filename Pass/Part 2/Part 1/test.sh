clang++ -c p21.cpp `llvm-config --cxxflags`;
clang++ -shared -o pass.so p21.o `llvm-config --ldflags`
opt -load ./pass.so -p21 <../../../Test/hello.bc> result.bc

