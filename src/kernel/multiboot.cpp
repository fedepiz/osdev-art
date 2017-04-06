#include <kernel/multiboot.h>
#include <kernel/arch.h>
#include <stddef.h>
#include <stdint.h>
#include <util/text.h>
namespace multiboot {
    using util::logf;
    module_t* first_module(multiboot_info_t* mbinfo) {
        //Phyisical address of first module
        uint32_t mod_address = mbinfo->mods_addr + arch::KERNEL_VIRTUAL_BASE;
        //Get the module out
        module_t* module = (module_t*)mod_address;
        return module;
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
};