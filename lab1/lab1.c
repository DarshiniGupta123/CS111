#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> 
#include <errno.h>
#include <signal.h>
#include <libio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define BUFFERSIZE 1024

#define READONLY 'a'
#define WRITEONLY 'b'
#define COMMAND 'c'
#define APPEND 'd'
#define CLOEXEC 'e'
#define CREATE 'f'
#define DSYNC 'g'
#define EXCL 'h'
#define NOFOLLOW 'i'
#define NONBLOCK 'j'
#define RSYNC 'k'
#define SYNC 'l'
#define TRUNC 'm'
#define RDWR 'n'
#define ABORT 'o'
#define CATCH 'p'
#define VERBOSE 'q'
#define PIPE 'r'
#define DEFAULT 's'
#define IGNORE 't'
#define PAUSE 'u'
#define WAIT 'v'
#define CHDIR 'w'
#define CLOSE 'x'
#define DIRECTORY 'y'
#define PROFILE 'z'


char buffer[BUFFERSIZE];

int verbose_flag = 0;
int flag = 0;
int pipe_options = 0;
int profile_flag = 0;

struct option long_options[] =
  {
   {"rdonly", 1, NULL, READONLY},
   {"wronly", 1, NULL, WRITEONLY},
   {"verbose", 0, NULL, VERBOSE},
   {"command", 1, NULL, COMMAND},
   {"append", 0, NULL, APPEND},
   {"cloexec", 0, NULL, CLOEXEC},
   {"creat", 0, NULL, CREATE},
   {"dysnc", 0, NULL, DSYNC},
   {"excl", 0, NULL, EXCL},
   {"nofollow", 0, NULL, NOFOLLOW},
   {"nonblock", 0, NULL, NONBLOCK},
   {"rsync", 0, NULL, RSYNC},
   {"sync", 0, NULL, SYNC},
   {"trunc", 0, NULL, TRUNC},
   {"rdwr", 1, NULL, RDWR},
   {"abort", 0, NULL, ABORT},
   {"catch", 1, NULL, CATCH},
   {"pipe", 0, NULL, PIPE},
   {"default", 1, NULL, DEFAULT},
   {"ignore", 1, NULL, IGNORE},
   {"pause", 0, NULL, PAUSE},
   {"wait", 0, NULL, WAIT},
   {"chdir", 1, NULL, CHDIR},
   {"close", 1, NULL, CLOSE},
   {"directory", 0, NULL, DIRECTORY},
   {"profile", 0, NULL, PROFILE},
   {0, 0, 0, 0}
  };

struct Children{
  pid_t child_process;
  int startIndex;
  int endIndex;
};
  
void sigsegv_handler(int signum) {
  fprintf(stderr, "%d caught", signum);
  exit(signum);
  
}


void verbose(int x, int argc, char* argv[]){
  printf("%s ", argv[x]);
  fflush(stdout);
  x++;
  for(int i = x; i < argc; i++){
        char* word = argv[i];
        if(word[0] == '-' && word[1] == '-')
	  break;
	else{
	   printf("%s ", argv[i]);
	   fflush(stdout);
	  }
   }

  fprintf(stdout, "\n");
  fflush(stdout);
}

void time_proc(struct timeval startUTime, struct timeval startSTime, struct timeval endUTime, struct timeval endSTime) {
  long int differenceU;
  long int differenceUM;
  long int differenceS;
  long int differenceSM;
  
  differenceU = (long int)(endUTime.tv_sec - startUTime.tv_sec);
  differenceUM = (long int)(endUTime.tv_usec - startUTime.tv_usec);
  differenceS = (long int)(endSTime.tv_sec - startSTime.tv_sec);
  differenceSM = (long int)(endSTime.tv_usec - startSTime.tv_usec);

  printf("User Time: %ld.%.6lds ", differenceU, differenceUM);
  printf("System Time: %ld.%.6lds\n", differenceS, differenceSM);
  fflush(stdout);
}



