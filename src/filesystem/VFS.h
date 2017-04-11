#ifndef VFS_H
#define VFS_H

#include <util/list.h>
#include <string.h>
#include <kernel/multiboot.h>
#include <filesystem/DataOps.h>
namespace vfs {
    using kstd::string;
    using util::list;

    class VFSNode {
        private:
        string name;
        list<VFSNode*> children;
        DataOps* dataOps;
        public:
        VFSNode(string name, DataOps* ops);
        VFSNode(const char* string, DataOps* ops);
        ~VFSNode();
        string getName() const;
        const list<VFSNode*>* getChildren() const;
        const DataOps* getDataOps() const;
        void addChild(VFSNode* child);
    };

    VFSNode* initialize();
    VFSNode* getRoot();
    VFSNode* getNode(const kstd::string& path);
    VFSNode* getNode(const char* path);
    void mount_modules(VFSNode* parent, multiboot::multiboot_info_t* mbinfo);
};
#endif
