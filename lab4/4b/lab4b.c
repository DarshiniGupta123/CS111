//NAME:Darshini Gupta
//ID: 004928411
//EMAIL: darshinigupta0711@gmail.com

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

#define PERIOD 'p'
#define SCALE 's'
#define LOG 'l'

#define B 4275
#define R0 100000.0

int logFlag = 0;
FILE* file;
int stopFlag = 1;

int period = 1;
char temp = 'F';

struct option args[] =
{
   {"period", 1, NULL, PERIOD},
   {"scale", 1, NULL, SCALE},
   {"log", 1, NULL, LOG},
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
  if(logFlag)
    fprintf(file, "%02d:%02d:%02d %.1f\n", tm->tm_hour, tm->tm_min, tm->tm_sec, tempNum);
  fprintf(stdout, "%02d:%02d:%02d %.1f\n", tm->tm_hour, tm->tm_min, tm->tm_sec, tempNum);
}

void do_when_pushed(){
  struct timespec ts;
  struct tm * tm;
  clock_gettime(CLOCK_REALTIME, &ts);
  tm = localtime(&(ts.tv_sec));
  if(logFlag)
    fprintf(file, "%02d:%02d:%02d SHUTDOWN\n", tm->tm_hour,
            tm->tm_min, tm->tm_sec);
  fprintf(stdout, "%02d:%02d:%02d SHUTDOWN\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
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

int main(int argc, char * argv[]){
  mraa_gpio_context button;
  button = mraa_gpio_init(60);
  if(button == NULL){
    fprintf(stderr, "Error with button");
    exit(1);
  }

  mraa_aio_context temperature;
  temperature = mraa_aio_init(1);
  if(temperature == NULL){
    fprintf(stderr, "Error with Temperature Sensor");
    exit(1);
  }

  mraa_gpio_dir(button, MRAA_GPIO_IN);
  mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, do_when_pushed, NULL);

  //int period = 1; //default for period is 1
  //char temp = 'F'; //default temperature is Farenheit

  //while(1){
    int ret = 0;
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
	if(file == NULL)
	{
	      fprintf(stderr, "Error! Could not open file\n"); 
              exit(1); // must include stdlib.h
	} 
	logFlag = 1;
	break;

      default:
	fprintf(stderr, "Unrecognized argument");
	exit(1);
	break;
      
      }
	  
    }
 //}

  struct timespec time1, time2;
  clock_gettime(CLOCK_MONOTONIC, &time1);

  struct pollfd pollfds[1];
  pollfds[0].fd = 0;
  pollfds[0].events = POLLIN;

  char command[1024];
  int counter = 0;
  while(1){
    
    poll(pollfds, 1, 5);
    if (pollfds[0].revents & POLLIN) {
      char buf[1];
      if(read(0, buf, sizeof(buf)) < 0){
	fprintf(stderr, "Error in reading");
	exit(1);
      }

      if(buf[0] == '\n'){
	command[counter] = '\0';
	if(logFlag)
	  fprintf(file, "%s\n", command);
	commands(command);
	command[0] = '\0';
	counter = 0;
      }

      else{
	command[counter] = buf[0];
	counter++;
	
      }
    }
    //printf("hello");
    clock_gettime(CLOCK_MONOTONIC, &time2);
    if((time1.tv_sec + period <= time2.tv_sec) && stopFlag){
      int reading = mraa_aio_read(temperature);
      float tempNum = convert_temp(reading);

      print_TimeTemp(time2, tempNum);
      
      clock_gettime(CLOCK_MONOTONIC, &time1);
    }
  }

  mraa_gpio_close(button);
  mraa_aio_close(temperature);
  fclose(file);

  exit(0);
}
