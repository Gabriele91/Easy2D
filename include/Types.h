#ifndef TYPES_H
#define TYPES_H


namespace Easy2D
{

//unsigned types
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
//bit types
typedef unsigned char bit8;
typedef unsigned short bit16;
typedef unsigned int bit32;
//byte types
typedef unsigned char byte;
typedef unsigned short byte2;
typedef unsigned int byte4;
#if defined(_MSC_VER)
typedef __int64    bit64;
typedef __int64    byte16;
#else
typedef long long  bit64;
typedef long long  byte16;
#endif

};

#endif