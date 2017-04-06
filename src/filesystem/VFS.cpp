#include <filesystem/VFS.h>
#include <kstdlib.h>
#include <util/text.h>
namespace vfs {
    using util::panicf;
    VFSNode::VFSNode(const char* name, VFSNodeType type) {
        //Copy the name
        size_t name_len = kstd::strlen(name)+1;
        this->name = new char[name_len];
        kstd::memcpy(this->name, name, name_len);
        //The node type
        this->type = type;
    }
    const char* VFSNode::getName() const {
        return this->name;
    }
    VFSNodeType VFSNode::getType() const {
        return this->type;
    }
    
    const util::vector<VFSNode*>& VFSNode::getChildren() const {
        return this->children;
    } 

    VirtualNode::VirtualNode(const char* name, VFSNodeType type):VFSNode(name, type) {

    }

    void VirtualNode::write(void* ptr, size_t size, uint64_t offset) {
        (void)ptr;
        (void)size;
        (void)offset;
        panicf("Cannot write to virtual file node %s", this->getName());
    }
    void VirtualNode::read(void* ptr,size_t size, uint64_t offset) {
        (void)ptr;
        (void)size;
        (void)offset;
        panicf("Cannot read from virtual file node %s", this->getName());
    }
    bool VirtualNode::isVirtual() {
        return true;
    }

    RAMNode::RAMNode(const char* name, VFSNodeType type, void* buffer, size_t size):VFSNode(name, type) {
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
};