#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef void (*caller_t)(void);

int main(int argc, char** argv)
{
  void* requested_addr;
  void* return_addr;
  int fd;
  struct stat st;
  caller_t call_shellcode;

  if(argc != 3) {
    fprintf(stderr, "Usage: %s <shellcode> <base_addr>\n", argv[0]);
    exit(1);
  }

  fd = open(argv[1], O_RDONLY);
  if(fd < 0) {
    perror("open");
    exit(1);
  }
  fstat(fd, &st);
  requested_addr = NULL + strtoll(argv[2], NULL, 16);
  return_addr = mmap(requested_addr, st.st_size,
      PROT_READ | PROT_EXEC, MAP_SHARED, fd, 0);
  if(return_addr != requested_addr) {
    fputs("Failed to mmap at requested address\n", stderr);
    fprintf(stderr, "requested_addr = %p\n", requested_addr);
    fprintf(stderr, "return_addr    = %p\n", return_addr);
    exit(1);
  }

  call_shellcode = return_addr;
  call_shellcode();

  munmap(return_addr, st.st_size);
  close(fd);
  exit(EXIT_SUCCESS);
}
