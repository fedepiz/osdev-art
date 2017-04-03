#ifndef KERNEL_ARCH_H
#define KERNEL_ARCH_H

#include <stdint.h>

    /** outb:
     *  Sends the given data to the given I/O port. Defined in io.s
     *
     *  @param port The I/O port to send the data to
     *  @param data The data to send to the I/O port
     */
extern "C" void outb(uint16_t port, uint8_t data);
extern "C" char inb(uint16_t port);

/* Defines a GDT entry. We say packed, because it prevents the
*  compiler from doing things that it thinks is best: Prevent
*  compiler "optimization" by packing */
struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

/* Special pointer which includes the limit: The max bytes
*  taken up by the GDT, minus 1. Again, this NEEDS to be packed */
struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Our GDT, with 3 entries, and finally our special GDT pointer */
extern struct gdt_entry gdt[3];
extern struct gdt_ptr gp;

/* This will be a function in start.asm. We use this to properly
*  reload the new segment registers */
extern "C" void gdt_flush();


/* Defines an IDT entry */
struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;        /* Our kernel segment goes here! */
    unsigned char always0;     /* This will ALWAYS be set to 0! */
    unsigned char flags;       /* Set using the above table! */
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries in this tutorial, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */
extern struct idt_entry idt[256];
extern struct idt_ptr idtp;

/* This exists in 'start.asm', and is used to load our IDT */
extern "C" void idt_load();
//Initializes the basic architecture pieces such as the gdt and
//idt
void arch_initialize();
#endif
