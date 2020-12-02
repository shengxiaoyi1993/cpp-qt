#include "math.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

int set_interface_attribs (int fd, int speed, int parity);
void set_blocking (int fd, int should_block);
unsigned char hex_to_char(char *src);


int open_port(int fd,char *dev) {

	long vdisable;
	char filename[64];
	sprintf(filename,"/dev/%s",dev);
	printf("#%s######",filename);
	fd = open(filename, O_RDWR|O_NOCTTY|O_NDELAY);
		if (-1 == fd){
			perror("Can't Open Serial Port");
			return(-1);
			}
		else
			printf("open %s .....\n",dev);
		if(fcntl(fd, F_SETFL, 0)<0)
		printf("fcntl failed!\n");
		else
		printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
		if(isatty(STDIN_FILENO)==0)
		printf("standard input is not a terminal device\n");
		else
		printf("isatty success!\n");
		printf("fd-open=%d\n",fd);
		return fd;
		}

int set_opt(int fd,int nSpeed) {
		struct termios newtio,oldtio;
		if ( tcgetattr( fd,&oldtio) != 0) {
				perror("SetupSerial 1");
					return -1;   }
			bzero( &newtio, sizeof( newtio ) );
			newtio.c_cflag |= CLOCAL | CREAD;
			newtio.c_cflag &= ~CSIZE;
			newtio.c_cflag |= CS8;
			newtio.c_cflag &= ~PARENB;
				switch( nSpeed )   {
				case 2400:
				cfsetispeed(&newtio, B2400);
				cfsetospeed(&newtio, B2400);
				 break;
				case 4800:
				cfsetispeed(&newtio, B4800);
				cfsetospeed(&newtio, B4800);
				break;
				case 9600:
				cfsetispeed(&newtio, B9600);
				cfsetospeed(&newtio, B9600);
				break;
				case 115200:
				cfsetispeed(&newtio, B115200);
				cfsetospeed(&newtio, B115200);
				break;
				case 460800:
				cfsetispeed(&newtio, B460800);
				cfsetospeed(&newtio, B460800);
				break;
				case 921600:
				cfsetispeed(&newtio, B921600);
				cfsetospeed(&newtio, B921600);
				break;
				default:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
			}

		newtio.c_cflag &= ~CSTOPB;
		newtio.c_cc[VTIME] = 0;
		newtio.c_cc[VMIN] = 0;
		tcflush(fd,TCIFLUSH);
			if((tcsetattr(fd,TCSANOW,&newtio))!=0)   {
			perror("com set error");
			return -1;   }
			printf("set done!\n");
			return 0;
			}

