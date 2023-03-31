#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

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