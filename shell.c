#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "parse.h"

int main() {
  char *line;
  while ((line = readline(">>> ")) != NULL) {
    struct root *r = parse(line);
    if (!r->valid) {
      fprintf(stderr, "Error on parse: invalid cmd\n");
      exit(1);
    } else {
      struct command *cmd = r->first_command;
      int cmdIn = -1, cmdOut = -1, nextIn = -1, fds[2], p_id; 
      while (cmd != NULL){
	if (cmd->next != NULL){
	  p_id = pipe(fds);
	  if (p_id < 0) {
	    fprintf(stderr, "Error on pipe \n");
	    exit(1);
	  }
	  cmdOut = fds[1];
	  nextIn = fds[0];
	}
	// gotta build out writing to file...
	p_id = fork();
	if (p_id < 0) {
	  fprintf(stderr, "Error on fork \n");
	  exit(1);
	} else if (p_id == 0) {
	  if (cmd->outfile != NULL) {
	    cmdOut = open(cmd->outfile, O_WRONLY | O_CREAT, 0666);
	    close(1);
	    dup(cmdOut);
	    close(cmdOut);
	    execvp(*cmd->argv, cmd->argv);
	    fprintf(stderr, "Error on cmd exec\n");
	  }
	  if (cmdIn != -1) {
	    close(0);
	    dup(cmdIn);
	    close(cmdIn);
	  }
	  if (cmdOut != -1) {
	    close(1);
	    dup(cmdOut);
	    close(cmdOut);
	  }
	  execvp(*cmd->argv, cmd->argv);
	  fprintf(stderr, "Error on cmd exec\n");
	}
	if (cmd->next != NULL) {
	  if (cmdIn != -1) {
	    close(cmdIn);
	  }
	  if (cmdOut != -1) {
	    close(cmdOut);
	  }
	}
	
	cmd = cmd->next;
	cmdIn = cmdOut = -1;
	if (nextIn != -1) {
	  cmdIn = nextIn;
	  nextIn = -1;
	}
	wait(NULL);
      
   
	parse_end(r);
	free(line);
      }
    }
   
  }
  
}
