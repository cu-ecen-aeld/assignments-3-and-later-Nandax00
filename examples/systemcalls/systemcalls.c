#include "systemcalls.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd) { return system(cmd) == 0; }

bool wait_for_system_call(char **cmd, const char *outputfile) {
  int fd;
  if (outputfile) {
    fd = open(outputfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (fd < 0) {
      printf("Could not open output file \"%s\"\n", outputfile);
      return false;
    }
  }

  pid_t pid = fork();
  if (pid == -1) {
    printf("Error during fork\n");
    if (outputfile) {
      close(fd);
    }
    return false;
  }

  if (pid == 0) {
    if (outputfile) {
      if (dup2(fd, 1) < 0) {
        printf("Error calling dup2\n");
        exit(-1);
      }
      close(fd);
    }

    execv(cmd[0], cmd);
    printf("Error starting \"%s\", error: %s\n", cmd[0], strerror(errno));
    exit(-1);
  }

  if (outputfile) {
    close(fd);
  }
  int stat;
  int rv = wait(&stat);
  if (rv == -1) {
    return false;
  } else if (WIFEXITED(stat)) {
    return WEXITSTATUS(stat) == 0;
  } else {
    return false;
  }
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

    int rv = wait_for_system_call(command, NULL);

    va_end(args);

    return rv;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

    int rv = wait_for_system_call(command, outputfile);

    va_end(args);

    return rv;
}
