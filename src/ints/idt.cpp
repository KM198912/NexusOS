#include <ints/idt.hpp>
#include <core/main.hpp>
#include <ints/gdt.hpp>
#include <lib/string.hpp>
#define LOW_16(x) (uint16_t)(x & 0xFFFF)
#define HIGH_16(x) (uint16_t)(x >> 16)

__cdecl void isr_0();
__cdecl void isr_1();
__cdecl void isr_2();
__cdecl void isr_3();
__cdecl void isr_4();
__cdecl void isr_5();
__cdecl void isr_6();
__cdecl void isr_7();
__cdecl void isr_8();
__cdecl void isr_9();
__cdecl void isr_10();
__cdecl void isr_11();
__cdecl void isr_12();
__cdecl void isr_13();
__cdecl void isr_14();
__cdecl void isr_15();
__cdecl void isr_16();
__cdecl void isr_17();
__cdecl void isr_18();
__cdecl void isr_19();
__cdecl void isr_20();
__cdecl void isr_21();
__cdecl void isr_22();
__cdecl void isr_23();
__cdecl void isr_24();
__cdecl void isr_25();
__cdecl void isr_26();
__cdecl void isr_27();
__cdecl void isr_28();
__cdecl void isr_29();
__cdecl void isr_30();
__cdecl void isr_31();

__cdecl void irq_0();
__cdecl void irq_1();
__cdecl void irq_2();
__cdecl void irq_3();
__cdecl void irq_4();
__cdecl void irq_5();
__cdecl void irq_6();
__cdecl void irq_7();
__cdecl void irq_8();
__cdecl void irq_9();
__cdecl void irq_10();
__cdecl void irq_11();
__cdecl void irq_12();
__cdecl void irq_13();
__cdecl void irq_14();
__cdecl void irq_15();
__cdecl void irq_16();

using namespace system::hal::interrupts;


__cdecl idt::idt_register_t *_idtr = nullptr;
__cdecl idt::idt32_gate idt32_table[256] = {};
__cdecl idt::callback_t callbacks_table[256] = {};

__cdecl void __idt_flush();
__cdecl void __reload_regs();

