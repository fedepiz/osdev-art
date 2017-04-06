#include <kernel/multiboot.h>
#include <kernel/arch.h>
#include <kernel/frame_alloc.h>
#include <stddef.h>
#include <stdint.h>
#include <util/text.h>
#include <util/vector.h>

namespace multiboot {
    using util::logf;
    using util::vector;
    module_t* first_module(multiboot_info_t* mbinfo) {
        //Phyisical address of first module
        uint32_t mod_address = mbinfo->mods_addr + arch::KERNEL_VIRTUAL_BASE;
        //Get the module out
        module_t* module = (module_t*)mod_address;
        return module;
    }

    virt_module_t to_virt_module(module_t* module) {
        virt_module_t virt;
        virt.mod_start = (void*)(module->mod_start + arch::KERNEL_VIRTUAL_BASE);
        virt.mod_end = (void*)(module->mod_end + arch::KERNEL_VIRTUAL_BASE);
        virt.string = (const char*)(module->string + arch::KERNEL_VIRTUAL_BASE);
        virt.reserved = module->reserved;
        return virt;
    }

    vector<virt_module_t> get_virt_modules(multiboot_info_t* mbinfo) {
        vector<virt_module_t> vec;
        module_t* module = first_module(mbinfo);
        vec.push_back(to_virt_module(module));
        //For now, only load the first module
        return vec;
    }

    void debug(multiboot_info_t* mbinfo) {
        util::logf("Number of kernel modules to load %d\n", mbinfo->mods_count);
        module_t* module = first_module(mbinfo);
        char* mod_cmd_line = (char*)(module->string + arch::KERNEL_VIRTUAL_BASE);
        size_t mod_length = module->mod_end - module->mod_start;
        util::logf("mod start address = %x, mod end address = %x size = %d cmd = %s\n", module->mod_start, module->mod_end, mod_length, mod_cmd_line);
    }

    void* load_module(multiboot_info_t* mbinfo, size_t *length) {
        module_t* module = first_module(mbinfo);
        size_t mod_length = module->mod_end - module->mod_start;
        if(length != nullptr) {
            *length = mod_length;
        }
        return (void*)(module->mod_start + arch::KERNEL_VIRTUAL_BASE);
    }

    void reserve_module_frames(module_t* module) {
        uint32_t mod_phys_addr = module->mod_start;
        size_t mod_length = module->mod_end - module->mod_start;
        int num_frames = frame_alloc::frames_in_range(mod_length);
        int mod_base_frame = frame_alloc::address_to_frame_index((void*)mod_phys_addr);
        for(int i = 0; i < num_frames; i++) {
            frame_alloc::reserve(mod_base_frame+i);
        }
    }

    //Reserves the frames where kernel modules have been loaded
    void reserve_modules_frames(multiboot_info_t* mbinfo) {
        module_t* module = first_module(mbinfo);
        //For each module...
        for(unsigned int i = 0; i < mbinfo->mods_count; i++) {
            reserve_module_frames(module);
        }
    }
};