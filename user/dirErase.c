#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"
#include "fcntl.h"

#define MAX_PATH_LENGTH 256
#define MAX_DIR_ENTRIES 64

struct dirent {
  ushort inum;
  char name[DIRSIZ];
};

void unlink_all_hard_links(char *filename) {
  // Open the file corresponding to the given filename
  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    printf("Error: could not open file %s\n", filename);
    return;
  }

  // Retrieve the inode number of the file
  struct stat st;
  if (fstat(fd, &st) < 0) {
    printf("Error: could not stat file %s\n", filename);
    close(fd);
    return;
  }
  int inum = st.ino;

  // Iterate through all directory entries in the file system
  char path[MAX_PATH_LENGTH] = "/";
  int dir_fd = open(path, O_RDONLY);
  struct dirent entries[MAX_DIR_ENTRIES];
  int n;
  while ((n = read(dir_fd, entries, sizeof(entries))) > 0) {
    for (int i = 0; i < n / sizeof(struct dirent); i++) {
      // Check if the directory entry points to the file
      if (entries[i].inum == inum) {
        // Unlink the directory entry
        char entry_path[MAX_PATH_LENGTH];
        strcpy(entry_path, path);
        strcat(entry_path, entries[i].name);
        if (unlink(entry_path) < 0) {
          printf("Error: could not unlink directory entry %s\n", entry_path);
        }
      }
    }
  }
  close(dir_fd);

  // Close the file
  close(fd);
}