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

    VFSNode::VFSNode(string name, DataOps* ops) {
        this->name = name;
        this->dataOps = ops;
    }

    VFSNode::VFSNode(const char* name, DataOps* ops) {
        this->name = string(name);
        this->dataOps = ops;
    }

    VFSNode::~VFSNode() {
        if(this->dataOps != nullptr) {
            delete this->dataOps;
        }
        while(this->children.length() > 0) {
            VFSNode* child = children.behead();
            delete child;
        }
    }

    string VFSNode::getName() const {
        return this->name;
    }

    const list<VFSNode*>* VFSNode::getChildren() const {
        return &this->children;
    }

    const DataOps* VFSNode::getDataOps() const {
        return this->dataOps;
    }

    void VFSNode::addChild(VFSNode* child) {
        this->children.append(child);
    }

    VFSNode* root;
    VFSNode* initialize() {
        root = new VFSNode("", nullptr);
        return root;
    }

    VFSNode* getRoot() {
        return root;
    }

    static VFSNode* getNode(const vector<string>& path, unsigned int pathCount, VFSNode* node) {
        //The idea:
        //Follow the path by finding the right child, or stop if we are done
        
        //First sanity check: we have not gone too far
        if(pathCount >= path.size()) {
            logf("Pathcount too high in vfs::getNode\n");
            return nullptr;
        }
        //Check if we are done
        if(pathCount == path.size() - 1) {
            return node;
        }
        //Else, we have to look into the nodes' children
        auto children = node->getChildren();
        for(unsigned int i = 0; i < children->length(); i++) {
            VFSNode* child = children->get(i);
            if(child->getName() == path.get(pathCount+1)) {
                //Found the child! recurse into it!
                return getNode(path, pathCount+1, child);
            }
        }
        //Nothing found
        return nullptr;
    }

    VFSNode* getNode(const kstd::string& path) {
        auto chunks = path.split('/');
        if(chunks.size() == 0) {
            return nullptr;
        }
        if(chunks[0].size() != 0) {
            logf("First file is not root - local paths not yet supported\n");
            return nullptr;
        }
        return getNode(chunks, 0, getRoot());
    }

    VFSNode* getNode(const char* path) {
        return getNode(string(path));
    }

    void mount_modules(VFSNode* parent, multiboot::multiboot_info_t* mbinfo) {
        auto modules = multiboot::get_virt_modules(mbinfo);
        for(unsigned int i = 0; i < modules.size();i++) {
            auto module = modules[i];
            size_t module_size = ((uint32_t)module.mod_end) - ((uint32_t)module.mod_start);
            RamDataOps* ops = new RamDataOps((uint8_t*)module.mod_start, module_size);
            auto nameBits = string(module.string).split('/');
            string name = nameBits[nameBits.size()-1];
            VFSNode* moduleNode = new VFSNode(name, ops);
            parent->addChild(moduleNode);
        }
    }
};