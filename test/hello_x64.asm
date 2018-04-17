[bits 64]
[org 0x1234000]

entry:
  mov rdx, hello_end-hello_begin ; size
  mov rsi, hello_begin ; str
  mov rdi, 1 ; fd
  mov rax, 1 ; write
  syscall
  ret

hello_begin:
  db 'Hello x64', 0x0a

hello_end:
