#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256
#define MAX_PASSWORD 10

#define SUCCESS "Password verified\n"
#define INVALID "Invalid password\n"
#define NO_USER "No such user\n"

int main(void) {
  char user_id[MAXLINE];
  char password[MAXLINE];

  /* The user will type in a user name on one line followed by a password 
     on the next.
     DO NOT add any prompts.  The only output of this program will be one 
	 of the messages defined above.
     Please read the comments in validate carefully.
   */

  if(fgets(user_id, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }
  if(fgets(password, MAXLINE, stdin) == NULL) {
      perror("fgets");
      exit(1);
  }

  // TODO
  if (strlen(user_id) > MAX_PASSWORD) {
      printf("%s", NO_USER);
      exit(3);
  }
  if (strlen(password) > MAX_PASSWORD) {
      printf("%s", INVALID);
      exit(1);
  }

  int fd[2];
  // create a pipe
  pipe(fd);

  int p = fork();

  if (p > 0) {
      close(fd[0]);
      write(fd[1], user_id, MAX_PASSWORD);
      write(fd[1], password, MAX_PASSWORD);
      close(fd[1]);

      int status;
      if (wait(&status) != -1)  {
          if (WIFEXITED(status)) {
              if (WEXITSTATUS(status) == 0) {
                  printf("%s", SUCCESS);
              }
              if (WEXITSTATUS(status) == 2) {
                  printf("%s", INVALID);
              } 
              if (WEXITSTATUS(status) == 3) {
                  printf("%s", NO_USER);
              } 
          }
      }
  }
  else if (p == 0) {
      close(fd[1]);
      dup2(fd[0], fileno(stdin));
      close(fd[0]);
      execl("./validate", "validate", NULL);
      exit(1);
  }
  else {
      exit(1);
  }

  return 0;
}
