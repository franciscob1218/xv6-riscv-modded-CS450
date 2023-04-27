#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// Helper function to concatenate two strings
void strjoin(char *dest, const char *s1, const char *s2) {
    while (*s1) {
        *dest++ = *s1++;
    }
    while ((*dest++ = *s2++));
}

// A function to walk the directory tree starting from a given directory
void directoryWalker(int fd, char *path) {
    struct dirent de;
    struct stat st;

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) continue;

        // Construct the full path for the entry
        char fullpath[512];
        strjoin(fullpath, path, "/");
        strjoin(fullpath + strlen(path), "/", de.name);

        if (stat(fullpath, &st) < 0) {
            printf("stat failed for %s\n", fullpath);
            continue;
        }

        if (st.type == T_DIR && strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
            int newfd = open(fullpath, 0);
            if (newfd < 0) {
                printf("cannot open directory %s\n", fullpath);
                continue;
            }
            directoryWalker(newfd, fullpath);
            close(newfd);
        } else {
            printf("%s\t%d\n", fullpath, de.inum);
        }
    }
}

int main(int argc, char *argv[]) {
    // Check if a directory is given as an argument
    if (argc != 2) {
        fprintf(2, "Usage: %s directory\n", argv[0]);
        exit(1);
    }

    // Call directoryWalker() with the given directory
    int fd = open(argv[1], 0);
    if (fd < 0) {
        printf("cannot open directory %s\n", argv[1]);
        exit(1);
    }

    directoryWalker(fd, argv[1]);
    close(fd);
    exit(0);
}
