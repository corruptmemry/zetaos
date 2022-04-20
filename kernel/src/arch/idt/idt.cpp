#include <zetaos/zetalib.h>
#include <zetaos/idt/idt.h>
#include <zetaos/io/IO.h>
#include <zetaos/userinput/kbScancodeTranslation.h>
#include <zetaos/video/tty.h>
#include <zetaos/sched/pit.h>
#include <zetaos/userinput/keyboard.h>
#include <zetaos/mm/heap.h>
#include <zetaos/rtc/rtc.h>

__attribute__((interrupt)) void PageFault_Handler(struct interrupt_frame* frame){
    Print(0xFFFFFF, "\n\nPage Fault detected, system halting");
    while(1);
}

__attribute__((interrupt)) void DoubleFault_Handler(struct interrupt_frame* frame){
    Print(0xFFFFFF, "\n\nDouble Fault Detected, system halting");
    while(1);
}

__attribute__((interrupt)) void GPFault_Handler(struct interrupt_frame* frame){
    Print(0xFFFFFF, "\n\nGeneral Protection Fault Detected, system halting");
    while(1);
}

void PIC_EndMaster(){
    outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave(){
    outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

__attribute__((interrupt)) void KeyboardInt_Handler(interrupt_frame* frame){
    Print(0xAAAFFF, "1");
    PIC_EndMaster();
    Print(0xFFFFFF, "\n           @&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%&@/");
    Print(0xFFFFFF, "\n***********(@&&&&&&&&&&&&&&&&&&&&&&&%&&&&&&&#%");
    Print(0xFFFFFF, "\n************#&&&&&&&&&&&&&&&&&&&&%%#(((((((((%&/");
    Print(0xFFFFFF, "\n************/&(%&&&&%###((##&&&&%&&&&&&&&&&&&&&&&&&&&&@@");
    Print(0xFFFFFF, "\n*************(%(%@@@@@@&&&&&&&&@@%#(((*,.,,,,,...,,,%&&@%");
    Print(0xFFFFFF, "\n***********(@&@&&&&&&&@@/,........,,,,....,,,.......,*@@/*/");
    Print(0xFFFFFF, "\n***********(@&@&&&&&&&@@/,........,,,,....,,,.......,*@@/*/");
    Print(0xFFFFFF, "\n*******#@@&&&&&@(,,.....,.........,,,*,....,*,,.....,/*/");
    Print(0xFFFFFF, "\n****/%&&&@%/...........,,........,,,*,***,,,***,,,..,,#/");
    Print(0xFFFFFF, "\n#***#@&&@*,..........,,,,,.....,*,,(,,,*#*#(****/,,,,,");
    Print(0xFFFFFF, "\n/****%@@&*,.....,,,,,,((/*/,...,/*(,,,,,,,,*,,/,,,,,,,*/#");
    Print(0xFFFFFF, "\n********(,,,,,,,*///**,,,(**,,,,&,,,**/(**,,**(,,.,,,,,/(,////");
    Print(0xFFFFFF, "\n*******/(*,,,,,,,*//*******/#*,,,(******(%@@@@(,,,,,,,,*/");
    Print(0xFFFFFF, "\n********(**,,,,../,****#@@#****#/*/**.%//(  */,,,,,,,,(/");
    Print(0xFFFFFF, "\n*********/,,,,,,,,%#, *(//%.**,,,***********(/*,,*,,*(");
    Print(0xFFFFFF, "\n***********/,,,,,,,(/%/*****,,,,,,,,,,,.,.,,,,,//*/*(//");
    Print(0xFFFFFF, "\n************/*,*,*(***//(,......,,.........,**%/");
    Print(0xFFFFFF, "\n***********/,,,,,,,(/%/*****,,,,,,,,,,,.,.,,,,,//");
    Print(0xFFFFFF, "\n************/*,*,*(***//(,......,,.........,**%/");
    Print(0xFFFFFF, "\n*************//*/,**#**,...................*(*/");
    Print(0xFFFFFF, "\n********************(/**%,,....(........*&%");
    Print(0xFFFFFF, "\n*****************/%%%&&@@@&(,@&%#,*#(,*&((((////");
    Print(0xFFFFFF, "\n********////***&%&%&&&&&&%(((((#*,,,,(((((((&@&&&&&&(");
    Print(0xFFFFFF, "\n**************%%&&&&&&&&@#(((((%%#*#%#(((((%&&&&&&&&&&&");
    Print(0xFFFFFF, "\n*******///****&&&&&&&&&&@(%&((#&@##(%&((#%%/@&&&&&&&&&&&");
    Print(0xFFFFFF, "\n*************%&&&&&&&&&&&//%/%#%@(%#/%@((&*&&&&&&&&&&&&@/");
    Print(0xFFFFFF, "\n************%&&&&&&&&&&&&@*%%/@((&(##(%//&&&&&&&&@&&&&&/");
}

IDTR idtr;
void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t type_attr, uint8_t selector){

    IDTDescEntry* interrupt = (IDTDescEntry*)(idtr.Offset + entryOffset * sizeof(IDTDescEntry));
    interrupt->SetOffset((uint64_t)handler);
    interrupt->type_attr = type_attr;
    interrupt->selector = selector;
}


void init_idt(){
    idtr.Limit = 0x0FFF;
    idtr.Offset = (uint64_t)malloc(4096);

    SetIDTGate((void*)PageFault_Handler, 0xE, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)DoubleFault_Handler, 0x8, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)GPFault_Handler, 0xD, IDT_TA_InterruptGate, 0x08);
    SetIDTGate((void*)KeyboardInt_Handler, 0x21, IDT_TA_InterruptGate, 0x08);


    asm ("lidt %0" : : "m" (idtr));
    RemapPIC();

    outb(PIC1_DATA, 0b11111101);
    outb(PIC2_DATA, 0b11111111);
    asm ("sti");
}

void IDTDescEntry::SetOffset(uint64_t offset){
    offset0 = (uint16_t)(offset & 0x000000000000ffff);
    offset1 = (uint16_t)((offset & 0x00000000ffff0000) >> 16);
    offset2 = (uint32_t)((offset & 0xffffffff00000000) >> 32);
}

uint64_t IDTDescEntry::GetOffset(){ 
    uint64_t offset = 0;
    offset |= (uint64_t)offset0;
    offset |= (uint64_t)offset1 << 16;
    offset |= (uint64_t)offset2 << 32;
    return offset;
}
void RemapPIC(){
    uint8_t a1, a2; 

    a1 = inb(PIC1_DATA);
    io_wait();
    a2 = inb(PIC2_DATA);
    io_wait();

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    io_wait();
    outb(PIC2_DATA, a2);
}