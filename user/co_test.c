#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// static void test_infinite()
// {
//     int pid1 = getpid();
//     int pid2 = fork();
//     int message1 = 2;
//     int message2 = -2;

//     if (pid2 == 0) // child
//     {
//         for (;;)
//         {
//             int value = co_yield(pid1, message1);
//             printf("Child received: %d\n", value);
//             message1 += 1;
//         }
//     }
//     else // parent
//     {
//         printf("child pid is %d \n", pid2);
//         for (;;)
//         {
//             int value = co_yield(pid2, message2);
//             printf("Parent received: %d\n", value);
//             message2 -= 1;
//         }
//     }
// }

static void fail(const char *msg)
{
    printf("FAIL: %s\n", msg);
    exit(1);
}

static void test_invalid_pid(void)
{
    if (co_yield(0, 123) != -1)
        fail("pid 0 should fail");
    if (co_yield(-1, 123) != -1)
        fail("pid -1 should fail");
    if (co_yield(getpid(), 123) != -1)
        fail("self pid should fail");
    if (co_yield(99999, 123) != -1)
        fail("nonexistent pid should fail");
    printf("test_invalid_pid: OK\n");
}

static void test_dead_pid(void)
{
    int pid = fork();
    if (pid < 0)
        fail("fork failed");
    if (pid == 0)
    {
        exit(0);
    }

    int st = 0;
    if (wait(&st) != pid)
        fail("wait failed");
    if (co_yield(pid, 7) != -1)
        fail("dead pid should fail");
    printf("test_dead_pid: OK\n");
}

static void test_ping_pong(int rounds)
{
    int parent_pid = getpid();
    int pid = fork();
    if (pid < 0)
        fail("fork failed");

    // const int rounds = 10;
    if (pid == 0)
    {
        int msg = -2;
        for (int i = 0; i < rounds; i++)
        {
            int got = co_yield(parent_pid, msg);
            if (got != (1000 + i))
                fail("child got wrong message");
            msg -= 1;
        }
        exit(0);
    }
    else
    {
        int msg = 1000;
        for (int i = 0; i < rounds; i++)
        {
            int got = co_yield(pid, msg);
            if (got != (-2 - i))
                fail("parent got wrong message");
            msg += 1;
        }
        int st = 0;
        if (wait(&st) != pid)
            fail("wait failed");
        if (st != 0)
            fail("child exit status");
        printf("test_ping_pong: OK\n");
    }
}

int main(int argc, char *argv[])
{
    test_invalid_pid();
    test_dead_pid();
    test_ping_pong(10);

    printf("co_test: OK\n");
    exit(0);
}