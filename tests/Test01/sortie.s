.text
.global main
main:
  ## prologue
  pushq %rbp # save %rbp on the stack
  movq %rsp, %rbp # define %rbp for the current function
  ## contenu
  ## epilogue
  popq %rbp # restore %rbp from the stack
  ret

