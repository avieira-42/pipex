#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv, char **envp)
{
    int fd[2];
    pid_t pid1, pid2;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <cmd1> <cmd2>\n", argv[0]);
        return 1;
    }

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return 1;
    }

    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid1 == 0)
    {
        // First child: executes cmd1, writes to pipe
        close(fd[0]);                // Close unused read end
        dup2(fd[1], STDOUT_FILENO);  // Redirect stdout to pipe write end
        close(fd[1]);

        char *cmd1 = argv[1];
        char *args1[] = {cmd1, NULL};

        // You should replace the path here with full path or logic to find it
        execve(cmd1, args1, envp);

        perror("execve cmd1 failed");
        exit(1);
    }

    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid2 == 0)
    {
        // Second child: executes cmd2, reads from pipe
        close(fd[1]);                // Close unused write end
        dup2(fd[0], STDIN_FILENO);   // Redirect stdin to pipe read end
        close(fd[0]);

        char *cmd2 = argv[2];
        char *args2[] = {cmd2, NULL};

        // You should replace the path here with full path or logic to find it
        execve(cmd2, args2, envp);

        perror("execve cmd2 failed");
        exit(1);
    }

    // Parent closes pipe fds and waits for children
    close(fd[0]);
    close(fd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}
