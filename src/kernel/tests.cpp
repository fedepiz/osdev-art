void testPageAllocator() {
	paging::page_allocator* alloc = &paging::kernel_page_allocator;
	alloc->debug(true);
	uint8_t* ptr = (uint8_t*)paging::page_index_to_address(alloc->allocate());
	vga_term::puts("Base address of page ");
	vga_term::puts(itoa((uint32_t)ptr,16).str);
	vga_term::puts("\n");
	int* x = (int*)ptr;
	int* y = (int*)ptr;
	*x = 5;
	vga_term::puts(itoa(*x).str);
	vga_term::puts("\n");
	vga_term::puts(itoa(*y).str);
	vga_term::puts("\n");
}

void testDynamicHeap() {
	util::DynamicHeap heap;
	util::DynamicHeap_initialize(&heap, &paging::kernel_page_allocator);

	int* ptr[4];
	for(int i = 0; i < 4; i++) {
		ptr[i] = (int*)heap.malloc(63);
	}
	heap.free(ptr[2]);
	heap.free(ptr[1]);
	paging::kernel_page_allocator.debug(true);
	heap.debug();
}