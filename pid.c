#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
        pid_t c_pid;
        pid_t p_pid;

        c_pid = getpid();
        p_pid = getppid();

        //現在のプロセスID
        printf("PID:%d\n", c_pid);

        //親のプロセスID
        printf("PPID:%d\n", p_pid);

        return 0;
}
