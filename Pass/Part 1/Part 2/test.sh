#clang++ -c p12.cpp `llvm-config --cxxflags`;
#clang++ -shared -o pass.so p12.o `llvm-config --ldflags`
opt -load ./pass.so -p12 <../../../Test/hello.bc> /dev/null

