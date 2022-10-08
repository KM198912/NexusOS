#pragma once

#define SYSCALL_IRQ 0x80

#include <stddef.h>
#include <stdint.h>
//#include <std/array.h>
#include <std/decl.h>
#include <lib/ports.hpp>
#define PIC_MASTER_CMD 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD 0xA0
#define PIC_SLAVE_DATA 0xA1
namespace system::hal::interrupts::idt
{
    struct regs32_t
    {
        uint32_t ds;
        uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;

        union {
            uint32_t		 interrupt_number;
            uint32_t		 trap_number;
        } identifier;
    
        uint32_t eip, cs, eflags, useresp, ss;
    };

    typedef void (*callback_t)(regs32_t*);

    enum class idt32_gate_t : uint8_t
    {
        TASK = 0x5,

        INTERRUPT_16 = 0x6,
        TRAP_16 = 0x7,

        INTERRUPT_32 = 0xE,
        TRAP_32 = 0xF
    } packed__;
    enum class idt32_DPL_t : uint8_t
    {
        KERNEL = 0,
        RING1 = 1,
        RING2 = 2,
        USER = 3
    };

    struct idt32_gate_attribute
    {
        idt32_gate_t type 		: 4;
        uint8_t      unused 	: 1;
        uint8_t 	 DPL		: 2;
        uint8_t		 present	: 1;
    } packed__;

    struct idt32_gate
    {
        uint16_t 			 offset_low; 	// gate's offset low 16bit
        uint16_t 			 segment; 		// segment selector from GDT (code, data, system)
        
        uint8_t 			 UNUSED;		// unused byte, should always be 0

        idt32_gate_t type 		: 4;
        uint8_t      unused 	: 1;
        uint8_t 	 DPL		: 2;
        bool		 present	: 1;

        uint16_t			 offset_high;	// gate's offset high 16bit;
    } packed__;

    struct idt_register_t{
        uint16_t limit;
        uint32_t base;
    } packed__;


  /*  extern system::hal::drivers::ports::port8_t PIC_master;
    extern system::hal::drivers::ports::port8_t PIC_master_ctrl;

    extern system::hal::drivers::ports::port8_t PIC_second;
    extern system::hal::drivers::ports::port8_t PIC_second_ctrl;*/

    class manager_t final
    {
    public:
        static const idt32_gate & set_gate(uint8_t n, uintptr_t handler, uint8_t access_ring = 3, idt32_gate_t gate_type = idt32_gate_t::INTERRUPT_32, uint16_t segment = 0x08);
    public:
        static bool init();
    public:
        static void enable_interrupts();
        static void disable_interrupts();
        static bool state();
    public:
        static void register_irq(uint8_t irq, callback_t callback);
    };
}