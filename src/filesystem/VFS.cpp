#include <filesystem/VFS.h>
#include <kstdlib.h>
#include <util/text.h>
#include <kernel/multiboot.h>
#include <util/vector.h>
namespace vfs {
    using util::logf;
    using util::panicf;
    using util::vector;
    using kstd::string;

    VFSNode::VFSNode(string name) {
        this->name = name;
    }

    VFSNode::VFSNode(const char* name) {
        this->name = string(name);
    }

    string VFSNode::getName() {
        return this->name;
    }

    const list<VFSNode*> VFSNode::getChildren() {
        return this->children;
    }
    void VFSNode::addChild(VFSNode* child) {
        this->children.append(child);
    }

    VFSNode* root;
    VFSNode* initialize() {
        root = new VFSNode("root");
        return root;
    }

    VFSNode* getRoot() {
        return root;
    }

    void mount_modules(VFSNode* parent, multiboot::multiboot_info_t* mbinfo) {
        auto modules = multiboot::get_virt_modules(mbinfo);
        for(unsigned int i = 0; i < modules.size();i++) {
            auto module = modules[i];
            string name = string(module.string);
            VFSNode* moduleNode = new VFSNode(name);
            parent->addChild(moduleNode);
        }
    }
};