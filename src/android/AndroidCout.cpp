#include <android/log.h>
#include <string>
#include <iostream>
#include <sstream>

/* android debug */
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Easy2D", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Easy2D", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "Easy2D", __VA_ARGS__))

#define LOGWRI(str) ((void)__android_log_write(ANDROID_LOG_INFO, "Easy2D", str))
#define LOGWRW(str) ((void)__android_log_write(ANDROID_LOG_WARN, "Easy2D", str))
#define LOGWRE(str) ((void)__android_log_write(ANDROID_LOG_ERROR, "Easy2D", str))


//rewrite cout
class androidbuf: public std::streambuf 
{

	public:

		enum { 
			bufsize = 1028 
		}; // ... or some other suitable buffer size
		
		char strBuffer[bufsize];

		androidbuf()
		{ 
			this->setp(strBuffer, strBuffer + bufsize - 1);
		}

	private:
		int overflow(int c) {
			if (c == traits_type::eof()) { 
				*this->pptr() = traits_type::to_char_type(c);
				this->sbumpc();
			}
			return this->sync()? traits_type::eof(): traits_type::not_eof(c);
		}
		int sync() {
			int rc = 0;
			if (this->pbase() != this->pptr()) {
				std::string buffer;
				buffer.assign(this->pbase(), this->pptr() - this->pbase());
				LOGE(buffer.c_str());
				rc = true;
				this->setp(strBuffer, strBuffer + bufsize - 1);
			}
			return rc;
		}
};

extern "C" void overloadSTDCOUT(){
	std::cout.rdbuf(new androidbuf);
}
extern "C" void unoverloadSTDCOUT(){
	delete std::cout.rdbuf(0);
}