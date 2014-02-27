clang++ -c p22.cpp `llvm-config --cxxflags`;
clang++ -shared -o pass.so p22.o `llvm-config --ldflags`
opt -load ./pass.so -p22 <../../../Test/hello.bc> result.bc
lli result.bc

