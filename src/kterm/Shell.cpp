#include <kterm/Shell.h>
#include <kterm/Terminal.h>
#include <string.h>
#include <util/vector.h>
#include <kstdio.h>
#include <kstdlib.h>
#include <util/text.h>
#include <filesystem/VFS.h>
#include <filesystem/DataOps.h>
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
    void handleFileinfo(Shell *shell, const vector<string> &line);
    void handleHexdump(Shell* shell, const vector<string> &line);

    shellCommand commandTable[] = {
        { "help" , "Usage: Help [commandName]", handleHelp},
        { "quit", "Exits kTerm", handleQuit },
        { "debug", "Prints debug information. Usage: debug [frames|pages|fs]", handleDebug },
        { "echo", "Parrots back something. Usage: echo [anything]", handleEcho },
        { "fileinfo", "Displays file information. Usage: fileinfo [filename]", handleFileinfo }, 
        { "hexdump", "Prints the file binary content. Usage: hexdump [filename]", handleHexdump },
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

    static void printVFSNode(Terminal* term, vfs::VFSNode* node, int indent) {
        for(int i = 0; i < indent;i++) {
            term->putchar(' ');
        }
        term->puts(node->getName().str());
        if(node->getChildren()->length() > 0) {
            term->putchar('/');
        }
        term->putchar('\n');
        indent++;
        for(unsigned int i = 0; i < node->getChildren()->length();i++) {
            vfs::VFSNode* child = node->getChildren()->get(i);
            printVFSNode(term, child, indent);
        }
    }

    vfs::VFSNode* getFile(Terminal* term, const string& filename) {
        vfs::VFSNode* fileNode = vfs::getNode(filename);
        if(fileNode == nullptr) {
            term->puts("Cannot find file %s\n", filename.str());
            return nullptr;
        }
        return fileNode;
    }

    void handleFileinfo(Shell *shell, const vector<string> &line) {
        Terminal *term = shell->getTerminal();
        if(line.size() <= 1) {
            term->puts("Usage: fileinfo [filename]\n");
            return;
        }
        vfs::VFSNode* fileNode = getFile(term, line.get(1));
        if(fileNode == nullptr) {
            return;
        } else {
            auto dataOps = fileNode->getDataOps();
            if(dataOps == nullptr) {
                term->puts("Type: Pure virtual file\n");
            }
            else {
                switch(dataOps->getType()) {
                    case vfs::DataOpsType::RANDOM_ACCESS:
                        term->puts("Type: Random Access\n");
                        break;
                    case vfs::DataOpsType::OUTSTREAM:
                        term->puts("Type: Output Stream\n");
                        break;
                    case vfs::DataOpsType::INSTREAM:
                        term->puts("Type: Input Stream\n");
                }
            }
            printVFSNode(term, fileNode, 0);
        }
    }

    void handleHexdump(Shell* shell, const vector<string> &line) {
        Terminal *term = shell->getTerminal();
        if(line.size() <= 1) {
            term->puts("Usage: hexdump [filename] {-log}\n");
            return;
        }

        auto filename = line.get(1);
        vfs::VFSNode* fileNode = getFile(term, filename);
        if(fileNode == nullptr) {
            return;
        }
        auto ops = fileNode->getDataOps();
        if(ops == nullptr || ops->getType() != vfs::DataOpsType::RANDOM_ACCESS) {
            term->puts("Can only dump random access files\n");
            return;
        }
        auto r_ops = (vfs::RandomAccessDataOps*)ops;
        unsigned int count = r_ops->size();
        unsigned char* buffer = new unsigned char[count];
        r_ops->read(0, buffer, count);

        bool hasLog = false;
        if(line.size() > 2 && line.get(2) == string("-log")) {
            hasLog = true;
            term->setOutputDevice(TerminalOutputDevice::SERIAL);
        }  
        for(unsigned int i = 0; i < count; i++) {
            term->puts("%x ", buffer[i]);
            if(i % 5 == 0) term->putchar('\n');            
        }
        term->puts("\n");
        if(hasLog){
            term->setOutputDevice(TerminalOutputDevice::VGA);
        }
        delete[] buffer;
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
            printVFSNode(term, root, 0);
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
        logf("Parsed unkown commmand %s\n", commandName.str());
        logf("Character breakdown:\n");
        for(unsigned int i = 0; i < commandName.size();i++) {
            logf("Character %d: '%c'(%d)\n", i, commandName[i], commandName[i]);
        } 
        this->term->puts("Unknown command %s\n", commandName.str());
    }
};
