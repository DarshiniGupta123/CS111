#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <poll.h>
#include <mraa.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PERIOD 'p'
#define SCALE 's'
#define LOG 'l'
#define ID 'i'
#define HOST 'h'

#define B 4275
#define R0 100000.0

int logFlag = 0;
FILE* file;
int stopFlag = 1;

int period = 1;
char temp = 'F';

mraa_aio_context temperature;

int socket1;

struct option args[] =
{
   {"period", 1, NULL, PERIOD},
   {"scale", 1, NULL, SCALE},
   {"log", 1, NULL, LOG},
   {"id", 1, NULL, ID},
   {"host", 1, NULL, HOST},
   {0, 0, 0, 0}
};

float convert_temp(int reading)
{
  float R = 1023.0/((float) reading) - 1.0;
  R = R0*R;
  //C is the temperature in Celcious
  float C = 1.0/(log(R/R0)/B + 1/298.15) - 273.15;
  //F is the temperature in Fahrenheit
  float F = (C * 9)/5 + 32;

  if(temp == 'F')
    return F;
  else
    return C;
}

void print_TimeTemp(struct timespec ts, float tempNum)
{
  struct tm * tm;
  clock_gettime(CLOCK_REALTIME, &ts);
  tm = localtime(&(ts.tv_sec));
  if(logFlag){
    fflush(file);
    fprintf(file, "%02d:%02d:%02d %.1f\n", tm->tm_hour, tm->tm_min, tm->tm_sec, tempNum);
    fflush(file);
  }
  dprintf(socket1, "%02d:%02d:%02d %.1f\n", tm->tm_hour, tm->tm_min, tm->tm_sec, tempNum);
}

void do_when_pushed(){
  struct timespec ts;
  struct tm * tm;
  clock_gettime(CLOCK_REALTIME, &ts);
  tm = localtime(&(ts.tv_sec));
  if(logFlag){
    fflush(file);
    fprintf(file, "%02d:%02d:%02d SHUTDOWN\n", tm->tm_hour,
            tm->tm_min, tm->tm_sec);
    fflush(file);
    }
  dprintf(socket1, "%02d:%02d:%02d SHUTDOWN\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
  mraa_aio_close(temperature);

  fclose(file);

  exit(0);
}

void commands(char command[1024]){
  if (strcmp(command, "SCALE=F") == 0)
    temp = 'F';
  else if (strcmp(command, "SCALE=C") == 0)
    temp = 'C';
  else if (strcmp(command, "STOP") == 0)
    stopFlag = 0;
  else if (strcmp(command, "START") == 0)
    stopFlag = 1;
  else if (strncmp(command, "PERIOD=", 7) == 0)
    period = atoi(&command[7]);
  else if (strcmp(command, "OFF") == 0)
    do_when_pushed();
  else if (strncmp(command, "LOG", 3) == 0) 
    temp = temp; 
  else{
    fprintf(stderr, "unrecognized command\n");
    exit(1);
  }

}

int client_connect(char* host_name, unsigned int port) 
//e.g.host_name:google.com, port:80, return the socket for subsequent communication
{
  struct sockaddr_in serv_addr; //encode the ip address and the port for the remote
  
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
  if(sockfd < 0){
	fprintf(stderr, "Error in socket\n");
	exit(2);
  }
  struct hostent *server = gethostbyname(host_name);
    
  memset(&serv_addr, 0, sizeof(struct sockaddr_in));
  
  serv_addr.sin_family = AF_INET; //address is Ipv4
  
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
 
  serv_addr.sin_port = htons(port); //setup the port

  if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
  	fprintf(stderr, "Error in connecting to server\n");
	//printf("error in connecting");
	exit(2);
  }
  return sockfd; 
}


int main(int argc, char * argv[]){

  //while(1){
  char* host_name;
  char id[10];
  unsigned int port;
  int ret = 0;
  int hostflag = 0;
  int idflag = 0;
  int portflag = 0;
  
  while((ret = getopt_long(argc, argv, "", args, NULL)) != -1){
    switch(ret){
    case 'p':
      period = atoi(optarg);
      break;

    case 's':
      temp = optarg[0];
      break;

    case 'l':
      file = fopen(optarg, "w");
      logFlag = 1;
      if(file == NULL){
	fprintf(stderr, "Cannot open file\n");
	exit(1);
      }
      break;

    case 'h':
      host_name = malloc((strlen(optarg)+1)*sizeof(char));
      strcpy(host_name, optarg);
      hostflag = 1;
      break;

    case 'i':
      strcpy(id, optarg);
      idflag = 1;
      break;

    default:
      fprintf(stderr, "Unrecognized argument");
      exit(1);
      break;

    }

  }

  if(optind < argc){
    port = atoi(argv[optind]);
    portflag = 1;
  }

  if(!logFlag || !hostflag || !idflag || !portflag){
    fprintf(stderr, "Mandatory arguments are missing\n");
    exit(1);
  }

  socket1 = client_connect(host_name, port);
  if(socket1 < 0){
    fprintf(stderr, "Cannot connect\n");
    exit(2);
  }

  dprintf(socket1, "ID=%s\n", id);
  fflush(file);
  if(logFlag)
  	fprintf(file, "ID=%s\n", id);
  fflush(file);

  //mraa_aio_context temperature;
  temperature = mraa_aio_init(1);
  if(temperature == NULL){
    fprintf(stderr, "Error with Temperature Sensor\n");
    exit(1);
  }

  
  struct timespec time1, time2;
  clock_gettime(CLOCK_MONOTONIC, &time1);

  struct pollfd pollfds[1];
  pollfds[0].fd = socket1;
  pollfds[0].events = POLLIN;
 
  char command[1024];
  int counter = 0;
  while(1){    
    poll(pollfds, 1, 0);
    if (pollfds[0].revents & POLLIN) {
      char buf[1];
      if(read(socket1, buf, sizeof(buf)) < 0){
	fprintf(stderr, "Error in reading");
	exit(1);
      }

      if(buf[0] == '\n'){
	command[counter] = '\0';
        
	if(logFlag){
	  fflush(file);
	  fprintf(file, "%s\n", command);
	  fflush(file);
        }
	commands(command);
	command[0] = '\0';
	counter = 0;
      }

      else{
	command[counter] = buf[0];
	counter++;
	
      }
    }
  
    clock_gettime(CLOCK_MONOTONIC, &time2);
    if((time1.tv_sec + period <= time2.tv_sec) && stopFlag){
      int reading = mraa_aio_read(temperature);
      float tempNum = convert_temp(reading);

      print_TimeTemp(time2, tempNum);
      
      clock_gettime(CLOCK_MONOTONIC, &time1);
    }
  }

  //mraa_gpio_close(button);
  mraa_aio_close(temperature);
  
  fclose(file);

  exit(0);
}

