#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char **argv)
{
    pid_t pid = fork();
    char **args = argv + 1;

    if (pid < 0)
        fprintf(stderr, "Failed to fork.\n");
    else if (pid == 0 && argc > 1)
        fprintf(stderr, "Running '%s' with %d arg(s)\n", *args, argc - 2),
        execvp(*args, args);
    else if (pid > 0  && argc > 1)
    {
        char   info[1024] = {0};
        double delay_sec = 0.00;

        sprintf(info, "start=$(date +%%s.%%N);                         \
                       while [ -e /proc/%d ]; do                       \
                       end=$(date +%%s.%%N);                           \
                       size=\"$(pmap -d %d | tail -n 1 |               \
                                awk '{print \"memory = \" $4}'|        \
                                sed 's/K$/000/g')\";                   \
                       if [ -z \"$size\" ]; then exit 0; fi;           \
                       time=$(echo \"scale=9; ($end-$start)/1\" | bc); \
                       printf \"%%10.6f sec  %%s\\n\" $time \"$size\"; \
                       sleep %f; done &", pid, pid, delay_sec);
        system(info);

        int status, ret;
        waitpid(pid, &status, 0);
        usleep(10000);

        if (!WIFEXITED(status))
            ret = EXIT_FAILURE,
            fprintf(stderr, "Process terminated abnormally.\n");
        else
            ret = WEXITSTATUS(status),
            fprintf(stderr, "Process (%d) returned %d (0x%X)\n", pid, ret, ret);

        exit(ret);
    }

    exit(EXIT_FAILURE);
}
