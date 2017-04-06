#include <filesystem/VFS.h>
#include <kstdlib.h>
#include <util/text.h>
#include <kernel/multiboot.h>
namespace vfs {
    using util::logf;
    using util::panicf;
    using util::vector;
    using kstd::string;
    VFSNode::VFSNode(string name, VFSNodeType type) {
        this->name = name;
        //The node type
        this->type = type;
    }
    string VFSNode::getName() const {
        return this->name;
    }
    VFSNodeType VFSNode::getType() const {
        return this->type;
    }
    
    const util::vector<VFSNode*>& VFSNode::getChildren() const {
        return this->children;
    } 

    bool VFSNode::addChild(VFSNode* newChild) {
        //For now, simply check that child does not already exist
        for(unsigned int i = 0; i < this->children.size();i++) {
            VFSNode* child = this->children[i];
            if(string::compare(child->getName(), newChild->getName()) == 0) {
                //Child already present, abort
                return false;
            }
        }
        this->children.push_back(newChild);
        return true;
    }

    VirtualNode::VirtualNode(string name, VFSNodeType type):VFSNode(name, type) {

    }

    void VirtualNode::write(void* ptr, size_t size, uint64_t offset) {
        (void)ptr;
        (void)size;
        (void)offset;
        panicf("Cannot write to virtual file node %s", this->getName().str());
    }
    void VirtualNode::read(void* ptr,size_t size, uint64_t offset) {
        (void)ptr;
        (void)size;
        (void)offset;
        panicf("Cannot read from virtual file node %s", this->getName().str());
    }
    bool VirtualNode::isVirtual() {
        return true;
    }

    RAMNode::RAMNode(string name, VFSNodeType type, void* buffer, size_t size):VFSNode(name, type) {
        this->buffer = (uint8_t*)buffer;
        this->size = size;
    }

    void RAMNode::write(void* ptr, size_t size, uint64_t offset) {
        kstd::memcpy(this->buffer+offset, ptr, size);
    }
    void RAMNode::read(void* ptr, size_t size, uint64_t offset) {
        kstd::memcpy(ptr, this->buffer+offset, size);
   }
    bool RAMNode::isVirtual() {
        return false;
    }

    VFSNode* root;

    void initialize() {
        root = new VirtualNode("root", VFSNodeType::directory);
    }

    void mount_multiboot_modules(multiboot::multiboot_info_t* mbinfo){
        //For now, make a file per module, and add under a specific multiboot file
        //First, make the parent file
        VirtualNode* modulesDir = new VirtualNode("/modules", VFSNodeType::directory);
        if(!root->addChild(modulesDir)) {
            panic("Could not mount child /modules onto virtual root");
        }
        //Add modules to root
        vector<multiboot::virt_module_t> modules = multiboot::get_virt_modules(mbinfo);
        for(unsigned int i = 0; i < modules.size();i++) {
            auto module = modules[i];
            string name(module.string);
            size_t moduleSize = ((uint32_t)module.mod_end) - ((uint32_t)module.mod_start);
            RAMNode* moduleNode = new RAMNode(name, VFSNodeType::file, module.mod_start, moduleSize);
            modulesDir->addChild(moduleNode);
            logf("Added module %s\n", moduleNode->getName().str());
        }
    }
};