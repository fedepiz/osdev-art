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

        {
            vector<string> vec;
            vec.push_back(string("Test"));
            vec.push_back(string("it"));
            memory::kernelHeapLogState();
        }
        memory::kernelHeapLogState();
        bool keep_going = false;
        while(keep_going) {
            //string line = this->term->gets();
            this->processCommand();
            //keep_going = line != string("quit");
        }
    }

    void Shell::processCommand() {
        //string s = string("Test/it/now");           
        //auto vec = s.split('/', false);
        //for(unsigned i = 0; i < vec.size(); i++) {
        //    this->getTerminal()->puts(vec[i].str());
        //}
    }
};
