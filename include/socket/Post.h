#ifndef TCP_POST_H
#define TCP_POST_H

#include <Config.h>
#include <EString.h>
#include <Socket.h>

namespace Easy2D
{

class Post
{
public:

    struct Field
    {
        String name;
        String value;
    };

    void push(const Field field)
    {
        attributes.push_back(field);
    }

    size_t size() const
    {
        return attributes.size();
    }

    void clear()
    {
        attributes.clear();
    }

    Field& operator[](size_t i)
    {
        return attributes[i];
    }

    const Field& operator[](size_t i) const
    {
        return attributes[i];
    }

    String content() const
    {
        String buffer;
        for (const Field& field : attributes)
        {
            if (buffer.size()) buffer += "&";
            buffer += field.name + "=" + field.value;
        }
        return buffer;
    }

    std::vector<byte> form(const String& formAction,
                           const String& host, 
                           bool closeConnection = false)
    {
        String bufContent = content();
        String bufHeader;
        bufHeader += "POST " + formAction + " HTTP/1.1\r\n";
        bufHeader += "Content-Type: application/x-www-form-urlencoded; charset=utf-8\r\n";
        bufHeader += "Accept: */*\r\n";
        bufHeader += "Host: " + host + "\r\n";
        bufHeader += "Content-Length: " + String::toString(bufContent.length()) + "\r\n";
        //close connection
        if (closeConnection)
            bufHeader += "Connection: close\r\n\r\n";
        //ouput
        std::vector<byte> buffer(bufHeader.size() + bufContent.size() + 1);
        std::memset(buffer.data(), 0, buffer.size());
        std::memcpy(buffer.data(), bufHeader.data(),  bufHeader.size());
        std::memcpy(buffer.data() + bufHeader.size(), bufContent.data(), bufContent.size() + 1);

        return buffer;
    }

protected:

    std::vector<Field> attributes;

};

};

#endif