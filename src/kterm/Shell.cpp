#include <kterm/Shell.h>
#include <kterm/Terminal.h>
#include <string.h>
#include <util/vector.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <util/text.h>
#include <filesystem/VFS.h>
#include <memory/debug.h>

#include<tasks/tasks.h>

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
        this->quitSignal = false;
        this->term = term;
    }

    Terminal* Shell::getTerminal() {
        return this->term;
    }

    void Shell::mainLoop() {
        this->term->puts("Welcome to Art 0.1a\n");
        print_rainbow(this->term, "Beauty lies in the eye of the beholder\n");

        while(!this->quitSignal) {
            string line = this->getTerminal()->gets();
            processCommand(line);
        }
    }

    typedef void (*processCommand_t)(Shell* shell, const vector<string>& line);

    struct shellCommand {
        const char* name;
        const char* helpText;
        processCommand_t handler;
    };

    void handleHelp(Shell* shell, const vector<string> &line);
    void handleQuit(Shell* shell, const vector<string> &line);
    void handleDebug(Shell* shell, const vector<string> &line);
    void handleEcho(Shell *shell, const vector<string> &line);

    shellCommand commandTable[] = {
        { "help" , "Usage: Help [commandName]", handleHelp},
        { "quit", "Exits kTerm", handleQuit },
        { "debug", "Prints debug information. Usage: debug [frames|pages|fs]", handleDebug },
        { "echo", "Parrots back something. Usage: echo [anything]", handleEcho },
        { nullptr, nullptr, nullptr }
    };

    unsigned int countCommandsInTable(shellCommand* table) {
        unsigned int counter = 0;
        while(table[counter].name != nullptr) {
            counter++;
        }
        return counter;
    }

    void handleQuit(Shell* shell, const vector<string> &line) {
        (void)line;
        shell->getTerminal()->puts("KTerm closing...\n");
        shell->quitSignal = true;
    }

    void handleHelp(Shell* shell, const vector<string> &line) {
        Terminal *term = shell->getTerminal();
        if(line.size() == 1) {
            term->puts("Valid commands\n");
            for(unsigned int i = 0; i < countCommandsInTable(commandTable);i++) {
                shellCommand* command = &commandTable[i];
                term->puts(command->name);
                term->puts(" - ");
                term->puts(command->helpText);
                term->puts("\n");
            }
        }
    }

    void handleDebug(Shell* shell, const vector<string> &line) {
        Terminal* term = shell->getTerminal();
        if(line.size() == 1) {
            term->puts("Usage: debug [frames|pages|fs]\n");
            return;
        } 
        string type = line.get(1);
        if(type == "pages") {
            memory::kernel_page_allocator_debug(true);
        } else if (type == "frames") {
            memory::kernel_frame_allocator_debug(true);    
        }
        else if(type == "fs") {
            vfs::VFSNode* root = vfs::getRoot();
            term->puts(root->getName().str());
            term->puts("\n");
        } else {
            term->puts("Unknown debug type\n");
        }
    }

    void handleEcho(Shell *shell, const vector<string> &line) {
        Terminal* term = shell->getTerminal();
        if(line.size() == 1) {
            term->puts("echo [anything]\n");
            return;
        }
        string message = line.get(1);
        //strip things away if is literal
        if(message[0] == '"') {
            message = message.substring(1, message.size() - 2);
        }
        term->puts(message);
        term->puts("\n");
    }


    void Shell::processCommand(const string &line) {
        auto vec = line.split(' ', false, '"', true);
        if(vec.size() == 0) {
            return;
        }
        string commandName = vec[0];
        for(unsigned int i = 0; i < countCommandsInTable(commandTable); i++) {
            shellCommand *command = &commandTable[i];
            if(string(command->name) == commandName) {
                command->handler(this, vec);
                return;
            }
        }
        string str = util::stringf("Unknown command %s\n", commandName.str());
        logf("Parsed unkown commmand %s\n", str.str());
        //logf("Character breakdown:\n");
        //for(unsigned int i = 0; i < commandName.size();i++) {
        //    logf("Character %d: '%c'(%d)\n", i, commandName[i], commandName[i]);
        //} 
        this->term->puts(str.str());
    }
};
