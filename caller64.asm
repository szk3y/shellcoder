[bits 64]
  global call_func

[section .text]
call_func:
  mov rax, rdi
  call rax
  ret
