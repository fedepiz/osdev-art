#include <string.h>
#include <kstdlib.h>
#include <kstdio.h>
namespace kstd {
    string::string() {
        this->buffer = new char[1];
        this->buffer[0] = '\0';
        this->length = 0;
    }
    string::string(const char* str) {
        size_t len = strlen(str)+1;
        this->buffer = new char[len];
        this->length = len-1;
        kstd::memcpy(this->buffer, str, len);
    }
    string::string(const string& other) {
        this->length = other.length;
        this->buffer = new char[this->length+1];
        kstd::memcpy(this->buffer, other.buffer, this->length+1);
    }

    string::~string() {
        delete this->buffer;
    }
    string& string::operator=(const string& other) {
        this->length = other.length;
        this->buffer = new char[this->length+1];
        kstd::memcpy(this->buffer, other.buffer, this->length+1);
        return *this;
    }

    unsigned int string::size() const {
        return this->length;
    }
    char string::operator[](unsigned int index) const {
        if(index > this->length) {
            panic("Index out of bounds in string access");
        }
        return this->buffer[index];
    }

    const char* string::str() {
        return this->buffer;
    }
    int string::compare(const string& s1,const string& s2) {
        return kstd::strcmp(s1.buffer, s2.buffer);
    }
};