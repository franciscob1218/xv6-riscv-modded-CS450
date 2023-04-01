#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


#define ADDR_VALID (void*)0x4000
#define ADDR_INVALID (void*)0x80000000

int main(int argc, char *argv[]) {
    // Call whereIs() with a valid virtual address
    int frame_num = whereIs(ADDR_VALID);
    if (frame_num >= 0) {
        fprintf(1, "Physical frame number of %p: %d\n", ADDR_VALID, frame_num);
    } else {
        fprintf(1, "Error: virtual address %p is invalid\n", ADDR_VALID);
    }

    // Call whereIs() with an invalid virtual address
    frame_num = whereIs(ADDR_INVALID);
    if (frame_num >= 0) {
        fprintf(1, "Physical frame number of %p: %d\n", ADDR_INVALID, frame_num);
    } else {
        fprintf(1, "Error: virtual address %p is invalid\n", ADDR_INVALID);
    }

    exit(0);
}

/*

#define PAGE_SIZE 4096

int main(int argc, char *argv[])
{
    char *address = (char *)0x100000;

    printf("Virtual Address: %p\n", address);

    int pfn = whereIs(address);

    if (pfn < 0)
        printf("Error: Invalid Address\n");
    else
        printf("Physical Frame Number: %d\n", pfn);

    address = (char *)(PAGE_SIZE - 1);

    printf("Virtual Address: %p\n", address);

    pfn = whereIs(address);

    if (pfn < 0)
        printf("Error: Invalid Address\n");
    else
        printf("Physical Frame Number: %d\n", pfn);

    return 0;
}



*/
