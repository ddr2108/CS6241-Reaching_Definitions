; ModuleID = 'hello.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@asd = common global i32 0, align 4

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %i = alloca i32, align 4
  %k = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %retval
  store i32 8, i32* %i, align 4
  store i32 11, i32* %j, align 4
  %0 = load i32* %i, align 4
  %add = add nsw i32 2, %0
  store i32 %add, i32* %k, align 4
  br label %while.cond

while.cond:                                       ; preds = %if.end, %if.then, %entry
  %call = call i32 @rand() #2
  %rem = srem i32 %call, 20
  %cmp = icmp eq i32 %rem, 5
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %1 = load i32* %i, align 4
  %2 = load i32* %j, align 4
  %add1 = add nsw i32 %2, %1
  store i32 %add1, i32* %j, align 4
  %3 = load i32* %j, align 4
  %cmp2 = icmp sgt i32 %3, 25
  br i1 %cmp2, label %if.then, label %if.else

if.then:                                          ; preds = %while.body
  store i32 15, i32* %j, align 4
  br label %while.cond

if.else:                                          ; preds = %while.body
  store i32 9, i32* %j, align 4
  br label %if.end

if.end:                                           ; preds = %if.else
  store i32 3, i32* %i, align 4
  store i32 5, i32* %j, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  store i32 1, i32* %i, align 4
  store i32 10, i32* %j, align 4
  ret i32 2
}

; Function Attrs: nounwind
declare i32 @rand() #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5 (trunk 200694)"}
