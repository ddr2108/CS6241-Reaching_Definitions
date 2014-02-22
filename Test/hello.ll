; ModuleID = 'hello.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [12 x i8] c"%d %d %d %d\00", align 1
@asd = common global i32 0, align 4

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %k = alloca i32*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %z = alloca i32, align 4
  store i32 0, i32* %retval
  store i32 7, i32* %i, align 4
  store i32 9, i32* %z, align 4
  %0 = load i32* %i, align 4
  %add = add nsw i32 %0, 9
  store i32 %add, i32* %j, align 4
  store i32 8, i32* %i, align 4
  store i32 8, i32* %j, align 4
  store i32* %i, i32** %k, align 8
  %1 = load i32** %k, align 8
  store i32 5, i32* %1, align 4
  %2 = load i32* %i, align 4
  store i32 %2, i32* %j, align 4
  %3 = load i32* %i, align 4
  %4 = load i32* %j, align 4
  %5 = load i32** %k, align 8
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([12 x i8]* @.str, i32 0, i32 0), i32 %3, i32 %4, i32* %5, i32 9)
  ret i32 2

}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 200694)"}
