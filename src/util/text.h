#ifndef UTIL_TEXT_H
#define UTIL_TEXT_H

#include <stdarg.h>
#include <string.h>

namespace util {
    char* vsformat(const char* fmt, va_list args);
    char* format(const char* fmt, ...);

    template <typename T> static void writestring(T writeF, const char* str) {
        while(*str != '\0') {
            writeF(*str);
            str++;
        }
    }

    template <typename T> void implace_format(T writeF, const char* fmt, va_list args) {
        //While we have some format characters
        while(*fmt != '\0') {
            //is it special?
            if(*fmt == '%') {
                //Advance it
                fmt++;
                char c = *fmt;
                if(c == '%') {
                    //Double % - just counts as one
                    writeF(c);
                } else if (c == 'd') {
                    //print integer in decimal form
                    int x = va_arg(args, int);
                    writestring(writeF, kstd::itoa(x).str);
                }
                else if (c == 'i') {
                    unsigned int x = va_arg(args, int);
                    writestring(writeF, kstd::itoa(x).str);
                } else if (c == 'x') {
                    //hex
                    int x = va_arg(args, int);
                    writestring(writeF, "0x");
                    writestring(writeF, kstd::itoa(x,16).str);
                } else if (c == 's') {
                    //String within a string
                    char* s = va_arg(args, char*);
                    writestring(writeF, s);
                } else if (c == 'b') {
                    int b = va_arg(args, int);
                    if(b == 1) {
                        writestring(writeF, "true");
                    } else {
                        writestring(writeF, "false");
                    }
                }
                else if (c == 'c') {
                    char ch = (char)va_arg(args, int);
                    writeF(ch);
                }
            } else {
                //no - just add it to the vector
                writeF(*fmt);
            }
            //Advance
            fmt++;
        }    
    }

    void printf(const char* fmt, ...);
    void logf(const char* fmt, ...);
    void panicf(const char* fmt, ...);
    kstd::string stringf(const char* fmt, ...);
};
#endif