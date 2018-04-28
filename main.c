#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PAGE_SIZE  0x1000
#define PRINT_RANGE 0x100

typedef void (*caller_t)(void);

void print_memory(const unsigned char* memory, size_t size)
{
  memory -= (size_t)memory & 0xf;
  for(size_t i = 0; i < size; i++) {
    if(i % 16 == 0) {
      printf("%08zx | ", (size_t)(memory+i));
    }
    printf("%02hhx", memory[i]);
    if(i % 16 == 15) {
      putchar('\n');
    } else if(i % 16 == 7) {
      putchar(' ');
      putchar(' ');
    } else {
      putchar(' ');
    }
  }
  putchar('\n');
  fflush(stdout);
}

void read_shellcode(void* page_base, long long offset, int fd)
{
  ssize_t nread;
  ssize_t total_nread = 0;

  while(1) {
    nread = read(fd, page_base + offset + total_nread, PAGE_SIZE);
    total_nread += nread;
    if(nread == 0) { // EOF
      break;
    } else if(nread < 0) { // error
      perror("read");
      exit(1);
    }
  }
}

int main(int argc, char** argv)
{
  long long req_addr;
  long long offset;
  void* aligned_req_addr;
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
  req_addr = strtoll(argv[2], NULL, 16);
  offset = req_addr % PAGE_SIZE;
  aligned_req_addr = NULL + req_addr - offset;
  return_addr = mmap(aligned_req_addr, st.st_size + offset,
      PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS,
      -1, 0);
  if(return_addr == (void*)(-1)) {
    perror("mmap");
    exit(1);
  }
  if(return_addr != aligned_req_addr) {
    fputs("Failed to mmap at requested address\n", stderr);
    fprintf(stderr, "aligned_req_addr = %p\n", aligned_req_addr);
    fprintf(stderr, "return_addr  = %p\n", return_addr);
    exit(1);
  }

  read_shellcode(return_addr, offset, fd);
  puts("Before:");
  print_memory(NULL + req_addr, PRINT_RANGE);

  call_shellcode = return_addr + offset;
  call_shellcode();

  puts("After:");
  print_memory(NULL + req_addr, PRINT_RANGE);

  munmap(return_addr, st.st_size + offset);
  close(fd);
  exit(EXIT_SUCCESS);
}
