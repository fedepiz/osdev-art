#include <string.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <util/text.h>

namespace kstd {
    using util::logf;
    using util::vector;

    string::string() {
        //logf("Allocating string\n");
        this->buffer = new char[1];
        this->buffer[0] = '\0';
        this->length = 0;
    }
    string::string(const char* str) {
        //logf("Allocating string\n");
        size_t len = strlen(str);
        this->buffer = new char[len+1];
        this->length = len;
        kstd::memcpy(this->buffer, str, len+1);
    }

    string::string(const char* str, unsigned int count) {
        size_t len = count+1;
        this->buffer = new char[len];
        this->length = count;
        kstd::memcpy(this->buffer, str, count);
        //Null terminate the string
        this->buffer[count] = '\0';
    }

    string::string(const string& other) {
        //logf("Allocating string\n");
        this->length = other.length;
        this->buffer = new char[this->length+1];
        kstd::memcpy(this->buffer, other.buffer, this->length+1);
    }

    string::~string() {
        //logf("Freeing string\n");
        delete[] this->buffer;
    }
    string& string::operator=(const string& other) {
        this->length = other.length;
        delete[] this->buffer;
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

    const char* string::str() const {
        return this->buffer;
    }
    bool string::compare(const string& s1,const string& s2) {
        if(s1.length != s2.length) {
            return false;
        }
        for(unsigned int i = 0; i < s1.length; i++) {
            if(s1[i] != s2[i]) return false;
        }
        return true;
    }

    bool string::operator==(const string& other) const {
        return string::compare(*this, other);
    }

    bool string::operator!=(const string& other) const {
        return !(*this == other);
    }

    vector<string> string::split(char sep) const {
        return this->split(sep, true);
    }

    vector<string> string::split(char sep, bool repsep) const {
        vector<string> container;
        container.push_back(*this);
        return container;
    }
};