int main(int argc, char *argv[]) {

   int   i, j, k;
   char  pram[4][128];
   unsigned char  cmd_string[64], cmd_crc;
   unsigned char tx_buf[128];
   unsigned char rx_buf[16];
   int   burst;
   int fd;
   int baudrate;
   char *portname = "ttyUSB1";
   baudrate=115200;
   struct timeval start, end;

	 if((fd=open_port(fd,portname))<0){
		printf("open_port error\n");
		return;
	}
	if((i=set_opt(fd,baudrate))<0){
		perror("set_opt error");
		return;
	}
   // set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
 //  set_interface_attribs (fd, B921600, 0);  // set speed to 460,800 bps, 8n1 (no parity)
  // set_blocking (fd, 0);                // set no blocking

while (1) {
	printf("cycle !\n");

	      cmd_string[0] = 0xfa;
	      cmd_string[1] = 0x19;
	      cmd_string[2] = 0x79;
	      cmd_string[3] = 0x02;
				cmd_string[4] = 0x01;
	      cmd_string[5] = 0x10;
	      cmd_string[6] = 0x0c;
				cmd_crc = 0;
				for (i=4; i<7; i++) {
					 cmd_crc += cmd_string[i];
				}
				write(fd, cmd_string, 7);

				sleep(1);
				if (read(fd, rx_buf, sizeof rx_buf)!=0) {
					if (rx_buf[0]==cmd_crc) {
						 printf("Command Success!\n");
						 printf("rx_buf:%x %x %x %x %x\n",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4]);

					} else {
						 printf("rx_buf = %d, cmd_crc = %d\n", rx_buf[0], cmd_crc);
						 printf("Faile cmd return crc check, command not received correctly!!!!!!\n");
					}
				}

}


   // while (1) {
	 //
   //    printf("Waiting for command!\n");
	 //
   //    // get command input
   //    int pram_number = 0;
   //    while (fscanf(stdin, "%s", pram[pram_number])!=EOF && getchar()!='\n')
   //       pram_number++;
	 //
   //    if (strcmp(pram[0], "exit")==0) {
   //       exit(1);
   //    } else if (strcmp(pram[0], "write_cmos")==0) {
   //       // --{{{
   //       // lower bytes first
   //       cmd_string[0] = 0xfa;
   //       cmd_string[1] = 0x19;
   //       cmd_string[2] = 0x79;
   //       cmd_string[3] = 0x04;
   //       cmd_string[4] = 0x02;
   //       cmd_string[5] = hex_to_char(pram[1]+4);
   //       cmd_string[6] = hex_to_char(pram[1]+2);
   //       cmd_string[7] = hex_to_char(pram[1]+0);
   //       cmd_string[8] = hex_to_char(pram[2]+0);
	 //
   //       cmd_crc = 0;
   //       for (i=4; i<9; i++) {
   //          cmd_crc += cmd_string[i];
   //       }
   //       // printf("cmd_crc = %d\n", cmd_crc);
   //       write(fd, cmd_string, 9);
	 //
   //       while (read(fd, rx_buf, sizeof rx_buf)==0) {
   //       }
	 //
   //       if (rx_buf[0]==cmd_crc) {
   //          printf("Command Success!\n");
   //       } else {
   //          printf("rx_buf = %d, cmd_crc = %d\n", rx_buf[0], cmd_crc);
   //          printf("Faile cmd return crc check, command not received correctly!!!!!!\n");
   //       }
   //       // --}}}
	 //
   //    } else if (strcmp(pram[0], "write_regbank")==0) {
   //       // --{{{
   //       // lower bytes first
   //       cmd_string[0] = 0xfa;
   //       cmd_string[1] = 0x19;
   //       cmd_string[2] = 0x79;
   //       cmd_string[3] = 0x07;
   //       cmd_string[4] = 0x00;
   //       cmd_string[5] = hex_to_char(pram[1]+4);
   //       cmd_string[6] = hex_to_char(pram[1]+2);
   //       cmd_string[7] = hex_to_char(pram[1]+0);
   //       cmd_string[8] = hex_to_char(pram[2]+6);
   //       cmd_string[9] = hex_to_char(pram[2]+4);
   //       cmd_string[10] = hex_to_char(pram[2]+2);
   //       cmd_string[11] = hex_to_char(pram[2]+0);
	 //
   //       cmd_crc = 0;
   //       for (i=4; i<12; i++) {
   //          cmd_crc += cmd_string[i];
   //       }
   //       // printf("cmd_crc = %d\n", cmd_crc);
   //       write(fd, cmd_string, 12);
	 //
   //       while (read(fd, rx_buf, sizeof rx_buf)==0) {
   //       }
	 //
   //       if (rx_buf[0]==cmd_crc) {
   //          printf("Command Success!\n");
   //       } else {
   //          printf("rx_buf = %d, cmd_crc = %d\n", rx_buf[0], cmd_crc);
   //          printf("Faile cmd return crc check, command not received correctly!!!!!!\n");
   //       }
   //       // --}}}
	 //
		// 	} else if (strcmp(pram[0], "read_regbank")==0) {
   //       //--{{{
   //       // lower bytes first
   //       cmd_string[0] = 0xfa;
   //       cmd_string[1] = 0x19;
   //       cmd_string[2] = 0x79;
   //       cmd_string[3] = 0x03;
   //       cmd_string[4] = 0x01;
   //       cmd_string[5] = hex_to_char(pram[1]+4);
   //       cmd_string[6] = hex_to_char(pram[1]+2);
   //       cmd_string[7] = hex_to_char(pram[1]+0);
	 //
   //       cmd_crc = 0;
   //       for (i=4; i<8; i++) {
   //          cmd_crc += cmd_string[i];
   //       }
   //       // printf("cmd_crc = %d\n", cmd_crc);
   //       write(fd, cmd_string, 8);
	 //
   //       while (read(fd, rx_buf, 1)==0) {
   //       }
	 //
   //       if (rx_buf[0]==cmd_crc) {
   //          printf("Command Success!\n");
   //       } else {
   //          printf("rx_buf = %d, cmd_crc = %d\n", rx_buf[0], cmd_crc);
   //          printf("Faile cmd return crc check, command not received correctly!!!!!!\n");
   //       }
	 //
   //       // receive register
   //       printf("Return data ");
   //       for (i=0; i<4; i++) {
   //          while (read(fd, rx_buf, 1)==0) {
   //          }
   //          printf("%2x ", rx_buf[0]);
   //       }
   //       // while (read(fd, rx_buf, 4)==0) {
   //       // }
   //       // for (i=0; i<4; i++) {
   //       //    printf("%2x ", rx_buf[3-i]);
   //       // }
	 //
   //       printf("\nDone data receiving!!\n");
   //       // --}}}
	 //
   //    } else if (strcmp(pram[0], "read_cmos")==0) {
   //       //--{{{
   //       // lower bytes first
   //       cmd_string[0] = 0xfa;
   //       cmd_string[1] = 0x19;
   //       cmd_string[2] = 0x79;
   //       cmd_string[3] = 0x03;
   //       cmd_string[4] = 0x03;
   //       cmd_string[5] = hex_to_char(pram[1]+4);
   //       cmd_string[6] = hex_to_char(pram[1]+2);
   //       cmd_string[7] = hex_to_char(pram[1]+0);
	 //
   //       cmd_crc = 0;
   //       for (i=4; i<8; i++) {
   //          cmd_crc += cmd_string[i];
   //       }
   //       // printf("cmd_crc = %d\n", cmd_crc);
   //       write(fd, cmd_string, 8);
	 //
   //       while (read(fd, rx_buf, 1)==0) {
   //       }
	 //
   //       if (rx_buf[0]==cmd_crc) {
   //          printf("Command Success!\n");
   //       } else {
   //          printf("rx_buf = %d, cmd_crc = %d\n", rx_buf[0], cmd_crc);
   //          printf("Faile cmd return crc check, command not received correctly!!!!!!\n");
   //       }
	 //
   //       // receive register
   //       printf("Return data");
   //       for (i=0; i<1; i++) {
   //          while (read(fd, rx_buf, 1)==0) {
   //          }
   //          printf("%2x ", rx_buf[0]);
   //       }
   //       printf("\nDone data receiving!!\n");
   //       // --}}}
	 //
   //    } else if (strcmp(pram[0], "read_mem")==0) {
   //       //--{{{
   //       // lower bytes first
   //       cmd_string[0] = 0xfa;
   //       cmd_string[1] = 0x19;
   //       cmd_string[2] = 0x79;
   //       cmd_string[3] = 0x03;
   //       cmd_string[4] = 0x81;
   //       cmd_string[5] = hex_to_char(pram[1]+4);
   //       cmd_string[6] = hex_to_char(pram[1]+2);
   //       cmd_string[7] = hex_to_char(pram[1]+0);
	 //
   //       cmd_crc = 0;
   //       for (i=4; i<8; i++) {
   //          cmd_crc += cmd_string[i];
   //       }
   //       // printf("cmd_crc = %d\n", cmd_crc);
   //       write(fd, cmd_string, 8);
	 //
   //       while (read(fd, rx_buf, 1)==0) {
   //       }
	 //
   //       if (rx_buf[0]==cmd_crc) {
   //          printf("Command Success!\n");
   //       } else {
   //          printf("rx_buf = %d, cmd_crc = %d\n", rx_buf[0], cmd_crc);
   //          printf("Faile cmd return crc check, command not received correctly!!!!!!\n");
   //       }
	 //
   //       // receive register
   //       printf("Return data");
   //       for (i=0; i<32; i++) {
   //          while (read(fd, rx_buf, 1)==0) {
   //          }
   //          printf("%2x ", rx_buf[0]);
   //       }
   //       printf("\nDone data receiving!!\n");
   //       // --}}}
	 //
   //    } else if (strcmp(pram[0], "read_frame")==0) {
   //       //--{{{
   //       // lower bytes first
   //       int   frame_addr, frame_read_cnt;
   //       unsigned char rx_buf_temp[32];
   //       FILE  *frame_cap = fopen("frame_cap.mem", "wb+");
	 //
   //       frame_addr = hex_to_char(pram[1]+0)*65536 + hex_to_char(pram[1]+2)*256 + hex_to_char(pram[1]+4);
   //       frame_read_cnt = hex_to_char(pram[2]+0)*65536 + hex_to_char(pram[2]+2)*256 + hex_to_char(pram[2]+4);
		// 	gettimeofday( &start, NULL );
   //       while (frame_read_cnt > 0) {
   //          cmd_string[0] = 0xfa;
   //          cmd_string[1] = 0x19;
   //          cmd_string[2] = 0x79;
   //          cmd_string[3] = 0x03;
   //          cmd_string[4] = 0x81;
   //          cmd_string[5] = (frame_addr%256);
   //          cmd_string[6] = (frame_addr%65536)/256;
   //          cmd_string[7] = frame_addr/65536;
	 //
   //          cmd_crc = 0;
   //          for (i=4; i<8; i++) {
   //             cmd_crc += cmd_string[i];
   //          }
   //          // printf("cmd_crc = %d\n", cmd_crc);
   //          write(fd, cmd_string, 8);
	 //
   //          while (read(fd, rx_buf, 1)==0) {
   //          }
	 //
   //          if (rx_buf[0]==cmd_crc) {
   //          //   printf("Command Success!\n");
   //          } else {
   //             printf("rx_buf = %d, cmd_crc = %d\n", rx_buf[0], cmd_crc);
   //             printf("d0 = %d, d1 = %d, d2 = %d, d3 = %d\n", cmd_string[4], cmd_string[5], cmd_string[6], cmd_string[7]);
   //             printf("Faile cmd return crc check, command not received correctly!!!!!!\n");
   //          }
	 //
   //          // receive register
   //       //   printf("Return data");
   //          for (i=0; i<32; i++) {
   //             while (read(fd, rx_buf, 1)==0) {
   //             }
   //             // printf("%2x ", rx_buf[0]);
   //             rx_buf_temp[31-i] = rx_buf[0];
   //          }
   //          fwrite(rx_buf_temp, sizeof(unsigned char), 32, frame_cap);
	 //
   //          frame_read_cnt--;
   //          frame_addr++;
   //       }
		//  gettimeofday( &end, NULL );
		//   float timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
		// timeuse=timeuse/1000000.0;
		// printf("time: %f s\n", timeuse);
		// printf("the frame_cap is ok!\n\n");
   //       fclose(frame_cap);
	 //
	 //
   //       printf("\nDone data receiving!!\n");
   //       // --}}}
	 //
   //    } else if (strcmp(pram[0], "read_frame_set")==0) {
   //       //--{{{
   //       // lower bytes first
   //       for (k=0; k<12; k++) {
   //          int   frame_addr, frame_read_cnt;
   //          unsigned char rx_buf_temp[32];
   //          char  file_name[128];
   //          sprintf(file_name, "frame_cap_%d.mem", k);
   //          FILE  *frame_cap = fopen(file_name, "wb+");
	 //
   //          switch (k) {
   //             case 0 : frame_addr = 1048576; break;
   //             case 1 : frame_addr = 1572864; break;
   //             case 2 : frame_addr = 3145728; break;
   //             case 3 : frame_addr = 3670016; break;
   //             case 4 : frame_addr = 5242880; break;
   //             case 5 : frame_addr = 5767168; break;
   //             case 6 : frame_addr = 7340032; break;
   //             case 7 : frame_addr = 7864320; break;
   //             case 8 : frame_addr = 9437184; break;
   //             case 9 : frame_addr = 9961472; break;
   //             case 10 : frame_addr = 11534336; break;
   //             default : frame_addr = 12058624; break;
   //          }
   //          frame_read_cnt = 337600;
	 //
   //          while (frame_read_cnt > 0) {
   //             cmd_string[0] = 0xfa;
   //             cmd_string[1] = 0x19;
   //             cmd_string[2] = 0x79;
   //             cmd_string[3] = 0x03;
   //             cmd_string[4] = 0x81;
   //             cmd_string[5] = (frame_addr%256);
   //             cmd_string[6] = (frame_addr%65536)/256;
   //             cmd_string[7] = frame_addr/65536;
	 //
   //             cmd_crc = 0;
   //             for (i=4; i<8; i++) {
   //                cmd_crc += cmd_string[i];
   //             }
   //             // printf("cmd_crc = %d\n", cmd_crc);
   //             write(fd, cmd_string, 8);
	 //
   //             while (read(fd, rx_buf, 1)==0) {
   //             }
	 //
   //             if (rx_buf[0]==cmd_crc) {
   //                printf("Command Success!\n");
   //             } else {
   //                printf("rx_buf = %d, cmd_crc = %d\n", rx_buf[0], cmd_crc);
   //                printf("d0 = %d, d1 = %d, d2 = %d, d3 = %d\n", cmd_string[4], cmd_string[5], cmd_string[6], cmd_string[7]);
   //                printf("Faile cmd return crc check, command not received correctly!!!!!!\n");
   //             }
	 //
   //             // receive register
   //             printf("Return data");
   //             for (i=0; i<32; i++) {
   //                while (read(fd, rx_buf, 1)==0) {
   //                }
   //                // printf("%2x ", rx_buf[0]);
   //                rx_buf_temp[31-i] = rx_buf[0];
   //             }
   //             fwrite(rx_buf_temp, sizeof(unsigned char), 32, frame_cap);
	 //
   //             frame_read_cnt--;
   //             frame_addr++;
   //          }
	 //
   //          fclose(frame_cap);
   //       }
	 //
   //       printf("\nDone data receiving!!\n");
   //       // --}}}
	 //
   //    // } else if (strcmp(pram[0], "read")==0) {
   //    //    // printf("read channel_number addr burst");
   //    //    //--{{{
   //    //    cmd_string[0] = 0x17;
   //    //    cmd_string[1] = 0x5c;
   //    //    cmd_string[2] = 0x03;
   //    //    cmd_string[3] = 0xf4;
   //    //    cmd_string[4] = 0x01;
   //    //    cmd_string[5] = hex_to_char(pram[1]+0);
   //    //    cmd_string[6] = hex_to_char(pram[2]+0);
   //    //    cmd_string[7] = hex_to_char(pram[2]+2);
   //    //    cmd_string[8] = hex_to_char(pram[2]+4);
   //    //    cmd_string[9] = hex_to_char(pram[2]+6);
   //    //    cmd_string[10] = hex_to_char(pram[3]+0);
   //    //    cmd_string[11] = hex_to_char(pram[3]+2);
   //    //    cmd_string[12] = hex_to_char(pram[3]+4);
   //    //    cmd_string[13] = hex_to_char(pram[3]+6);
   //    //
   //    //    cmd_crc = 0;
   //    //    for (i=4; i<14; i++) {
   //    //       cmd_crc += cmd_string[i];
   //    //    }
   //    //    // printf("cmd_crc = %d\n", cmd_crc);
   //    //    write(fd, cmd_string, 14);
   //    //
   //    //    while (read(fd, rx_buf, 1)==0) {
   //    //    }
   //    //
   //    //    if (rx_buf[0]==cmd_crc) {
   //    //       printf("Command Success!\n");
   //    //    } else {
   //    //       printf("rx_buf = %d, cmd_crc = %d\n", rx_buf[0], cmd_crc);
   //    //       printf("Faile cmd return crc check, command not received correctly!!!!!!\n");
   //    //    }
	 //
   //    //    burst = (int)cmd_string[10]*16777216+(int)cmd_string[11]*65536+(int)cmd_string[12]*256+(int)cmd_string[13];
   //    //    printf("burst = %d\n", burst);
   //    //
   //    //    // receive register
   //    //    printf("Return data");
   //    //    for (i=0; i<burst; i++) {
   //    //       while (read(fd, rx_buf, 1)==0) {
   //    //       }
   //    //       printf("%2x ", rx_buf[0]);
   //    //    }
   //    //    printf("\nDone data receiving!!\n");
   //    //    // --}}}
   //    //
   //    }
	 //
	 //
	 //
   // }

   close (fd);

}







