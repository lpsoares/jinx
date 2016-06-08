//
// File: Serial.h
// Created by: Victor HP Gomes <vhpgomes@lsi.sp.br>
// Created on: Fri Sep  3 13:57:26 2004
//

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <string>
#include <termios.h>

#include <iostream>

#include <fcntl.h>
#include <sys/ioctl.h>

#include <unistd.h>

#define MAX_READ_ERRORS	5

typedef unsigned char byte;

class Serial
{
protected:
	int fd;

	struct termios oldtermios;
		
	
	FILE* file;

public:


	Serial(void);
	~Serial(void);

	bool sopen(std::string device, int baudrate = 9600);
	void close(void);

	bool setRTS(bool set);
	bool setDTR(bool set);

	int read(byte* buffer, uint len);

	inline bool swrite(const byte* buffer, uint len)
	{	return (write(fd,buffer,len) == (int)len); }

	inline void flush(void) { tcflush(fd,TCIOFLUSH); }

	inline int getFD(void) const { return fd; }

};


#endif	//_SERIAL_H_
