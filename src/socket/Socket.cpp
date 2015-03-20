#include <stdafx.h>
#include <Socket.h>
#include <Address.h>
#include <Debug.h>

#ifdef PLATFORM_WINDOW

	#include <winsock2.h>
	#pragma comment( lib, "wsock32.lib" )
	typedef int socklen_t;
	#define PRINT_SOCKET_ERROR DEBUG_MESSAGE("Socket error: " << WSAGetLastError()); 
	#define NOBLOCK_PRINT_SOCKET_ERROR                           \
	{                                                            \
		int err = WSAGetLastError();                             \
		if (err && err!=WSAEWOULDBLOCK)                          \
		{                                                        \
			DEBUG_MESSAGE("Socket error: " << WSAGetLastError());\
		}                                                        \
	}
	
#elif defined( PLATFORM_UNIX )

	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <fcntl.h>
	#define PRINT_SOCKET_ERROR DEBUG_MESSAGE("Socket error: ..")
	#define NOBLOCK_PRINT_SOCKET_ERROR  DEBUG_MESSAGE("Socket error: ..")
	#define INVALID_SOCKET (-1)

#else

	#error unknown platform!

#endif

using namespace Easy2D;
//static values
Address Address::ANY = Address(INADDR_ANY,  0x0000);
Address Address::NONE = Address(INADDR_NONE, 0xffff);
//socket
Socket::Socket()
{
	socket = 0;
	type = NONE;
	blockingMode = true;
}

Socket::~Socket()
{
	close();
}

bool Socket::open(Type typeSocket)
{
	DEBUG_ASSERT(!isOpen());

	// create socket
	switch (typeSocket)
	{
		case UDP: socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); break;
		case TCP: socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); break;
		default: socket = 0; break;
	}

	if (socket <= 0)
	{
		DEBUG_MESSAGE("failed to create socket");	
		PRINT_SOCKET_ERROR;
		socket = 0;
		type = NONE;
		return false;
	}

	//save type
	type = typeSocket;
	// set non-blocking io

	return true;
}

