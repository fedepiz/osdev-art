#ifndef FILESYSTEM_DATA_OPS_H
#define FILESYSTEM_DATA_OPS_H

#include <stddef.h>
#include <stdint.h>

namespace vfs {
    enum DataOpsType {
        RANDOM_ACCESS, INSTREAM, OUTSTREAM
    };

    class DataOps {
        private:
        DataOpsType type;
        public:
        DataOps(DataOpsType type);
        DataOpsType getType() const;
    };

    class RandomAccessDataOps : public DataOps {
        public:
        RandomAccessDataOps();
        virtual void read(size_t offset, uint8_t* dest, size_t count) = 0;
        virtual void write(size_t offset, uint8_t* bytes, size_t count) = 0;
        virtual size_t size() = 0;
    };

    class RamDataOps : public RandomAccessDataOps {
        private:
        uint8_t* buffer;
        size_t buffer_size;
        public:
        RamDataOps(uint8_t* buffer, size_t size);
        virtual void read(size_t offset, uint8_t* dest, size_t count);
        virtual void write(size_t offset, uint8_t* bytes, size_t count);
        virtual size_t size();
    };
};
#endif