#include <filesystem/DataOps.h>
#include <kstdlib.h>

namespace vfs {
    DataOps::DataOps(DataOpsType type) {
        this->type = type;
    }

    DataOpsType DataOps::getType() const {
        return this->type;
    }

    RandomAccessDataOps::RandomAccessDataOps() : DataOps(DataOpsType::RANDOM_ACCESS) {
        
    }

    RamDataOps::RamDataOps(uint8_t* buffer, size_t size) {
        this->buffer = buffer;
        this->buffer_size = size;
    }
            
    void RamDataOps::read(size_t offset, uint8_t* buffer, size_t count) {
        kstd::memcpy(buffer, this->buffer + offset, count);   
    }
    
    void RamDataOps::write(size_t offset, uint8_t* bytes, size_t count) {
        kstd::memcpy(this->buffer + offset, bytes, count);
    }

    size_t RamDataOps::size() {
        return this->buffer_size;
    }
};