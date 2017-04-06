#include <stddef.h>
#include <kstdlib.h>
#include <kstdio.h>

extern "C" void __cxa_pure_virtual() {
    panic("Pure virtual function call!");
}

void *operator new(size_t size) {
    return kstd::calloc(size);
}
 
void *operator new[](size_t size) {
    return kstd::calloc(size);
}
 
void operator delete(void *p) {
    kstd::free(p);
}
 
void operator delete[](void *p) {
    kstd::free(p);
}

