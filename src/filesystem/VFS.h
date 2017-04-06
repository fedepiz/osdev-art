#ifndef VFS_H
#define VFS_H
#include <util/vector.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <kernel/multiboot.h>
namespace vfs {
    enum VFSNodeType {
        file,
        directory
    };
    
    class VFSNode {
        protected:
        kstd::string name;
        VFSNodeType type;
        util::vector<VFSNode*> children;
        public:
        VFSNode(kstd::string name, VFSNodeType type);
        kstd::string getName() const;
        VFSNodeType getType() const;
        const util::vector<VFSNode*>& getChildren() const;
        //Read and write to file
        virtual void write(void* ptr, size_t size, uint64_t offset) = 0;
        virtual void read(void* ptr, size_t size, uint64_t offset) = 0;
        virtual bool isVirtual() = 0;
        bool addChild(VFSNode* child);
    };

    class VirtualNode:public VFSNode {
        public:
        VirtualNode(kstd::string name, VFSNodeType type);
        virtual void write(void* ptr, size_t size, uint64_t offset);
        virtual void read(void* ptr, size_t size, uint64_t offset);
        virtual bool isVirtual();
    };

    class RAMNode: public VFSNode {
        private:
        uint8_t* buffer;
        size_t size;
        public:
        RAMNode(kstd::string name, VFSNodeType type, void* buffer, size_t size);
        virtual void write(void* ptr, size_t size, uint64_t offset);
        virtual void read(void* ptr, size_t size, uint64_t offset);
        virtual bool isVirtual();
    };

    extern VFSNode* root;
    void initialize();
    void mount_multiboot_modules(multiboot::multiboot_info_t* mbinfo);
};
#endif
