// Copyright (c) 2002,2015 Sven Michael Klose <pixel@hugbox.org>

#ifndef _CPU_6800_H
#define  _CPU_6800_H

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
enum cpu_6800_adrmode { implied, extended, indexed, direct, immediate8,
                        immediate16, relative };

// Addressing mode groups.
//
// g_implied     implied
// g_extended    extended, indexed
// g_direct      extended, indexed, direct
// g_immediate8  extended, indexed, direct, immediate8
// g_immediate16 extended, indexed, direct, immediate16
// g_relative    relative
enum cpu_6800_adrgrp { g_implied, g_extended, g_direct, g_immediate8,
                       g_immediate16, g_relative, g_mask = 0xffff,
                       g_tmask = 0xf0000, g_byte = 0x10000, g_word = 0x20000 };

// Instruction descriptor.
typedef struct _cpu_6800_inst {
    const char* mnem;          // Mnemonic
    const char opcode;         // Base opcode.
    const int modes; // Addressing mode group.
} cpu_6800_inst;  

class cpu_6800 : public vcpu {

   public:
      static cpu_6800* instance ();

      const char* name () { return "6800 family"; }
      const char* vendor () { return "MOS Technology"; }
      const char* classtype ()
      {
         return "Micro processor (8 bit/64k address space)";
      }

      void get_vop (vop& v, vsegment* seg, unsigned long& pc);
      void analyze (vop& v, vsegment* seg, unsigned long& pc);
      void disassemble (vsegment* seg, unsigned long& pc);

   private:
      cpu_6800 ();
      static cpu_6800* _instance;

      void create_opcode (const cpu_6800_inst*);
      void disassemble_operand (vsegment* seg, unsigned long& pc,
                                cpu_6800_adrmode mode);
};

#endif // _CPU_6800_H
