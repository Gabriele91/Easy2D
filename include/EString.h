#ifndef ESTRING_H
#define ESTRING_H

#include <Config.h>

namespace Easy2D
{

class String : public std::string,
               public Pointers< String >
{

public:

    static String NONE;

    String();
    String(char c,unsigned int rep=1);
    String(const String* str);
    String(const String& str);
    String(const std::string& str);
    String(const char* str);
    //find
    int find(const String& value) const;
    int rfind(const String& value) const;
    //replace
    void replace(const String& toReplace,const String& replaceWith);
    //replaceALL
    void replaceAll(const String& toReplace,const String& replaceWith);
    //split
    void split(const String& delimiters , std::vector<String>& tokens) const;
    //to string
    template<class T> static String toString(T val)
    {
        return std::to_string(val);
    }
    static void split(const String& in, std::vector<String> & out, char delim);
    static void unsafe_split(String* in, std::vector<char *> & out, char delim);
    //lower/upper string
    String toLower() const;
    String toUpper() const;
    //cast utils
    double toDouble() const;
    int toInt() const;
    const char* toChars() const;
    float toFloat() const;
    //Unicode utils
    size_t UTF8length() const;
    uint   UTF8Char(size_t i) const;
    //overload
    char& operator[](int i);
    char operator[](int i) const;
    //cast
    operator const char *();
    operator const char *() const;

};
    
String operator*(const String& s,short c);
String operator*(const String& s,int c);
String operator*(const String& s,long c);
String operator*(const String& s,ushort c);
String operator*(const String& s,uint c);
String operator*(const String& s,ulong c);
//operatori
//int operator
String operator+(const String& s,int v);
String operator+(int v,const String& s);
String operator+(const String& s,unsigned int v);
String operator+(unsigned int v,const String& s);
String operator+(const String& s,long v);
String operator+(long v,const String& s);
String operator+(const String& s,unsigned long v);
String operator+(unsigned long v,const String& s);
String operator+(const String& s,short v);
String operator+(short v,const String& s);
String operator+(const String& s,unsigned short v);
String operator+(unsigned short v,const String& s);
//float operator
String operator+(const String& s,float v);
String operator+(float v,const String& s);
//double operator
String operator+(const String& s,double v);
String operator+(double v,const String& s);
//char operator
String operator+(char c,const String& s);
String operator+(const String& s,char c);

};

namespace std
{
template<>
struct DNOSTDHASH< Easy2D::String >
{
public:
    size_t operator()(const Easy2D::String& val) const
    {
        return DNOSTDHASH<std::string>()(val);
    }
    size_t operator()(const Easy2D::String* val) const
    {
        return DNOSTDHASH<std::string>()(*val);
    }
};
};

#endif
