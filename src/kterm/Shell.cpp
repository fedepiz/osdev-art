#include <kterm/Shell.h>
#include <kterm/Terminal.h>
#include <string.h>
#include <util/vector.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <util/text.h>
#include <memory/debug.h>
namespace kterm {
    using kstd::string;
    using util::vector;
    using util::logf;
    using util::printf;

    void print_rainbow(Terminal* term, const char* ptr) {
	uint8_t color = 0;
	uint8_t colorbg = 6;
    auto prevFGColor = term->getForegroundColor();
    auto prevBGColor = term->getBackgroundColor();
	while(*ptr != '\0') {
		uint8_t actualColor = color;
		uint8_t actualBgColor = colorbg;
		if(actualColor == actualBgColor) {
			actualColor = 15;
			actualBgColor = 0;
		}
        term->setForegroundColor(static_cast<VGAColor>(actualColor));
        term->setBackgroundColor(static_cast<VGAColor>(actualBgColor));
		term->putchar(*ptr);
		color++;
		colorbg--;
		if(color > 6) color = 0;
		if(colorbg == 0) colorbg = 6;
		ptr++;
	}
    term->setForegroundColor(prevFGColor);
    term->setBackgroundColor(prevBGColor);
    }

    Shell::Shell(Terminal* term) {
        this->term = term;
    }

    Terminal* Shell::getTerminal() {
        return this->term;
    }

    void Shell::mainLoop() {
        //this->term->puts("Welcome to Art 0.1a\n");
        //print_rainbow(this->term, "Beauty lies in the eye of the beholder\n");

        bool keep_going = true;
        while(keep_going) {
            //string line = this->term->gets();
            this->processCommand();
            //keep_going = line != string("quit");
            keep_going = false;
        }
    }

    class string2 {
        private:
        char* buffer;
        size_t length;
        public:
        string2() {
            this->buffer = new char[1];
            this->buffer[0] = '\0';
            this->length = 0;
        }
        string2(const char* str) {
            this->length = kstd::strlen(str);
            this->buffer = new char[this->length+1];
            this->buffer[this->length] = '\0';
        }
        string2(const string2& other) {
            this->length = other.length;
            this->buffer = new char[this->length+1];
            for(size_t i = 0; i < this->length;i++) {
                this->buffer[i] = other.buffer[i];
            }
            this->buffer[this->length] = '\0';
        }
        ~string2() {
            logf("Destructor invoked\n");
            //delete this->buffer;
        }
        //string(const char* str);
        //string(const char* str, unsigned int count);
        //string(const string& other);
        //string(const util::vector<char>& vec);
        //~string();
        //string& operator=(const string& other);
        //unsigned int size() const;
        //char operator[](unsigned int index) const;
        //bool operator==(const string& other) const;
        //bool operator!=(const string& other) const;
        //static bool compare(const string& s1, const string& s2);
        //const char* str() const;
        //util::vector<string> split(char sep, bool repsep) const;
    };

    class A {
        public:
        char* c;
        A() {
        }

        void init(){
            this->c = new char[22];
        }
        
        ~A() {
            delete c;
        }
    };

    template <class T> void testWithTemplate() {
       {
           {
               A* arr = new A();
               arr->init();
               delete arr;
           }
       }
    }


    void Shell::processCommand() {
        logf("---INTERESTING SECTION---\n");
        testWithTemplate<A>();
    }
};
