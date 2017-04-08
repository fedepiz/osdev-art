#include <string.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <util/text.h>
#include <memory/subsystem.h>
namespace kstd {
    using util::logf;
    using util::vector;

    string::string() {
        //logf("Allocating string\n");
        memory::getKernelHeap()->setNextTag("S1");
        this->buffer = new char[1];
        this->buffer[0] = '\0';
        this->length = 0;
    }
    string::string(const char* str) {
        //logf("Allocating string\n");
        size_t len = strlen(str)+1;
        memory::getKernelHeap()->setNextTag("S2");
        this->buffer = new char[len];
        this->length = len-1;
        kstd::memcpy(this->buffer, str, len);
    }

    string::string(const char* str, unsigned int count) {
        size_t len = count+1;
        memory::getKernelHeap()->setNextTag("S3");
        this->buffer = new char[len];
        this->length = count;
        kstd::memcpy(this->buffer, str, count);
        //Null terminate the string
        this->buffer[count] = '\0';
    }

    string::string(const string& other) {
        //logf("Allocating string\n");
        this->length = other.length;
        memory::getKernelHeap()->setNextTag("S4");
        this->buffer = new char[this->length+1];
        kstd::memcpy(this->buffer, other.buffer, this->length+1);
    }

    string::string(const vector<char>& vec) {
        this->length = vec.size();
        memory::getKernelHeap()->setNextTag("SVEC");
        this->buffer = new char[this->length+1];
        kstd::memcpy(this->buffer, vec.buffer(), this->length);
        this->buffer[this->length+1] = '\0';
    }


    string::~string() {
        //logf("Freeing string\n");
        logf("Deleting string, buffer address %x\n", (uint32_t)this->buffer);
        delete this->buffer;
    }
    string& string::operator=(const string& other) {
        this->length = other.length;
        delete this->buffer;
        memory::getKernelHeap()->setNextTag("SCPY");
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


    vector<string> string::split(char sep, bool repsep) const {
        vector<string> string_vector;
        vector<char> accum;
        bool metSep = false;
        unsigned int pos = 0;
        while(pos < this->length) {
            //Read in the character
            char ch = this->operator[](pos);
            //Is it a separator?
            if(ch == sep) {
                //Yes
                if(repsep && metSep) {
                    //If we have repsep on and we previously met a separator, skip
                } else {
                    //Otherwise, make a string of the segment
                    string str(accum);
                    string_vector.push_back(str);
                    accum.clear();
                    metSep = true;
                }
            } else {
                //No separator, simply add to the accumulator
                metSep = false;
                accum.push_back(ch);
            }
            pos++;
        }
        //If we have anything left, we need to account for that as well
        if(accum.size() > 0) {
            string_vector.push_back(string(accum));
        }
        return string_vector;
    }
};