#ifndef EADDRESS_H
#define EADDRESS_H

#include <Config.h>
#include <Types.h>

namespace Easy2D
{
	class Address
	{
	public:

		static Address ANY;
		static Address NONE;

		Address()
		{
			address = 0;
			port = 0;
		}

		Address(uchar a, 
				uchar b, 
				uchar c,
				uchar d, 
				ushort port)
		{
			this->address = (a << 24) | (b << 16) | (c << 8) | d;
			this->port = port;
		}

		Address(unsigned int address, unsigned short port)
		{
			this->address = address;
			this->port = port;
		}

		unsigned int getAddress() const
		{
			return address;
		}

		unsigned char getA() const
		{
			return (unsigned char)(address >> 24);
		}

		unsigned char getB() const
		{
			return (unsigned char)(address >> 16);
		}

		unsigned char getC() const
		{
			return (unsigned char)(address >> 8);
		}

		unsigned char getD() const
		{
			return (unsigned char)(address);
		}

		unsigned short getPort() const
		{
			return port;
		}

		bool operator == (const Address & other) const
		{
			return address == other.address && port == other.port;
		}

		bool operator != (const Address & other) const
		{
			return !(*this == other);
		}

		bool operator < (const Address & other) const
		{
			return  other.address < other.address;
		}

	private:

		uint address;
		ushort port;

	};

};

#endif