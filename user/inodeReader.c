#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"
#include "../kernel/fs.h"
#include "fcntl.h"

// A program that takes an inode number as an argument
// and prints its type, size, number of links and block addresses
void inodeReader(int inum) {
  int fd;
  struct stat st;
  char buf[BSIZE];
  struct dinode *dip;

  // Open the device file
  fd = open("xv6.img", O_RDONLY);
  if (fd < 0) {
    printf(2, "inodeReader: cannot open xv6.img\n");
    exit(1);
  }

  // Read the superblock
  if (read(fd, buf, BSIZE) != BSIZE) {
    printf(2, "inodeReader: cannot read superblock\n");
    close(fd);
    exit(1);
  }
  struct superblock *sb = (struct superblock *)buf;

  // Calculate the block number of the inode table
  int itable = sb->inodestart;

  // Seek to the block containing the inode
  if (lseek(fd, itable * BSIZE + inum * sizeof(struct dinode), SEEK_SET) < 0) {
    printf(2, "inodeReader: cannot seek to inode\n");
    close(fd);
    exit(1);
  }

  // Read the inode
  if (read(fd, buf, sizeof(struct dinode)) != sizeof(struct dinode)) {
    printf(2, "inodeReader: cannot read inode\n");
    close(fd);
    exit(1);
  }
  dip = (struct dinode *)buf;

  // Check if the inode is valid
  if (dip->type == 0) {
    printf(2, "inodeReader: invalid inode\n");
    close(fd);
    exit(1);
  }

  // Fill in the stat structure
  st.type = dip->type;
  st.dev = sb->dev;
  st.ino = inum;
  st.nlink = dip->nlink;
  st.size = dip->size;

  // Print the inode information
  printf(1, "inode %d:\n", inum);
  printf(1, "type: %d\n", st.type);
  printf(1, "size: %d bytes\n", st.size);
  printf(1, "links: %d\n", st.nlink);
  printf(1, "blocks:");
  
  // Print the block addresses
  for (int i = 0; i < NDIRECT + NINDIRECT; i++) {
    uint baddr;
    if (i < NDIRECT) {
      baddr = dip->addrs[i];
    } else {
      // Read the indirect block
      if (lseek(fd, dip->addrs[NDIRECT] * BSIZE + (i - NDIRECT) * sizeof(uint), SEEK_SET) < 0) {
        printf(2, "inodeReader: cannot seek to indirect block\n");
        break;
      }
      if (read(fd, &baddr, sizeof(uint)) != sizeof(uint)) {
        printf(2, "inodeReader: cannot read indirect block\n");
        break;
      }
    }
    if (baddr == 0) break; // No more blocks
    printf(1, " %d", baddr);
  }
  
  printf(1, "\n");

  // Close the device file
  close(fd);
}

int main(int argc, char *argv[]) {
  
  if (argc != 2) {
    printf(2, "Usage: inodeReader <inode number>\n");
    exit(1);
  }

  int inum = atoi(argv[1]);
  
  if (inum < ROOTINO || inum >= NINODES) {
    printf(2, "inodeReader: invalid inode number\n");
    exit(1);
  }

  inodeReader(inum);

  
}