int set_interface_attribs (int fd, int speed, int parity) {
// --{{{
// The values for speed are B115200, B230400, B9600, B19200, B38400, B57600, B1200, B2400, B4800, etc.
// The values for parity are 0 (meaning no parity), PARENB|PARODD (enable parity and use odd),
// PARENB (enable parity and use even), PARENB|PARODD|CMSPAR (mark parity), and PARENB|CMSPAR (space parity).
//
// "Blocking" sets whether a read() on the port waits for the specified number of characters to arrive.
// Setting no blocking means that a read() returns however many characters are available without waiting for more,
// up to the buffer limit.
   struct termios tty;

   memset (&tty, 0, sizeof tty);
   if (tcgetattr (fd, &tty) != 0) {
      // error_message ("error %d from tcgetattr", errno);
      printf ("error %d from tcgetattr\n", errno);
      return -1;
   }

   cfsetospeed (&tty, speed);
   cfsetispeed (&tty, speed);

   tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
   // disable IGNBRK for mismatched speed tests; otherwise receive break
   // as \000 chars
   tty.c_iflag &= ~IGNBRK;         // disable break processing
   tty.c_lflag = 0;                // no signaling chars, no echo,
                                   // no canonical processing
   tty.c_oflag = 0;                // no remapping, no delays
   tty.c_cc[VMIN]  = 0;            // read doesn't block
   tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

   tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

   tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                   // enable reading
   tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
   tty.c_cflag |= parity;
   tty.c_cflag &= ~CSTOPB;
   tty.c_cflag &= ~CRTSCTS;

   // set uart to raw mode
   tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
   tty.c_oflag &= ~OPOST;
   tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
   tty.c_cflag &= ~(CSIZE | PARENB);
   tty.c_cflag |= CS8;

   if (tcsetattr (fd, TCSANOW, &tty) != 0) {
      // error_message ("error %d from tcsetattr", errno);
      printf ("error %d from tcsetattr\n", errno);
      return -1;
   }
   return 0;
}
// --}}}

