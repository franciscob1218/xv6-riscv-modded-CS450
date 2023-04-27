#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

void
walk_directory(char *path)
{
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "Cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "Cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type == T_FILE) {
    // Print the inode number of the file
    printf("%d\n", st.ino);
  } else if (st.type == T_DIR) {
    // Recursively walk through the directory
    char buf[512];
    char *p;
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0) continue;
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (stat(buf, &st) < 0) {
        fprintf(2, "Cannot stat %s\n", buf);
        continue;
      }
      if (st.ino == 0) continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
      walk_directory(buf);
    }
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    printf("Usage: imapWalker <path>\n");
    exit();
  }

  walk_directory(argv[1]);

  exit();
}
