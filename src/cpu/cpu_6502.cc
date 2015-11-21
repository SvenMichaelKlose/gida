// Copyright (c) 2002,2015 Sven Klose <sven@hugbox.org>

using namespace std;

#include <fstream>

#include "vop.h"
#include "vcpu.h"
#include "vdump.h"
#include "vchunk.h"
#include "vcode.h"
#include "vdata.h"
#include "vsegment.h"
#include "vsubroutine.h"
#include "cpu_6502.h"

// The one and only instance of cpu_6502.
cpu_6502* cpu_6502::_instance;

void cpu_6502::disassemble_operand (vsegment* seg, vaddr& pc, cpu_6502_adrmode mode)
{
    vimage* img = seg->image ();
    unsigned char l;
    unsigned int operand = 0; // Damn. Need to initialize this because gcc will
                              // not clear the high word when converting from
                              // unsigned char if optimization is > 1.
    vchunk* ch;
    vaddr tmp;

    if (mode == implied)
        return;

    cout << " ";
    switch (mode) {
        case direct:
            operand = img->get (pc);
            ch = seg->find (operand);
            if (ch)
               ch->dumper ()->label (ch);
            else {
               cout << "$";
               vdump::dump_hexbyte (operand);
            }
            break;

        case indexed:
            operand = img->get (pc);
            ch = seg->find (operand);
            if (ch)
               ch->dumper ()->label (ch);
            else {
               cout << "$";
               vdump::dump_hexbyte (operand);
            }
            cout << ",X";
            break;

        case immediate8:
            operand = img->get (pc);
            cout << "#$";
            vdump::dump_hexbyte (operand);
            break;

        case immediate16:
            operand = img->get_word (pc);
            ch = seg->find (operand);
            cout << "#";
            if (ch) {
               ch->dumper ()->label (ch);
               tmp = ch->start ();
               if (ch->is_asciiz ()) {
                  cout << " ; '";
                  char c;
                  while ((c = img->get (tmp)))
                     cout << c;
                  cout << "'";
               }
            } else {
               cout << "$";
               vdump::dump_hexword (operand);
            }
            break;

        case extended:
            operand = img->get_word (pc);
            ch = seg->find (operand);
            if (ch)
               ch->dumper ()->label (ch);
            else {
               cout << "$";
               vdump::dump_hexword (operand);
            }
            break;

        case relative:
            l = img->get (pc);
            operand = pc + l;
            ch = seg->find (operand);
            if (ch)
               ch->dumper ()->label (ch);
            else {
               cout << "$";
               vdump::dump_hexword (operand);
            }
            break;
    }
}

// Disassemble to plain text line.
void cpu_6502::disassemble (vsegment* seg, vaddr& pc)
{
    vimage* img = seg->image ();
    unsigned char c = img->get (pc);

    cpu_6502_opcode* opcode = &opcodemap[c];

    // Print byte constant if opcode is invalid.
    if (opcode->mnem == 0) {
        cout << "FCB $";
        vdump::dump_hexbyte (c);
        cout << "\n";
        return;
    }

    cout << "        " << opcode->mnem;

    if (opcode->mode != implied)
        disassemble_operand (seg, pc, opcode->mode);

    cout << "\n";
}

// Get vop of instruction.
//
// v   Instruction descriptor.
// seg Program segment to use.
// pc  Program counter
void cpu_6502::get_vop (vop& v, vsegment* seg, vaddr& pc)
{
   vimage* img = seg->image ();
   unsigned char c;

   c = img->get (pc);
   cpu_6502_opcode* opcode = &opcodemap[c];

   if (opcode->mnem == 0)
      return;
   v.set_mnemonic (opcode->mnem);

   switch (opcode->mode) {

      case direct:
      case indexed:
      case relative:
      case immediate8:
         pc++;
         break;

      case immediate16:
      case extended:
         pc += 2;
         break;

      case implied:
         break;
   }


   // Add code chunks or mark instruction as branch.
   switch (c) {
      case opcode_jmp:
      case opcode_bra:
      case opcode_rti:
      case opcode_rts:
         v.set_is_branch (true);
         break;
   }
}

// Analyse instruction.
//
// v   Instruction descriptor.
// seg Program segment to use.
// pc  Program counter
void cpu_6502::analyze (vop& v, vsegment* seg, vaddr& pc)
{
   vimage* img = seg->image ();
   vaddr strt = pc;
   get_vop (v, seg, strt);
   strt = pc;

   unsigned char c;

   c = img->get (pc);
   cpu_6502_opcode* opcode = &opcodemap[c];

   if (v.is_invalid ())
      return;

   unsigned char b;
   signed char d;
   vaddr w = 0;

   vchunk* ch = 0;
   switch (opcode->mode) {

      case immediate8:
         b = img->get (pc);
         break;

      case immediate16:
         w = img->get_word (pc);
         if (w != 0xffff)
            seg->insert_data (ch = new vdata (seg, w), strt);
         break;

      case extended:
         w = img->get_word (pc);
         if (c == opcode_jmp)
            seg->insert_code (new vcode (seg, w), strt);
         else if (c == opcode_jsr)
             seg->insert_subroutine (new vsubroutine (seg, w), strt);
         else if (w != 0xffff)
            seg->insert_data (ch = new vdata (seg, w), strt);
         break;

      case direct:
      case indexed:
         b = img->get (pc);
         seg->insert_data (ch = new vdata (seg, (vaddr) b), strt);
         break;

      case relative:
         d = img->get (pc);
         seg->insert_code (new vcode (seg, (vaddr) ((long) pc + (long) d)), strt);
         break;

      case implied:
         break;
   }
   if (ch)
      ch->set_datatype (opcode->datatype);
}

// Return pointer to one and only instance of this class.
cpu_6502* cpu_6502::instance ()
{
   if (_instance)
      return _instance;
   return _instance = new cpu_6502 ();
}
