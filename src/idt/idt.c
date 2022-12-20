#include "idt.h"
#include "config.h"
#include "memory/memory.h"

struct idt_desc idt_descriptors[PERSONALOS_TOTAL_INTERRUPTS];

struct idtr_desc idtr_descriptor;

void idt_init(){
    memset(idt_descriptor, 0, sizeof(struct idt_desc));
}
