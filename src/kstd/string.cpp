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

    string string::substring(size_t startIndex, size_t length) const {
        if(startIndex + length > this->length) {
            panic("Substring too long!");
        }

        char* ptr = this->buffer + startIndex;
        string str(ptr, length);
        return str;
    }

    vector<string> string::split(char sep, bool repsep) const {
        vector<string> container;
        size_t startPos = 0;
        bool met_separator = false;
        for(size_t pos = 0; pos < this->length; pos++) {
            char currentCharacter = (*this)[pos];
            //logf("Current char %d\n", currentCharacter);
            //Is the word a separator?
            if(currentCharacter == sep) {
                //If we met a separator before, and repsep is on, just do nothing
                if(met_separator && repsep) {

                } else {
                    //Otherwise, let's record in a string from the current start to the character before the separator
                    string str = this->substring(startPos, pos - startPos);
                    container.push_back(str);
                }
                //Mark meeting the separator
                met_separator = true;
                //Advance the pos
                startPos = pos+1;
            } else {
                //Just mark
                met_separator = false;
            }
        }
        //If the startPosition is not the same as the length, then we have some leftover to add
        if(startPos < this->length) {
            string str = this->substring(startPos, this->length - startPos);
            container.push_back(str);
        }
        return container;
    }
};