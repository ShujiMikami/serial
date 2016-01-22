#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

struct termios oldtio;//current termios setteing buffer
int fd;//fileDescriptor

void initSerial();//serial port initialize
void readAllMessage();//read all received messages

int main(int argc,char* argv[])
{/*{{{*/
  //command analysis

  fd=open("/dev/ttyS0",O_RDWR);
  
  initSerial(fd);

  write(fd,argv[1],strlen(argv[1]));
  write(fd,"\r\n",2);
  readAllMessage();


//  sleep(1);

  tcflush(fd,TCIFLUSH);
  tcsetattr(fd,TCSANOW,&oldtio);

  close(fd);
  return 0;
}/*}}}*/
void initSerial()
{/*{{{*/
  struct termios newtio;

  //save current setting to oldtio
  tcgetattr(fd,&oldtio);

  //set serial
  newtio=oldtio;
  
  //set speed
  cfsetspeed(&newtio,B9600);

  //set canonical mode off
  newtio.c_lflag&= ~ICANON;
  //set raw mode
  newtio.c_oflag=0;

  //set timer
  newtio.c_cc[VTIME]=1;

  //set minchar
  newtio.c_cc[VMIN]=0;
  //set newtio
  tcflush(fd,TCIFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);
}/*}}}*/
void readAllMessage()
{/*{{{*/
  unsigned char readByte[1024];
  ssize_t readSize=0;
  int i=0;
  do
  {
    readSize=read(fd,readByte,1024);
    for(i=0;i<readSize;i++)
    {
       printf("%c",readByte[i]);
    }
  }while(readSize);
  
}/*}}}*/
/* vim:set foldmethod=marker: */
