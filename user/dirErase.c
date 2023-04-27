#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MAX_PATH_LENGTH 256

// Iterate through all directory entries and unlink any entries that point to the specified inode
void unlink_all_hard_links(int inum) {
    int dir_fd;
    struct dirent de;
    char path[MAX_PATH_LENGTH] = "/";
    int n;

    // Open the root directory
    dir_fd = open(path, O_RDONLY);
    if (dir_fd < 0) {
        printf("Error: could not open root directory\n");
        return;
    }

    // Read the directory entries
    while ((n = read(dir_fd, &de, sizeof(de))) > 0) {
        if (de.inum == 0) {
            continue;
        }

        // Check if the directory entry points to the file
        if (de.inum == inum) {
            // Unlink the directory entry
            if (unlink(de.name) < 0) {
                printf("Error: could not unlink directory entry %s\n", de.name);
            }
        }
    }

    close(dir_fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    // Open the file corresponding to the given filename
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf("Error: could not open file %s\n", argv[1]);
        exit(1);
    }

    // Retrieve the inode number of the file
    struct stat st;
    if (fstat(fd, &st) < 0) {
        printf("Error: could not stat file %s\n", argv[1]);
        close(fd);
        exit(1);
    }
    int inum = st.ino;

    // Close the file
    close(fd);

    // Unlink all hard links to the file
    unlink_all_hard_links(inum);

    exit(0);
}
