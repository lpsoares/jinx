// Implementation of the Socket class.


#include "Socket.h"
//#include "string.h"

#include <string>

#include <errno.h>
#include <fcntl.h>
#include <iostream>

#ifndef LINUX
	typedef int socklen_t;
	#define MSG_NOSIGNAL 16384
#endif


using namespace std;

Socket::Socket() :
  m_sock ( -1 )
{

  memset ( &m_addr,
	   0,
	   sizeof ( m_addr ) );

}

Socket::~Socket()
{
  if ( is_valid() )
    ::close ( m_sock );
}

bool Socket::create()
{
  m_sock = socket ( AF_INET,
		    SOCK_STREAM,
		    //SOCK_SEQPACKET,
		    0 );

  if ( ! is_valid() )
    return false;


  // TIME_WAIT - argh
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
    return false;

  on = 307200*3;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_SNDBUF, ( const char* ) &on, sizeof ( on ) ) == -1 )
    return false;
  
  return true;
  
}



bool Socket::bind ( const int port )
{

  if ( ! is_valid() )
    {
      return false;
    }



  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( port );

  int bind_return = ::bind ( m_sock,
			     ( struct sockaddr * ) &m_addr,
			     sizeof ( m_addr ) );


  if ( bind_return == -1 )
    {
      return false;
    }

  return true;
}


bool Socket::listen() const
{
  if ( ! is_valid() )
    {
      return false;
    }

  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );


  if ( listen_return == -1 )
    {
      return false;
    }

  return true;
}


bool Socket::accept ( Socket& new_socket ) const
{
  int addr_length = sizeof ( m_addr );
  new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

  if ( new_socket.m_sock <= 0 )
    return false;
  else
    return true;
}


bool Socket::send ( const std::string s ) const
{
  int status = ::send ( m_sock, s.c_str(), s.size(), MSG_NOSIGNAL );
  if ( status == -1 )
    {
      return false;
    }
  else
    {
      return true;
    }
}


bool Socket::send ( int *s ) const
{
  int status = ::send ( m_sock, s, MAXRECV, MSG_WAITALL );
  //printf("Tamanho do char %d, valor mandado %d, sizeof %d \n",strlen((const char *)s), status, sizeof(s));
  if ( status == -1 )
    {
      return false;
    }
  else
    {
      return true;
    }
}


int Socket::send ( int *s, int size )
{
  int status = ::send ( m_sock, s, size, MSG_WAITALL );
  //printf("Tamanho do char %d, valor mandado %d, sizeof %d \n",strlen((const char *)s), status, sizeof(s));
  //cout << "send socket " << status << "  " << size << endl;
  return status;
}



int Socket::recv ( std::string& s ) const
{
  char buf [ MAXRECV + 1 ];

  s = "";

  memset ( buf, 0, MAXRECV + 1 );

  int status;
  status = ::recv ( m_sock, buf, MAXRECV, 0 );

  if ( status == -1 )
    {
      std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
      return 0;
    }
  else if ( status == 0 )
    {
      return 0;
    }
  else
    {
      s = buf;
      return status;
    }
}

int Socket::recv ( int *frame ) const
{
  int buf [ MAXRECV + 1 ];
  
  int status = ::recv ( m_sock, buf, MAXRECV, MSG_WAITALL );

	if ( status == -1 ) {
      std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
      return 0;
    }
  else if ( status == 0 )
    {
      return 0;
    }
  else
    {
      frame = buf;
      return status;
    }
}

int Socket::recv ( int *frame, int size ) {
	
	//int *buf = new int[size];
	
	int buf [ MAXRECV + 1 ];
  
	//int offset = 0;
    
	int status = ::recv ( m_sock, buf, size, MSG_WAITALL );
  
	if(status != size)
		perror("status diferente de size. recv Socket.cpp\n");
  
	#ifdef DEBUG
	cout << "Tamanho pedido " << size << " tamanho recebido " << status << " sizeof " << sizeof(buf) << endl;
	#endif
  
	if ( status == -1 ) {
		std::cout << "status == -1   errno == " << errno << "  in Socket::recv\n";
		return 0;
	} else if ( status == 0 ){
		return 0;
	} else {
      memcpy(frame,buf,size);
      return status;
	}
  
}



bool Socket::connect ( const std::string host, const int port )
{
  if ( ! is_valid() ) return false;

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons ( port );

  int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

  if ( errno == EAFNOSUPPORT ) return false;

  status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( status == 0 )
    return true;
  else
    return false;
}

void Socket::set_non_blocking ( const bool b )
{

  int opts;

  opts = fcntl ( m_sock,
		 F_GETFL );

  if ( opts < 0 )
    {
      return;
    }

  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );

  fcntl ( m_sock,
	  F_SETFL,opts );

}