int main(int argc, char* argv[]) {

  struct rusage r_usage;

  struct timeval startUTime;
  struct timeval startSTime;
  struct timeval endUTime;
  struct timeval endSTime;

   
  int sig2 = -1; 
  int ret = 0;
  
  int fd = 0;
  int fds[BUFFERSIZE];
  int fds_index = 0;
  

  char* inidx = 0;
  char* outidx = 0;
  char* erridx = 0;

  int index = -1;

  struct Children *arr2 = malloc(argc * sizeof(struct Children));
  int counter = 0;

  optind = 1;
  while((ret = getopt_long(argc, argv, "", long_options, NULL)) != -1){
    /*if(verbose_flag == 1){
        int x = optind;
        verbose(x, argc, argv);
	}*/
    
    switch(ret) {
    case PROFILE:
       if(verbose_flag == 1){
        int x = optind;
        x = x-1;
        verbose(x, argc, argv);
        }

      profile_flag = 1;
      break;
      
    case PIPE:
      if(verbose_flag == 1){
        int x = optind;
        x = x-1;
        verbose(x, argc, argv);
	}

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      
      int child_array[2];
      if(pipe(child_array) < 0)
     	fprintf(stderr, "%s\n", strerror(errno));	
	
      fds[fds_index] = child_array[0];
      fds_index++;
      fds[fds_index] = child_array[1];
      fds_index++;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;

    case DIRECTORY:
      if(verbose_flag == 1){
        int x = optind;
        x = x-1;
        verbose(x, argc, argv);
      }

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;


      pipe_options |= O_DIRECTORY;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;
      
    case CLOSE:
      if(verbose_flag == 1){
        int x = optind;
        x = x-2;
        verbose(x, argc, argv);
	}
      //index = n;
      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      int n = atoi(optarg);
      index = n;
      if(close(fds[n]) < 0){
	fprintf(stderr, "%s\n", strerror(errno));
	flag = 1;
      }

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;

    case VERBOSE:
      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      verbose_flag = 1;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;
      
    case ABORT:
      if(verbose_flag == 1)
      {
	int x = optind;
	x = x-1;
	verbose(x, argc, argv);
      }

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      raise(11);
      exit(139);

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;
      
    case CATCH:
      if(verbose_flag == 1){
      int x = optind;
      x = x-2;
      verbose(x, argc, argv);
      }
      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      sig2 = atoi(optarg);
      signal(sig2, sigsegv_handler);

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;

    case DEFAULT:
      if(verbose_flag == 1){
	int x = optind;
	x = x-2;
	verbose(x, argc, argv);
      }
      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      sig2 = atoi(optarg);
      signal(sig2, SIG_DFL);

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;

    case IGNORE:
      if(verbose_flag == 1){
        int x = optind;
        x = x-2;
        verbose(x, argc, argv);
      }
      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

       sig2 = atoi(optarg);
       signal(sig2, SIG_IGN);

       if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

       break;

    case PAUSE:
      if(verbose_flag == 1){
        int x = optind;
        x = x-1;
        verbose(x, argc, argv);
      }
      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

       pause();

       if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

       break;
      
    case NOFOLLOW:
      if(verbose_flag == 1){
      int x = optind;
      x = x-1;
      verbose(x, argc, argv);
      }
      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      pipe_options |= O_NOFOLLOW;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;

    case RSYNC:
      if(verbose_flag == 1){
      int x = optind;
      x = x-1;
      verbose(x, argc, argv);
      }
      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      pipe_options |= O_RSYNC;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;

    case SYNC:
      if(verbose_flag == 1){
	int x = optind;
	x = x-1;
	verbose(x, argc, argv);
	}
      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      pipe_options |= O_SYNC;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;

    case TRUNC:
      if(verbose_flag == 1){
	int x = optind;
	x = x-1;
	verbose(x, argc, argv);
      }
      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;
 
      pipe_options |= O_TRUNC;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;
      
    case NONBLOCK:
      if(verbose_flag == 1){
	int x = optind;
	x = x-1;
	verbose(x, argc, argv);
      }

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      pipe_options |=  O_NONBLOCK;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;
      
    case EXCL:
      if(verbose_flag == 1){
	int x = optind;
	x = x-1;
	verbose(x, argc, argv);
      }

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      pipe_options |= O_EXCL;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;
      
    case DSYNC:
      if(verbose_flag == 1){
	int x = optind;
	x = x-1;
	verbose(x, argc, argv);
      }
      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      pipe_options |= O_DSYNC;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;
      
    case CREATE:
      if(verbose_flag == 1){
      int x = optind;
      x = x-1;
      verbose(x, argc, argv);
      }

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      pipe_options |= O_CREAT;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;
    
    case CLOEXEC:
      if(verbose_flag == 1){
      int x = optind;
      x = x-1;
      verbose(x, argc, argv);
      }
      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      pipe_options |= O_CLOEXEC;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;
      
    case APPEND:
      if(verbose_flag == 1){
      int x = optind;
      x = x-1;
      verbose(x, argc, argv);
      }
      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      pipe_options |= O_APPEND;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      break;
      
    case READONLY:
      if(verbose_flag == 1){
      int x = optind;
      x = x-2;
      verbose(x, argc, argv);
      }

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      fd = open(optarg, O_RDONLY | pipe_options, 0644);

      if (fd < 0){
	   fprintf(stderr, "%s:No file\n", strerror(errno));
	   flag = 1;
	 }

      fds[fds_index] = fd;
      fds_index++;


      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }	 

       pipe_options = 0;
       break;
       
    case WRITEONLY:
      if(verbose_flag == 1){
      int x = optind;
      x = x-2;
      verbose(x, argc, argv);
      }

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
	  }

       startUTime = r_usage.ru_utime;
       startSTime = r_usage.ru_stime;

      fd = open(optarg, O_WRONLY | pipe_options, 0644);
	if(fd < 0){
	  fprintf(stderr, "%s:%s\n", optarg, strerror(errno));
	  flag = 1;
	  //fds_index++;
	}
 
	fds[fds_index] = fd;
	fds_index++;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

	
	pipe_options = 0;
      break;

    case RDWR:
      if(verbose_flag == 1){
      int x = optind;
      x = x-2;
      verbose(x, argc, argv);
      }

      if(profile_flag == 1){
	if(getrusage(RUSAGE_SELF, &r_usage) == -1)
	  fprintf(stderr, "Error with usage");
	//startUTime = r_usage.ru_utime;
	//startSTime = r_usage.ru_stime;
      }

      startUTime = r_usage.ru_utime;
      startSTime = r_usage.ru_stime;
      
      fd = open(optarg, O_RDWR | pipe_options, 0666);
       if(fd < 0){
         fprintf(stderr, "%s:%s\n", optarg, strerror(errno));
         flag = 1;
	
       }
        fds[fds_index] = fd;
        fds_index++;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
	endUTime = r_usage.ru_utime;
	endSTime = r_usage.ru_stime;
	time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      pipe_options = 0;
      break;

    case CHDIR:
      if(verbose_flag == 1){
	int x = optind;
	x = x-2;
	verbose(x, argc, argv);
	}

       if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        startUTime = r_usage.ru_utime;
        startSTime = r_usage.ru_stime;
        }

      if(chdir(optarg) < 0){
	fprintf(stderr, "%s:%s\n", optarg, strerror(errno));
	flag = 1;
      }

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
      }

      
      break;

    case WAIT:
      if(verbose_flag == 1){
        int x = optind;
        x = x-1;
        verbose(x, argc, argv);
	}
      
      //int sig2 = 0;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
      
	startUTime = r_usage.ru_utime;
	startSTime = r_usage.ru_stime;
	}

      //startUTime = r_usage.ru_utime;
      //startSTime = r_usage.ru_stime;

      int process = 0;
      while(process < counter){
	int status = 0;
	pid_t s = wait(&status);
	
	for(int k = 0; k < counter; k++){
	  
	  if(s == arr2[k].child_process){

	    if(WIFEXITED(status)){
	      
	      int e = WEXITSTATUS(status);
	      if (e > flag)
		flag = e;
	      fprintf(stdout, "exit ");
	      fprintf(stdout, "%d ", e);

	      }
	    //fflush(stdout);

	    if(WIFSIGNALED(status)){
	       int s2 = WTERMSIG(status);
	       if(s2 > sig2)
		 sig2 = s2;
	       fprintf(stdout, "signal ");
	       fprintf(stdout, "%d ", s2);
	       			       
	       }
	    //fflush(stdout);
	    for(int m = arr2[k].startIndex; m < arr2[k].endIndex; m++){
                fprintf(stdout, "%s", argv[m]);
                fprintf(stdout, " ");
		}
	    fprintf(stdout, "\n");

	    //fflush(stdout);
	  }
	  fflush(stdout);
	}
	process++;
      }

       if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);
       }

        if(profile_flag == 1){
        if(getrusage(RUSAGE_CHILDREN, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

        endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
        time_proc(startUTime, startSTime, endUTime, endSTime);


      }


     
      break;
      
    case COMMAND:
      if(verbose_flag == 1){
	int x = optind;
	x = x-2;
	verbose(x, argc, argv);
       }
      
      int start = 0;
      int end = 0;
      

      char* arr[BUFFERSIZE];
      int count = 0;

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");
	//startUTime = r_usage.ru_utime;
	//startSTime = r_usage.ru_stime;
      }

      startUTime = r_usage.ru_utime;
      startSTime = r_usage.ru_stime;


      optind--;
      inidx = argv[optind];

      optind = optind + 1;
      outidx = argv[optind];
            
      optind = optind + 1;
      erridx = argv[optind];
      
      optind = optind + 1;

      start = optind;
      
      for(int i = optind; i < argc; i++){
	char* word = argv[i];
	if(word[0] == '-' && word[1] == '-')
	  break;
	else{
	  arr[count] = argv[i];
	  count++;
	  optind++;
	}
      }

      end = optind;

      arr[count] = NULL;

      if(count == 0){
	flag = 1;
	fprintf(stderr, "%s:\n",  strerror(errno));
      }
      
      int ix = *inidx - '0';
      int ox = *outidx - '0';
      int ex = *erridx - '0';
      pid_t child_pid = -1;     

      if((ix >= fds_index) || (ox >= fds_index) || (ex >= fds_index)){
	fprintf(stderr, "%s:\n",  strerror(errno));
	exit(1);
        flag = 1;
      }
      else{
	child_pid = fork();
      }

            
      if(child_pid == 0){

	int closeflag = 0;
	      
       	if(dup2(fds[ix], STDIN_FILENO) < 0){
	  fprintf(stderr, "%s:Cannot open, Error\n",  strerror(errno));
	  //exit(1);
	}
       
		
	if(dup2(fds[ox], STDOUT_FILENO)  < 0) {
	  fprintf(stderr, "%s:\n",  strerror(errno));
          //exit(1);
	}
	
	
        if(dup2(fds[ex], STDERR_FILENO) < 0){
	  fprintf(stderr, "%s:\n",  strerror(errno));
          //exit(1);
	}
	
	for(int i = 0; i < fds_index; i++){
	  close(fds[i]);
        }

	if(closeflag == 0){
	  if(execvp(arr[0], arr) == -1){
	    fprintf(stderr, "%s:\n",  strerror(errno));
	    //exit(1);
	  }
	}

		
	
      }
      
      else if (child_pid > 0){
        	
	if(ix == index || ex == index || ox == index){
	   fprintf(stderr, "%s:\n",  strerror(errno));
           exit(1);
	}
	  
       
       
	arr2[counter].child_process = child_pid;
	arr2[counter].startIndex = start;
	arr2[counter].endIndex = end;
	counter++;

      }

      else{
	;
      }

      if(profile_flag == 1){
        if(getrusage(RUSAGE_SELF, &r_usage) == -1)
          fprintf(stderr, "Error with usage");

	endUTime = r_usage.ru_utime;
        endSTime = r_usage.ru_stime;
	time_proc(startUTime, startSTime, endUTime, endSTime);
      }
      
      break;

    default:
      fprintf(stderr, "%s:\n", "Unrecognized argument");
      flag = 1;
	
      break;
      

    }
    
  }
  if(sig2 > 0){
    signal(sig2, SIG_DFL);
    raise(sig2);
  }
  
  else
    exit(flag);     

  free(arr2);
  
  }