void set_blocking (int fd, int should_block) {
// --{{{
   struct termios tty;

   memset (&tty, 0, sizeof tty);
   if (tcgetattr (fd, &tty) != 0) {
      // error_message ("error %d from tggetattr", errno);
      printf ("error %d from tggetattr\n", errno);
      return;
   }

   tty.c_cc[VMIN]  = should_block ? 1 : 0;
   tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

   if (tcsetattr (fd, TCSANOW, &tty) != 0)
      // error_message ("error %d setting term attributes", errno);
      printf ("error %d setting term attributes\n", errno);
}
// --}}}

unsigned char hex_to_char(char *src) {
//    --{{{
   unsigned char value;

   if (src[1]<='9' && src[1]>='0') {
      value = (int)(src[1]-'0');
   } else if (src[1]<='f' && src[1]>='a') {
      value = (int)(src[1]-'a'+10);
   } else if (src[1]<='F' && src[1]>='A') {
      value = (int)(src[1]-'A'+10);
   } else {
      printf("Hex to Char convert error!!!\n");
   }

   if (src[0]<='9' && src[0]>='0') {
      value = value + 16*(int)(src[0]-'0');
   } else if (src[0]<='f' && src[0]>='a') {
      value = value + 16*(int)(src[0]-'a'+10);
   } else if (src[0]<='F' && src[0]>='A') {
      value = value + 16*(int)(src[0]-'A'+10);
   } else {
      printf("Hex to Char convert error!!!\n");
   }

   return value;
}
// --}}}
