#ifndef ESOCKET_H
#define ESOCKET_H

#include <Config.h>

namespace Easy2D
{
//////////////
class  Address;
//////////////
class Socket
{
public:

	enum Type
	{
		NONE,
		UDP,
		TCP
	};

	Socket();

	~Socket();

	bool open(Type type);

	bool bind(ushort port);

	bool listen(int backlog = 0);

	Socket* accept(Address & client);
	
	bool connect(const Address & server);
	
	bool enableBlocking();

	bool disableBlocking();

	void close();

	bool isOpen() const;

	Type getType() const;

	bool isBlocking() const;

	bool sendTo(const Address & destination, const void * data, int size);
	
	int receiveFrom(Address & sender, void * data, int size);

	bool send(const void * data, int size);

	int receive(void * data, int size);

	void printError();

private:

	bool blockingMode;
	int socket;
	Type  type;

#ifdef PLATFORM_WINDOW
	friend class WindowsApp;
	static bool initializeSockets();
	static void shutdownSockets();
#endif

};

};
#endif 
