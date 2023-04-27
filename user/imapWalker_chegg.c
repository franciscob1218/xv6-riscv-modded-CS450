#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"
#include "../kernel/fs.h"

int
main(int argc, char *argv[])
{
  struct superblock sb;
  struct dinode di;
  int i, j, ninodes;

  if(argc < 2){
    printf(2, "Usage: imapWalker file_system_image\n");
    exit(1);
  }

  int fd = open(argv[1], 0);
  if(fd < 0){
    printf(2, "Cannot open file system image %s\n", argv[1]);
    exit(1);
  }

  // read superblock to get number of inodes
  if(read(fd, &sb, sizeof(sb)) != sizeof(sb)){
    printf(2, "Error reading superblock\n");
    exit(1);
  }

  ninodes = sb.ninodes;

  // walk through inode bitmap and print out allocated inodes
  for(i = 0; i < ninodes; i++){
    if((i % IPB) == 0){
      // read block containing inode bitmap
      uint blockno = BBLOCK(i, sb.nblocks);
      char bitmap[BSIZE];
      if(read(fd, bitmap, BSIZE) != BSIZE){
        printf(2, "Error reading inode bitmap\n");
        exit();
      }
    }
    if((bitmap[(i / 8) % BSIZE] & (1 << (i % 8))) != 0){
      // inode is allocated, read inode and print inode number
      if(read(fd, &di, sizeof(di)) != sizeof(di)){
        printf(2, "Error reading inode\n");
        exit();
      }
      printf(1, "Inode %d is allocated\n", i);
    }
  }

  close(fd);
  exit();
} 
