//
// File: Serial.cpp
// Created by: Victor HP Gomes <vhpgomes@lsi.sp.br>
// Created on: Fri Sep  3 13:57:27 2004
//

#include "Serial.h"

#include <iostream>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <asm/errno.h>

using namespace std;


Serial::Serial(void)
{
	fd = 0;
}

Serial::~Serial(void)
{
	if(fd > 0)	close();
}

bool Serial::sopen(string device, int baudrate)
{
	
	cout << "opening" << device.c_str() << endl;
	fd = open(device.c_str(), O_RDWR | O_NONBLOCK | O_NOCTTY);
	
	//fd = ::open(device.c_str(),O_RDWR | O_NOCTTY);
	if(fd < 0) {
		
		cout << "Error" << endl;
		
		return false;

	}
	
	cout << "Step 1" << endl;
	
	// saves the current configuration
	if(tcgetattr(fd,&oldtermios) < 0)
		return false;

	struct termios newtermios;
	memset(&newtermios,0,sizeof(struct termios));	

	// now prepare the new termios
		//control mode flags
		//BXXX    - baud rate communication
		//CS8     - 8 bit, no parity, 1 stopbit
		//CLOCAL  - local connection, no modem contol
		//CREAD   - enable receiving characters
	newtermios.c_cflag = CS8 | CLOCAL | CREAD;
		
	switch(baudrate)
	{
		case 0		: newtermios.c_cflag |= B0; break;
		case 50		: newtermios.c_cflag |= B50; break;
		case 75		: newtermios.c_cflag |= B75; break;
		case 110	: newtermios.c_cflag |= B110; break;
		case 134	: newtermios.c_cflag |= B134; break;
		case 150	: newtermios.c_cflag |= B150; break;
		case 200	: newtermios.c_cflag |= B200; break;
		case 300	: newtermios.c_cflag |= B300; break;
		case 600	: newtermios.c_cflag |= B600; break;
		case 1200	: newtermios.c_cflag |= B1200; break;
		case 1800	: newtermios.c_cflag |= B1800; break;
		case 2400	: newtermios.c_cflag |= B2400; break;
		case 4800	: newtermios.c_cflag |= B4800; break;
		case 9600	: newtermios.c_cflag |= B9600; break;
		case 19200	: newtermios.c_cflag |= B19200; break;
		case 38400	: newtermios.c_cflag |= B38400; break;
		case 57600	: newtermios.c_cflag |= B57600; break;
		case 115200	: newtermios.c_cflag |= B115200; break;
		case 230400	: newtermios.c_cflag |= B230400; break;
		default:
			cout << "Invalid baudrate: " << baudrate << endl;
			return false;
	}
	
	cout << "Step 2" << endl;
	
	//input mode flags
	newtermios.c_iflag = 0;

	//local mode flags
	newtermios.c_lflag = 0; //non canonical input

	//control characters
	//set a 1 second timeout
	newtermios.c_cc[VMIN]  = 0;
	newtermios.c_cc[VTIME] = 10;

	cout << "Step 3" << endl;
	
	// flushes the serial
	flush();

	// Configure serial.
	if(tcsetattr(fd,TCSANOW,&newtermios) < 0)
		return false;

	// flushes the serial
	flush();

	cout << "Step 4" << endl;
	
	return true;
}

void Serial::close(void)
{
	flush();
	tcsetattr(fd,TCSANOW,&oldtermios);
	::close(fd);
}

bool Serial::setRTS(bool high)
{
	int status;
	if(ioctl(fd,TIOCMGET,&status) < 0)
		return false;

	if(high && (status & TIOCM_RTS))
		return true;	//already high
	else if(!high && !(status & TIOCM_RTS))
		return true;	//already low

	//fall through, must toggle RTS
	status ^= TIOCM_RTS;
	if(ioctl(fd,TIOCMSET,&status) < 0)
		return false;

	//success
	return true;
}

bool Serial::setDTR(bool high)
{
	int status;
	if(ioctl(fd,TIOCMGET,&status) < 0)
		return false;

	if(high && (status & TIOCM_DTR))
		return true;	//already high
	else if(!high && !(status & TIOCM_DTR))
		return true;	//already low

	//fall through, must toggle DTR
	status ^= TIOCM_DTR;
	if(ioctl(fd,TIOCMSET,&status) < 0)
		return false;

	//success
	return true;
}

int Serial::read(byte* buffer, uint len)
{
	int tot,count,read_error;

	tot = count = read_error = 0;

	while((tot < (int)len) && (read_error < MAX_READ_ERRORS))
	{
		count = ::read(fd,&buffer[tot],len - tot);
		if(count <= 0)	// in our case, timeout is error
		{
			if((count < 0) && read_error == EINTR)
				continue;	// error due interrupt... don't count
			
			if(count == 0)
				cout << "Serial read timeout" << endl;
			
			else
				cout << "Generic error: " << strerror(read_error) << endl;

			read_error++;
		}
		else
		{
			//bytes read
			tot += count;
		}
	}

	return tot;


}