const idt::idt32_gate & idt::manager_t::set_gate(uint8_t n, uintptr_t handler, uint8_t access_ring, idt32_gate_t gate_type, uint16_t segment)
{
    idt32_table[n].offset_low = LOW_16(handler);
	idt32_table[n].offset_high = HIGH_16(handler);

	idt32_table[n].UNUSED = 0;

	idt32_table[n].DPL = access_ring;
	idt32_table[n].type = gate_type;
	idt32_table[n].present = true;

	idt32_table[n].segment = segment;
	return idt32_table[n];
}
bool idt::manager_t::init()
{

    idt::idt_register_t idtr = {};
    _idtr = &idtr;

	// install isrs
	set_gate(0,  (uintptr_t)isr_0,  3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(1,  (uintptr_t)isr_1,  3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(2,  (uintptr_t)isr_2,  3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(3,  (uintptr_t)isr_3,  3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(4,  (uintptr_t)isr_4,  3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(5,  (uintptr_t)isr_5,  3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(6,  (uintptr_t)isr_6,  3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(7,  (uintptr_t)isr_7,  3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(8,  (uintptr_t)isr_8,  3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(9,  (uintptr_t)isr_9,  3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(10, (uintptr_t)isr_10, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(11, (uintptr_t)isr_11, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(12, (uintptr_t)isr_12, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(13, (uintptr_t)isr_13, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(14, (uintptr_t)isr_14, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(15, (uintptr_t)isr_15, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(16, (uintptr_t)isr_16, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(17, (uintptr_t)isr_17, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(18, (uintptr_t)isr_18, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(19, (uintptr_t)isr_19, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(20, (uintptr_t)isr_20, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(21, (uintptr_t)isr_21, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(22, (uintptr_t)isr_22, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(23, (uintptr_t)isr_23, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(24, (uintptr_t)isr_24, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(25, (uintptr_t)isr_25, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(26, (uintptr_t)isr_26, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(27, (uintptr_t)isr_27, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(28, (uintptr_t)isr_28, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(29, (uintptr_t)isr_29, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(30, (uintptr_t)isr_30, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(31, (uintptr_t)isr_31, 3, idt32_gate_t::TRAP_32, gdt::manager_t::get_kernel_code_seg());
	
	// remap PIC
	outb(PIC_MASTER_CMD, 0x10 + 0x01);
	outb(PIC_SLAVE_CMD,  0x10 + 0x01);
	/* Setup master's vector offset */
	outb(PIC_MASTER_DATA, 0x20);
	/* Tell the slave its vector offset */
	outb(PIC_SLAVE_DATA, 0x28);
	/* Tell the master that he has a slave */
	outb(PIC_MASTER_DATA, 4);
	outb(PIC_SLAVE_DATA, 2);
	/* Enabled 8086 mode */
	outb(PIC_MASTER_DATA, 0x01);
	outb(PIC_SLAVE_DATA, 0x01);

	outb(PIC_MASTER_DATA, 0);
	outb(PIC_SLAVE_DATA, 0);

	// install irqs
	set_gate(32, (uintptr_t)irq_0, 0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(33, (uintptr_t)irq_1, 0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(34, (uintptr_t)irq_2, 0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(35, (uintptr_t)irq_3, 0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(36, (uintptr_t)irq_4, 0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(37, (uintptr_t)irq_5, 0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(38, (uintptr_t)irq_6, 0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(39, (uintptr_t)irq_7, 0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(40, (uintptr_t)irq_8, 0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(41, (uintptr_t)irq_9, 0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(42, (uintptr_t)irq_10,0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(43, (uintptr_t)irq_11,0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(44, (uintptr_t)irq_12,0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(45, (uintptr_t)irq_13,0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(46, (uintptr_t)irq_14,0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());
	set_gate(47, (uintptr_t)irq_15,0,idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());

	// install syscall
	set_gate(0x80, (uintptr_t)irq_16, 3, idt32_gate_t::INTERRUPT_32, gdt::manager_t::get_kernel_code_seg());

	idtr.base = (uintptr_t) &idt32_table;
    idtr.limit = (256 * sizeof(idt32_gate));
	__reload_regs();
	__idt_flush();

	// install syscall handler
	//register_int_handler(16, [](regs32_t *r){
	//	return syscall(r);
	//});

	return true;
}

const char* exceptions[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

__cdecl void fault_handler(idt::regs32_t regs)
{
	asm ("cli");

	gdt::__tss.esp0 = (uintptr_t)regs.esp;
	if(!System::Kernel::term.IsInitialized())
	{
		System::Kernel::term.Initialize();
	}
	//Center the text
	System::Kernel::term.SetColors(Color::White, Color::DarkRed);
	System::Kernel::term.Write(Color::White, Color::DarkRed,"NexusOS Ran into a Kernel Panic and has been halted!\n");
	System::Kernel::term.Write(Color::White, Color::DarkRed,"Please report this error to the NexusOS team.\n");
	System::Kernel::term.Printf(Color::White, Color::DarkRed,"Fatal: %s\n",exceptions[regs.identifier.trap_number]);
	System::Kernel::term.Printf(Color::White, Color::DarkRed,"EIP: 0x%x ESP: 0x%x EBP: 0x%x EAX: 0x%x EBX: 0x%x ECX: 0x%x EDX: 0x%x\n",regs.eip,regs.esp,regs.ebp,regs.eax,regs.ebx,regs.ecx,regs.edx);
	System::Kernel::term.Render();
    asm volatile("cli");
    asm volatile("hlt");
    for(;;);
}
__cdecl void interrupt_handler(idt::regs32_t regs)
{
	gdt::__tss.esp0 = (uintptr_t)regs.esp;

	if (callbacks_table[regs.identifier.interrupt_number])
	{
		callbacks_table[regs.identifier.interrupt_number](&regs);
	}

	// send eoi
	if (regs.identifier.interrupt_number >= 8)
	{
		outb(PIC_SLAVE_CMD, 0x20);
	}
	outb(PIC_MASTER_CMD, 0x20);
	
	gdt::gdt32_table[TSS_INDEX].read_write = 0;
}

void idt::manager_t::enable_interrupts()
{
	printf("enable_interrupts() >> interrupts enabled\n");
	asm volatile("sti");
}
void idt::manager_t::disable_interrupts()
{
	System::Kernel::serial.Printf("disable_interrupts() >> interrupts disabled\n");
	asm volatile("cli");
}
bool idt::manager_t::state()
{
	uint16_t eflags;
	asm volatile("pushf; pop %0" : "=r"(eflags));
	return eflags & 0x200;
}

void idt::manager_t::register_irq(uint8_t irq, callback_t callback)
{
	// check if interrupts are enabled
	int eflags;
	asm("pushf\npop %0" : "=g"(eflags));

	if (eflags & 0x200) asm("cli");
	callbacks_table[irq] = callback;
	if (eflags & 0x200) asm("sti");
}