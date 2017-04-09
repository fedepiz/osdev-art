#ifndef KERNEL_STRING_H
#define KERNEL_STRING_H

#include <stddef.h>
#include <util/vector.h>
namespace kstd {
   class string {
        private:
        char* buffer;
        size_t length;
        public:
        string();
        string(const char* str);
        string(const char* str, unsigned int count);
        string(const string& other);
        ~string();
        string& operator=(const string& other);
        unsigned int size() const;
        char operator[](unsigned int index) const;
        bool operator==(const string& other) const;
        bool operator!=(const string& other) const;
        static bool compare(const string& s1, const string& s2);
        const char* str() const;
        util::vector<string> split(char sep) const;
        util::vector<string> split(char sep, bool repsep) const;
    };
};
#endif