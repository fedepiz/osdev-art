#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H
#include <stddef.h>
namespace kstd {
   class string {
        private:
        char* buffer;
        size_t length;
        public:
        string();
        string(const char* str);
        string(const string& other);
        ~string();
        string& operator=(const string& other);
        unsigned int size() const;
        char operator[](unsigned int index) const;
        static int compare(const string& s1, const string& s2);
        const char* str();
    };
};
#endif