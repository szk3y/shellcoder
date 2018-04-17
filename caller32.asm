[bits 32]
  global call_func

[section .text]
call_func:
  mov eax, [esp+4]
  call eax
  ret
