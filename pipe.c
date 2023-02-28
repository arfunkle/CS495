#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>


int pipe3(struct command *cmd) {
  int fds[2];
  int p_id = pipe(fds);
  if (p_id < 0) {
    fprintf(stderr,"Error on pipe\n");
    exit(1);
  }
  else {
    p_id = fork();
    if (p_id < 0) {
      fprintf(stderr, "Error on fork 1\n");
      exit(1);
    }
    else if (p_id == 0) {
      close(0);
      (void) dup(fds[0]);
      close(fds[1]);
      close(fds[0]);
      execv(cmd->argv);  // runs word count
    } else {
      while () {
	
      }
    }
  }
}
