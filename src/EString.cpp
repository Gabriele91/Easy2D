#include <stdafx.h>
#include <EString.h>
#include <Debug.h>
////////////////////////
using namespace Easy2D;
////////////////////////
//a void string
String String::NONE("");
//constructors
String::String()
:std::string()
{
}
String::String(char c,unsigned int rep)
:std::string(rep,c)
{
}
String::String(const String* str)
:std::string()
{
    (*this)=(*str);
}
String::String(const String& str)
:std::string()
{
    (*this)=str;
}
String::String(const std::string& str)
:std::string(str)
{
}
String::String(const char* str)
:std::string(str)
{
}
//find
int String::find(const String& value) const
{
    return basic_string::find(value);
}
//back find
int String::rfind(const String& value) const
{
    return basic_string::find_last_of(value);
}
///
void String::split(const String& delimiters ,
                   std::vector<String>& tokens) const
{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = find_first_of(delimiters, lastPos);
    }
}
void String::replace(const String& toReplace,const String& replaceWith)
{
    int lfind=find(toReplace);
    if(lfind>-1)
        std::string::replace(lfind,toReplace.size(),replaceWith);
}
//replaceALL
void String::replaceAll(const String& toReplace,const String& replaceWith)
{
    int lfind=find(toReplace);
    while(lfind>-1)
    {
        std::string::replace(lfind,toReplace.size(),replaceWith);
        lfind=find(toReplace);
    }
}
//split a string
void String::split(const String& in, std::vector<String> & out, char delim)
{
    //var dec
    size_t i = 0, s = 0, j = 0, alloc_s = 1;
    //memory alloc
    for (size_t i = 0; i != in.size(); ++i)
    {
        if (in[i] == delim)
            ++alloc_s;
    }
    out.resize(alloc_s);
    //loop
    while (i != in.size())
    {
        if (in[i] == delim)
        {
            out[j] = (in.substr(s, i - s));
            ++j;
            s = i + 1;
        }
        ++i;
    }
    //end case
    if (s != (in.size() - 1)){
        out[j] = in.substr(s, i - s);
    }
}
//unsafe split string
void String::unsafe_split(String* in, std::vector<char *> & out, char delim)
{
    //var dec
    size_t i = 0, s = 0, j = 0, alloc_s = 1;
    //memory alloc
    for (size_t i = 0; i != (*in).size(); ++i){
        if ((*in)[i] == delim)
            ++alloc_s;
    }
    out.resize(alloc_s);
    //loop
    while (i != (*in).size()) {
        if ((*in)[i] == delim){
            out[j] = &((*in)[s]);
            (*in)[i] = '\0';
            ++j;
            s = i + 1;
        }
        ++i;
    }
    //end case
    if (s != ((*in).size() - 1)){
        out[j] = (char*)&((*in)[s]);
        (*in)[i] = '\0';
    }
}
//utf8 utils
static const size_t UTF8CharSize(const char* cstr)
{
    //init size of this char
    size_t res = 1;
    //compute size
    for(++cstr; *cstr ; ++cstr, ++res)
    {
        if(!((*cstr)&0x80) || (((*cstr)&0xC0) == 0xC0)) break;
    }
    //return count of byte
    return res;
}
//Unicode utils
size_t String::UTF8length() const
{
    //ptr to cstring
    const char* cstring = c_str();
    //size count
    size_t size = 0;
    //compute size
    for(; *cstring; cstring += UTF8CharSize(cstring), ++size);
    //return unicode size
    return size;
}
uint String::UTF8Char(size_t i) const
{
    //ptr to cstring
    const char* cstring = c_str();
    //index
    size_t index = 0;
    //compute size
    for(; *cstring; cstring += UTF8CharSize(cstring), ++index)
    {
        if(index==i)
        {
            //init ouput
            uint output = 0;
            //size of this char
            size_t csize=UTF8CharSize(cstring);
            //array to int
            switch (csize)
            {
                case 4: output |= ((int)cstring[3])  << 24;
                case 3: output |= ((int)cstring[2])  << 16;
                case 2: output |= ((int)cstring[1])  << 8;
                case 1: output |= ((int)cstring[0]); break;
                default: DEBUG_ASSERT_MSG(0,"UTF8Char, not valid char"); break;
            };
            return output;
        }
    }
    return 0;
}
///
String String::toLower() const
{
    String out(*this);
    for (unsigned int i=0; i<size(); ++i)
    {
        if(out[i]<='Z' && out[i]>='A')
            out[i]-=('Z'-'z');
    }
    return out;
}
String String::toUpper() const
{
    String out(*this);
    for (unsigned int i=0; i<size(); ++i)
    {
        if(out[i]<='z' && out[i]>='a')
            out[i]+=('Z'-'z');
    }
    return out;
}
//overload
char& String::operator[](int i)
{
    return (*((std::string*)this))[i];
}
char String::operator[](int i) const
{
    return (*((std::string*)this))[i];
}
//cast
String::operator const char *()
{
    return c_str();
}
String::operator const char *() const
{
    return c_str();
}
//String buffer for printf
String Easy2D::operator*(const String& s,short c)
{
    return s*((ushort)c);
}
String Easy2D::operator*(const String& s,int c)
{
    return s*((uint)c);
}
String Easy2D::operator*(const String& s,long c)
{
    return s*((ulong)c);
}
String Easy2D::operator*(const String& s,ushort c)
{
    String out;
    for(ushort i=0;i!=c;++i) out+=s;
    return out;
}
String Easy2D::operator*(const String& s,uint c)
{
    String out;
    for(uint i=0;i!=c;++i) out+=s;
    return out;
}
String Easy2D::operator*(const String& s,ulong c)
{
    String out;
    for(ulong i=0;i!=c;++i) out+=s;
    return out;
}
///
//converti una stringa a qualunque tipo
double String::toDouble()  const
{
    return strtod(c_str() ,NULL);
}
int String::toInt()  const
{
    return atoi(c_str());
}
const char* String::toChars()  const
{
    return c_str();
}
float String::toFloat()  const
{
    return (float)atof(c_str());
}
//

