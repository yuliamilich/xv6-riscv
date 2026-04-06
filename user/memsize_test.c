#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
    int initial_size, after_alloc, after_free;
    char *ptr;

    // Print initial memory size
    initial_size = memsize();
    printf("Initial memory size: %d bytes\n", initial_size);

    // Allocate 20KB
    ptr = malloc(20480);
    if (ptr == 0)
    {
        printf("malloc failed\n");
        exit(1);
    }

    // Print memory size after allocation
    after_alloc = memsize();
    printf("Memory size after 20KB allocation: %d bytes\n", after_alloc);

    // Free the allocated memory
    free(ptr);

    // Print memory size after freeing
    after_free = memsize();
    printf("Memory size after freeing: %d bytes\n", after_free);

    exit(0);
}