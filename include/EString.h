#ifndef ESTRING_H
#define ESTRING_H

#include <Config.h>

namespace Easy2D{

	class String : public std::string {

	public:

	String():std::string(){};
	String(char c,unsigned int rep):std::string(rep,c){};
	String(const String* str):std::string(){ (*this)=(*str); };
	String(const String& str):std::string(){ (*this)=str; };
	String(const std::string& str):std::string(str){};
	String(const char* str):std::string(str){};
	//find
	int find(const String& value) const;
	int rfind(const String& value) const;
	//replace
	void replace(const String& toReplace,const String& replaceWith){
		int lfind=find(toReplace);
		if(lfind>-1)
			std::string::replace(lfind,toReplace.size(),replaceWith);
	}
	//replaceALL
	void replaceAll(const String& toReplace,const String& replaceWith){
		int lfind=find(toReplace);
		while(lfind>-1){
			std::string::replace(lfind,toReplace.size(),replaceWith);
			lfind=find(toReplace);
		}
	}
	//
	//split
	void split(const String& delimiters , std::vector<String>& tokens);
	//
	template<class T> static String toString(T dato){
		   std::stringstream br;//creo il buffer
		   br << dato;//aggiungo il valore
		   return br.str();//converto in stringa e lo restituisco
		}
	//lower/upper string
	String toLower() const;
	String toUpper() const;
	//
	double toDouble() const;
	int toInt() const;
	const char* toChars() const;
	float toFloat() const;
	//overload
	char& operator[](int i){
		return (*((std::string*)this))[i];
	}
	char operator[](int i) const{
		return (*((std::string*)this))[i];
	}
	//cast
	operator const char *(){
		return c_str();
	}
	operator const char *() const{
		return c_str();
	}

	};

	String operator*(const String& s,const char* c);
	String operator*(const String& s,char* c);
	String operator*(const String& s,int c);
	String operator*(const String& s,unsigned int c);
	String operator*(const String& s,float c);
	String operator*(const String& s,double c);
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

namespace std{
	 template<>
     struct DNOSTDHASH< Easy2D::String > {
			public:
			size_t operator()(const Easy2D::String& val) const {
			  return DNOSTDHASH<std::string>()(val);
			}
			size_t operator()(const Easy2D::String* val) const {
			  return DNOSTDHASH<std::string>()(*val);
			}
	 };
};

#endif