//int Easy2D::operator
String Easy2D::operator+(const String& s,int v)
{
    return s+String::toString(v);
}
String Easy2D::operator+(int v,const String& s)
{
    return String::toString(v)+s;
}
String Easy2D::operator+(const String& s,unsigned int v)
{
    return s+String::toString(v);
}
String Easy2D::operator+(unsigned int v,const String& s)
{
    return String::toString(v)+s;
}

String Easy2D::operator+(const String& s,long v)
{
    return s+String::toString(v);
}
String Easy2D::operator+(long v,const String& s)
{
    return String::toString(v)+s;
}
String Easy2D::operator+(const String& s,unsigned long v)
{
    return s+String::toString(v);
}
String Easy2D::operator+(unsigned long v,const String& s)
{
    return String::toString(v)+s;
}

String Easy2D::operator+(const String& s,short v)
{
    return s+String::toString(v);
}
String Easy2D::operator+(short v,const String& s)
{
    return String::toString(v)+s;
}
String Easy2D::operator+(const String& s,unsigned short v)
{
    return s+String::toString(v);
}
String Easy2D::operator+(unsigned short v,const String& s)
{
    return String::toString(v)+s;
}

//float Easy2D::operator
String Easy2D::operator+(const String& s,float v)
{
    return s+String::toString(v);
}
String Easy2D::operator+(float v,const String& s)
{
    return String::toString(v)+s;
}
//double Easy2D::operator
String Easy2D::operator+(const String& s,double v)
{
    return s+String::toString(v);
}
String Easy2D::operator+(double v,const String& s)
{
    return String::toString(v)+s;
}
//char Easy2D::operator
String Easy2D::operator+(char c,const String& s)
{
    return String::toString(c)+s;
}
String Easy2D::operator+(const String& s,char c)
{
    return s+String::toString(c);
}