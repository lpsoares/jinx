// Definition of the Socket class

#ifndef Socket_class
#define Socket_class


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <arpa/inet.h>


const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 3*3*307200;
//const int MSG_NOSIGNAL = 0; // defined by dgame

class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  bool create();
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& ) const;

  // Client initialization
  bool connect ( const std::string host, const int port );

  // Data Transimission
  bool send ( const std::string ) const;
  //  bool send ( unsigned char *frame );
  bool send ( int *s ) const;
 
  int send ( int *s, int size );
  
  int recv ( std::string& ) const;
  int recv ( int *frame ) const;
  int recv ( int *frame, int size );
  void set_non_blocking ( const bool );

  bool is_valid() const { return m_sock != -1; }

  int getFd() {return m_sock;}
  
 private:

  int m_sock;
  sockaddr_in m_addr;


};


#endif
