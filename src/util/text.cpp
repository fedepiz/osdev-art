#include <util/text.h>
#include <util/vector.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <stdarg.h>

namespace util {
    using util::vector;
    using kstd::itoa;
    using kstd::log;

    void push_string(vector<char>& vec, const char* str) {
        //While the string is not over
        while(*str != '\0') {
            vec.push_back(*str);
            str++;
        }
    }

    char* vector_as_string(vector<char>& vec) {
        char* str = new char[vec.size()+1];
        for(unsigned int i = 0; i < vec.size(); i++) {
            str[i] = vec[i];
        }
        str[vec.size()] = '\0';
        return str;
    }

    char* vsformat(const char* fmt, va_list args) {
        vector<char> vec;
        //While we have some format characters
        while(*fmt != '\0') {
            //is it special?
            if(*fmt == '%') {
                //Advance it
                fmt++;
                char c = *fmt;
                if(c == '%') {
                    //Double % - just counts as one
                    vec.push_back(c);
                } else if (c == 'd') {
                    //print integer in decimal form
                    int x = va_arg(args, int);
                    push_string(vec, kstd::itoa(x).str);
                } else if (c == 'x') {
                    //hex
                    int x = va_arg(args, int);
                    push_string(vec, "0x");
                    push_string(vec, kstd::itoa(x,16).str);
                } else if (c == 's') {
                    //String within a string
                    char* s = va_arg(args, char*);
                    push_string(vec, s);
                } else if (c == 'c') {
                    char c = (char)va_arg(args, int);
                    vec.push_back(c);
                }
            } else {
                //no - just add it to the vector
                vec.push_back(*fmt);
            }
            //Advance
            fmt++;
        }
        //Now return the string represented in this vector
        return vector_as_string(vec);
    }

    char* format(const char* fmt, ...) {
        vector<char> vec;
        va_list args;
        va_start(args, fmt);
        //While we have some format characters
        char* res = vsformat(fmt, args);
        va_end(args);
        //Now return the string represented in this vector
        return res;
    }

    void printf(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        implace_format(&kstd::putch, fmt, args);
        va_end(args);

        //kstd::puts(str);
        //delete str;
    }
    void logf(const char* fmt, ...){
        va_list args;
        va_start(args, fmt);
        implace_format(&kstd::logch, fmt, args);
        va_end(args);

        //kstd::log(str);
        //delete str;
    }
    void panicf(const char* fmt, ...){
        va_list args;
        va_start(args, fmt);
        implace_format(&kstd::puterrch, fmt, args);
        for(;;){
            
        }
        va_end(args);
    }

    kstd::string stringf(const char* fmt, ...) {
        vector<char> vec;
        auto f = [&] (char c) { vec.push_back(c); };
        va_list args;
        va_start(args, fmt);
        implace_format(f, fmt, args);
        va_end(args);
        return kstd::string(vec.buffer(), vec.size());
    }
};