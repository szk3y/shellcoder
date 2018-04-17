[bits 32]
[org 0x1234000]

entry:
  mov edx, hello_end-hello_begin ; size
  mov ecx, hello_begin ; str
  mov ebx, 1 ; fd
  mov eax, 4 ; write
  int 0x80
  ret

hello_begin:
  db 'Hello 32bit', 0x0a

hello_end:
