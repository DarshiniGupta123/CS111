//NAME:Darshini Gupta
//EMAIL:darshinigupta0711@gmail.com
//ID: 004928411
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

#define BUFFERSIZE 1024

#define INPUT 'i'
#define OUTPUT 'o'
#define SEGFAULT 's'
#define CATCH 'c'
#define DUMPCORE 'd'

char buffer[BUFFERSIZE];

struct option args[] = 
  {
   {"input", 1, NULL, INPUT}, //what does the 1 mean, the number of arguments
   {"output", 1, NULL, OUTPUT},
   {"segfault", 0, NULL, SEGFAULT},
   {"catch", 0, NULL, CATCH},
   {"dump-core", 0, NULL,  DUMPCORE},
    {0, 0, 0, 0}
  };
void sigsegv_handler() {
  fprintf(stderr, "%s\n", "Caught segmentation fault");
  exit(4);
}
int main(int argc, char *argv[]) {
  int in_fd1 = 0;
  int in_fd2 = 1; //do I need this or is it always stdout or 1
  int ifd = 0;
  int ofd = 0;
  int flag = 0;
  int flag2 = 0;
  int ret = getopt_long(argc, argv, "", args, NULL);
  while( ret != -1)  {

  switch (ret) {
  case SEGFAULT:
    flag2 = 1;
    break;
  case CATCH:
    flag += 1;
    break;
  case DUMPCORE:
    flag -= 1;
    break;
  case INPUT:
    //why won't it let me indent
    ifd = open(optarg, O_RDONLY);
    if(ifd >= 0){
      close(0);
      dup(ifd);
      close(ifd);
    }

    else{
      fprintf(stderr, "%s:%s\n", optarg, strerror(errno));
      exit(2);
    }

    break;
    
  case OUTPUT:
    ofd = creat(optarg, 0644);
    if (ofd >= 0) {
	close(1);
	dup(ofd);
	close(ofd);
    }
    else{
      fprintf(stderr, "%s:%s\n", optarg, strerror(errno));
      exit(3);
    }

    break;

  default:
      fprintf(stderr, "%s\n", "Unrecognized argument");
      exit(1);
      break;
  }
   
  ret = getopt_long(argc, argv, "", args, NULL);

  }
  
  if(flag <= 0 && flag2 == 1){
    if(flag < 0)
      exit(139);
    char * ptr = NULL;
    *ptr = 123; 
  }
  else if(flag > 0){
    char * ptr2 = NULL;
    signal(SIGSEGV, sigsegv_handler);
    *ptr2 = 123; 
    
  }
  else{
    while(1)
    {
      int x = read(in_fd1, buffer, 1024);
      if ( x <= 0 ) //End of file or error
        break;
      int y = write(in_fd2, buffer, 1024);
      if( y <= 0 )
        break;
    }
  }
  exit(0);

}


