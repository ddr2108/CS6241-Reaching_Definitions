#clang++ -c p13.cpp `llvm-config --cxxflags`;
#clang++ -shared -o pass.so p13.o `llvm-config --ldflags`
opt -load ./pass.so -p13 <../../../Test/hello.bc> /dev/null

