#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/fcntl.h"

// Helper function to construct a path for the inode
void inode_path(char *buf, int size, int inum) {
    char *p = buf;
    *p++ = '#';
    int n = size - 2; // Leave space for '#' and null terminator
    while (inum > 0 && n > 0) {
        *p++ = '0' + (inum % 10);
        inum /= 10;
        n--;
    }
    *p = '\0';
}

// A program that takes an inode number as an argument
// and prints its type, size, and number of links
void inodeReader(uint inum) {
    int fd;
    struct stat st;
    char path[16];

    // Construct a path for the inode
    inode_path(path, sizeof(path), inum);

    // Get the inode information using stat
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        printf("inodeReader: cannot open inode #%d\n", inum);
        return;
    }

    if (fstat(fd, &st) < 0) {
        printf("inodeReader: cannot stat inode #%d\n", inum);
        close(fd);
        return;
    }

    // Print the inode information
    printf("inode %d:\n", inum);
    printf("type: %d\n", st.type);
    printf("size: %d bytes\n", st.size);
    printf("links: %d\n", st.nlink);

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: inodeReader <inode number>\n");
        exit(1);
    }

    uint inum = atoi(argv[1]);

    if (inum < ROOTINO) {
        printf("inodeReader: invalid inode number\n");
        exit(1);
    }

    inodeReader(inum);
    exit(0);
}