bool Socket::enableBlocking()
{
	if(blockingMode) return true;
#ifdef PLATFORM_UNIX
	int nonBlocking = 0;
	if (fcntl(socket, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
	{
		DEBUG_MESSAGE("failed to set non-blocking socket");
		PRINT_SOCKET_ERROR;
		close();
		return false;
	}
#elif defined( PLATFORM_WINDOW )
	u_long nonBlocking = 0;
	if (ioctlsocket(socket, FIONBIO, &nonBlocking) != 0)
	{
		DEBUG_MESSAGE("failed to set non-blocking socket");
		PRINT_SOCKET_ERROR;
		close();
		return false;
	}
#endif
	blockingMode = true; 
	return true;
}

bool Socket::disableBlocking()
{
	if (!blockingMode) return true;
#ifdef PLATFORM_UNIX
	int nonBlocking = 1;
	if (fcntl(socket, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
	{
		DEBUG_MESSAGE("failed to set non-blocking socket");
		PRINT_SOCKET_ERROR;
		close();
		return false;
	}
#elif defined( PLATFORM_WINDOW )
	u_long nonBlocking = 1;
	if (ioctlsocket(socket, FIONBIO, &nonBlocking) != 0)
	{
		DEBUG_MESSAGE("failed to set non-blocking socket");
		PRINT_SOCKET_ERROR;
		close();
		return false;
	}
#endif
	blockingMode = false; 
	return true;
}

bool Socket::bind(ushort port)
{
	DEBUG_ASSERT(isOpen());
	// bind to port
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons((unsigned short)port);

	if (::bind(socket, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
	{
		DEBUG_MESSAGE("failed to bind socket");
		PRINT_SOCKET_ERROR;
		/* close(); */
		return false;
	}
	return true;
}

bool Socket::connect(const Address & server)
{
	DEBUG_ASSERT(isOpen());
	DEBUG_ASSERT(getType() == TCP);
	//connect
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(server.getAddress());
	address.sin_port = htons((unsigned short)server.getPort());
	int cret = ::connect(socket,(struct sockaddr*) &(address),sizeof(struct sockaddr_in));
	//fail
	if (cret != 0)
	{
		DEBUG_MESSAGE("failed to connect");
		NOBLOCK_PRINT_SOCKET_ERROR;
		/* close(); */
		return false;
	}
	//return true
	return true;
}

Socket* Socket::accept(Address& client)
{
	DEBUG_ASSERT(isOpen());
	DEBUG_ASSERT(getType() == TCP);

	sockaddr_in from;
	socklen_t fromLength = sizeof(from);
	int csocket = ::accept(this->socket, 
						  (struct sockaddr*)&from,
						  &fromLength);
	if (csocket != INVALID_SOCKET)
	{
		uint address = ntohl(from.sin_addr.s_addr);
		uint port = ntohs(from.sin_port);
		auto clientSocket = new Socket();
		clientSocket->socket = csocket;
		clientSocket->type   = TCP;
		client = Address(address, port);
		return clientSocket;
	}
	NOBLOCK_PRINT_SOCKET_ERROR;
	return nullptr;
}

bool Socket::listen(int backlog)
{
	DEBUG_ASSERT(isOpen());
	DEBUG_ASSERT(getType() == TCP);

	if( ::listen(socket, backlog) != SOCKET_ERROR ) return true;

	PRINT_SOCKET_ERROR;
	return false;
}

void Socket::close()
{
	if (socket != 0)
	{
		#ifdef PLATFORM_UNIX
				close(socket);
		#elif defined( PLATFORM_WINDOW )
		if (closesocket(socket) == SOCKET_ERROR)
		{
			PRINT_SOCKET_ERROR;
		}
		#endif
		socket = 0;
		type = NONE;
	}
}

bool Socket::isOpen() const
{
	return socket != 0;
}

bool Socket::isBlocking() const
{
	return blockingMode;
}

Socket::Type Socket::getType() const
{
	return type;
}

bool Socket::sendTo(const Address & destination, const void * data, int size)
{
	assert(data);
	assert(size > 0);

	if (socket == 0)
		return false;

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(destination.getAddress());
	address.sin_port = htons((unsigned short)destination.getPort());

	int sent_bytes = ::sendto(socket, (const char*)data, 
									  size, 
									  0, 
									  (sockaddr*)&address, 
									  sizeof(sockaddr_in));

	return sent_bytes == size;
}

int Socket::receiveFrom(Address & sender, void * data, int size)
{
	assert(data);
	assert(size > 0);

	if (socket == 0)
		return false;

	sockaddr_in from;
	socklen_t fromLength = sizeof(from);

	int received_bytes = recvfrom(socket,
		(char*)data,
		size,
		0,
		(sockaddr*)&from,
		&fromLength);

	if (received_bytes <= 0)
		return 0;

	uint address = ntohl(from.sin_addr.s_addr);
	uint port = ntohs(from.sin_port);

	sender = Address(address, port);

	return received_bytes;
}

bool Socket::send(const void * data, int size)
{
	assert(data);
	assert(size > 0);

	if (socket == 0)
		return false;

	int sent_bytes = ::send(socket, (const char*)data, size, 0);

	return sent_bytes == size;
}

int Socket::receive(void * data, int size)
{
	assert(data);
	assert(size > 0);

	if (socket == 0)
		return false;
		
	int received_bytes = recv(socket,(char*)data, size, 0);

	if (received_bytes <= 0)
		return 0;


	return received_bytes;
}

void Socket::printError()
{
	PRINT_SOCKET_ERROR;
}

#ifdef PLATFORM_WINDOW
bool Socket::initializeSockets()
{
	WSADATA WsaData;
	if (WSAStartup(MAKEWORD(2, 2), &WsaData) == 0) return true;
	PRINT_SOCKET_ERROR;
	return false;
}
void Socket::shutdownSockets()
{
	WSACleanup();
}
#endif