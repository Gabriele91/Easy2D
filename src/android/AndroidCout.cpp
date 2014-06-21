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

/*
//rewrite cout
class androidbuf: public std::streambuf
{

	public:
	int sync() {
			unsigned int count = this->pptr() - this->pbase();//out_waiting();
			std::string buffer;
			buffer.assign(this->pbase(), count);
			LOGWRE(buffer.c_str());
			return std::streambuf::sync();
		}
};
*/
template <typename charT, typename traits = std::char_traits<charT> >
class androidbuf : public std::basic_streambuf<charT, traits>
{
public:
    // The size of the input and output buffers.
    static const size_t BUFF_SIZE = 1024;
    typedef traits traits_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;

    // You can use any method that you want, but here, we'll just take in a raw streambuf as a
    // slave I/O object.  xor_char is what each character is xored with before output.
    explicit androidbuf()
        : out_buf_(new charT[BUFF_SIZE])
    {
        // Initialize the put pointer.  Overflow won't get called until this buffer is filled up,
        // so we need to use valid pointers.
        this->setp(out_buf_, out_buf_ + BUFF_SIZE - 1);
    }

    // It's a good idea to release any resources when done using them.
    ~androidbuf()
    {
        delete [] out_buf_;
    }

protected:
    // This is called when there are too many characters in the buffer (thus, a write needs to be performed).
    virtual int_type overflow(int_type c);
    // This is called when the buffer needs to be flushed.
    virtual int_type sync();


private:
    // Output buffer
    charT* out_buf_;
};
template <typename charT, typename traits>
typename androidbuf<charT, traits>::int_type
androidbuf<charT, traits>::overflow(androidbuf<charT, traits>::int_type c)
{
    charT* ibegin = this->out_buf_;
    charT* iend = this->pptr();

    // Reset the put pointers to indicate that the buffer is free
    // (at least it will be at the end of this function).
    setp(out_buf_, out_buf_ + BUFF_SIZE + 1);

    // If this is the end, add an eof character to the buffer.
    // This is why the pointers passed to setp are off by 1
    // (to reserve room for this).
    if(!traits_type::eq_int_type(c, traits_type::eof()))
    {
        *iend++ = traits_type::to_char_type(c);
    }

    // Compute the write length.
    int_type ilen = iend - ibegin;

    //Do something with the string
    //printf("String: %.*s\n", ilen, out_buf_);
    std::string buffer;
    buffer.assign(out_buf_, ilen);
    LOGWRE(buffer.c_str());

    return traits_type::not_eof(c);
}

// This is called to flush the buffer.
// This is called when we're done with the file stream (or when .flush() is called).
template <typename charT, typename traits>
typename androidbuf<charT, traits>::int_type
androidbuf<charT, traits>::sync()
{
    return traits_type::eq_int_type(this->overflow(traits_type::eof()),
                                    traits_type::eof()) ? -1 : 0;
}

extern "C" void overloadSTDCOUT()
{
    std::cout.rdbuf(new androidbuf<char>());
}
extern "C" void unoverloadSTDCOUT()
{
    delete std::cout.rdbuf(0);
}


