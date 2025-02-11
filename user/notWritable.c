#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "stdint.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Please provide a virtual address.\n");
    exit(1);
  }

  char *va_str = argv[1];
  int va = atoi(va_str);

  int result = notWritable((char*) (intptr_t) va);

  if (result == -1) {
    printf("Error: invalid virtual address.\n");
  } else if (result == 0) {
    printf("The page at virtual address %s was already not writable.\n", va_str);
  } else {
    printf("The page at virtual address %s is now not writable.\n", va_str);
  }

  exit(0);
}