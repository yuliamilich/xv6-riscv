#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int pid1 = getpid();
    int pid2 = fork();
    int message1 = 2;
    int message2 = -2;

    if (pid2 == 0) // child
    {
        for (;;)
        {
            int value = co_yield(pid1, message1);
            printf("Child received: %d\n", value);
            message1 += 1;
        }
    }
    else // parent
    {
        printf("child pid is %d \n", pid2);
        for (;;)
        {
            int value = co_yield(pid2, message2);
            printf("Parent received: %d\n", value);
            message2 -= 1;
        }
    }
}
