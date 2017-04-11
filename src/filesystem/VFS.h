#ifndef VFS_H
#define VFS_H

#include <util/list.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <kernel/multiboot.h>
namespace vfs {
    using kstd::string;
    using util::list;

    class VFSNode {
        private:
        string name;
        list<VFSNode*> children;
        public:
        VFSNode(string name);
        VFSNode(const char* string);
        string getName();
        const list<VFSNode*>* getChildren();
        void addChild(VFSNode* child);
    };

    VFSNode* initialize();
    VFSNode* getRoot();
    void mount_modules(VFSNode* parent, multiboot::multiboot_info_t* mbinfo);
};
#endif
