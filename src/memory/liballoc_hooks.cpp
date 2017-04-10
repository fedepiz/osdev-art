#include <memory/paging.h>
#include <util/text.h>

using util::logf;


extern "C" int liballoc_lock() {
    __asm__ ("cli");
    return 0;
}

/** This function unlocks what was previously locked by the liballoc_lock
 * function.  If it disabled interrupts, it enables interrupts. If it
 * had acquiried a spinlock, it releases the spinlock. etc.
 *
 * \return 0 if the lock was successfully released.
 */
extern "C" int liballoc_unlock() {
    __asm__ ("sti");
    return 0;
}

/** This is the hook into the local system which allocates pages. It
 * accepts an integer parameter which is the number of pages
 * required.  The page size was set up in the liballoc_init function.
 *
 * \return NULL if the pages were not allocated.
 * \return A pointer to the allocated memory.
 */



 extern "C" void* liballoc_alloc(int num_pages) {
    //logf("Liballoc requesting %d pages\n", num_pages);
    int basePage = -1;
    for(int i = 0; i < num_pages;i++) {
        int index = memory::kernel_page_allocator.allocate();
        if(i == 0) {
            basePage = index;
        }
    }
    //memory::kernel_page_allocator.debug(true);
    void* ptr = memory::page_index_to_address(basePage);
    //logf("Allocated pages starting from %x\n",ptr);
    return ptr;
 }

/** This frees previously allocated memory. The void* parameter passed
 * to the function is the exact same value returned from a previous
 * liballoc_alloc call.
 *
 * The integer value is the number of pages to free.
 *
 * \return 0 if the memory was successfully freed.
 */
extern "C" int liballoc_free(void* ptr,int num_pages) {
    //logf("Liballoc wishes to free %d pages, starting at %x\n", num_pages, ptr);
    int basePage = memory::address_to_page_index(ptr);
    for(int i = 0; i < num_pages;i++) {
        memory::kernel_page_allocator.free(basePage+i);
    }
    //memory::kernel_page_allocator.debug(true);
    return 0;
}
