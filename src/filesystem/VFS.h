#ifndef VFS_H
#define VFS_H
#include <util/vector.h>
#include <stddef.h>
#include <stdint.h>
namespace vfs {
    enum VFSNodeType {
        file,
        directory
    };
    
    class VFSNode {
        protected:
        char* name;
        VFSNodeType type;
        util::vector<VFSNode*> children;
        public:
        VFSNode(const char* name, VFSNodeType type);
        const char* getName() const;
        VFSNodeType getType() const;
        const util::vector<VFSNode*>& getChildren() const;
        //Read and write to file
        virtual void write(void* ptr, size_t size, uint64_t offset) = 0;
        virtual void read(void* ptr, size_t size, uint64_t offset) = 0;
        virtual bool isVirtual() = 0;
    };

    class VirtualNode:public VFSNode {
        public:
        VirtualNode(const char* name, VFSNodeType type);
        virtual void write(void* ptr, size_t size, uint64_t offset);
        virtual void read(void* ptr, size_t size, uint64_t offset);
        virtual bool isVirtual();
    };

    class RAMNode: public VFSNode {
        private:
        uint8_t* buffer;
        size_t size;
        public:
        RAMNode(const char* name, VFSNodeType type, void* buffer, size_t size);
        virtual void write(void* ptr, size_t size, uint64_t offset);
        virtual void read(void* ptr, size_t size, uint64_t offset);
        virtual bool isVirtual();
    };

    extern VFSNode* root;
    void initialize();
};
#endif
