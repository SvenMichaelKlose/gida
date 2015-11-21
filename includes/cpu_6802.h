// Motorola 6802 CPU support

#ifndef _CPU_6802_H
#define  _CPU_6802_H

#include "vcpu.h"

// Addressing modes
//
// implied      No operands.
// extended     16-bit absolute address
// indexed      8-bit address + index register
// direct       8-bit address
// immediate8   8-bit constant
// immediate16  16-bit constant
// relative     8-bit relative address
enum cpu_6802_adrmode { implied, extended, indexed, direct, immediate8,
                        immediate16, relative };

// Addressing mode groups.
//
// g_implied     implied
// g_extended    extended, indexed
// g_direct      extended, indexed, direct
// g_immediate8  extended, indexed, direct, immediate8
// g_immediate16 extended, indexed, direct, immediate16
// g_relative    relative
enum cpu_6802_adrgrp { g_implied, g_extended, g_direct, g_immediate8,
                       g_immediate16, g_relative, g_mask = 0xffff,
                       g_tmask = 0xf0000, g_byte = 0x10000, g_word = 0x20000 };

// Instruction descriptor.
typedef struct _cpu_6802_inst {
    const char* mnem;          // Mnemonic
    const char opcode;         // Base opcode.
    const int modes; // Addressing mode group.
} cpu_6802_inst;  

class cpu_6802 : public vcpu {

   public:
      static cpu_6802* instance ();

      const char* name ();
      const char* vendor () { return "Motorola"; }
      const char* classtype ()
      {
         return "Micro processor (8 bit/64k address space)";
      }

      void get_vop (vop& v, vsegment* seg, unsigned long& pc);
      void analyze (vop& v, vsegment* seg, unsigned long& pc);
      void disassemble (vsegment* seg, unsigned long& pc);

   private:
      cpu_6802 ();
      static cpu_6802* _instance;

      void create_opcode (const cpu_6802_inst*);
      void disassemble_operand (vsegment* seg, unsigned long& pc,
                                cpu_6802_adrmode mode);
};

#endif // _CPU_6802_H
