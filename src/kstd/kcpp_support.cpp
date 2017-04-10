#include <stddef.h>
#include <kstdlib.h>
#include <kstdio.h>
#include <memory/liballoc.h>
#include <util/text.h>

extern "C" void __cxa_pure_virtual() {
    panic("Pure virtual function call!");
}

void *operator new(size_t size) {
    return malloc(size);
}

 
void *operator new[](size_t size) {
    return  malloc(size);
}
 
void operator delete(void *p) {
     free(p);
}
 
void operator delete[](void *p) {
     free(p);
}

