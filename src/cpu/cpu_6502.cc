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

#define AM_ACCU 1
#define AM_IMM  2
#define AM_ZP   4
#define AM_ZPX  8
#define AM_ZPY  16
#define AM_ABS  32
#define AM_ABSX 64
#define AM_ABSY 128
#define AM_IZPX 256
#define AM_IZPY 512
#define AM_INDI 1024
#define AM_BRANCH 2048

#define BYTE_AMS (AM_IMM | AM_ZP | AM_ZPX | AM_IZPX | AM_IZPY)
#define WORD_AMS (AM_ABS | AM_ABSX | AM_ABSY | AM_INDI)

#define OPCODE_JMP  0x4c
#define OPCODE_JSR  0x20

/* Generated by https://github.com/SvenMichaelKlose/bender/src/gen-opcode-map.lisp */
struct instruction {
    const char * mnemonic;
    int addrmode;
} opcode_map[256] = {
    { "brk", AM_ACCU },
    { "ora", AM_IZPX },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "ora", AM_ZP },
    { "asl", AM_ZP },
    { NULL, 0 },
    { "php", AM_ACCU },
    { "ora", AM_IMM },
    { "asl", AM_ACCU },
    { NULL, 0 },
    { NULL, 0 },
    { "ora", AM_ABS },
    { "asl", AM_ABS },
    { NULL, 0 },
    { "bpl", AM_BRANCH },
    { "ora", AM_IZPY },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "ora", AM_ZPX },
    { "asl", AM_ZPX },
    { NULL, 0 },
    { "clc", AM_ACCU },
    { "ora", AM_ABSY },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "ora", AM_ABSX },
    { "asl", AM_ABSX },
    { NULL, 0 },
    { "jsr", AM_ABS },
    { "and", AM_IZPX },
    { NULL, 0 },
    { NULL, 0 },
    { "bit", AM_ZP },
    { "and", AM_ZP },
    { "rol", AM_ZP },
    { NULL, 0 },
    { "plp", AM_ACCU },
    { "and", AM_IMM },
    { "rol", AM_ACCU },
    { NULL, 0 },
    { "bit", AM_ABS },
    { "and", AM_ABS },
    { "rol", AM_ABS },
    { NULL, 0 },
    { "bmi", AM_BRANCH },
    { "and", AM_IZPY },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "and", AM_ZPX },
    { "rol", AM_ZPX },
    { NULL, 0 },
    { "sec", AM_ACCU },
    { "and", AM_ABSY },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "and", AM_ABSX },
    { "rol", AM_ABSX },
    { NULL, 0 },
    { "rti", AM_ACCU },
    { "eor", AM_IZPX },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "eor", AM_ZP },
    { "lsr", AM_ZP },
    { NULL, 0 },
    { "pha", AM_ACCU },
    { "eor", AM_IMM },
    { "lsr", AM_ACCU },
    { NULL, 0 },
    { "jmp", AM_ABS },
    { "eor", AM_ABS },
    { "lsr", AM_ABS },
    { NULL, 0 },
    { "bvc", AM_BRANCH },
    { "eor", AM_IZPY },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "eor", AM_ZPX },
    { "lsr", AM_ZPX },
    { NULL, 0 },
    { "cli", AM_ACCU },
    { "eor", AM_ABSY },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "eor", AM_ABSX },
    { "lsr", AM_ABSX },
    { NULL, 0 },
    { "rts", AM_ACCU },
    { "adc", AM_IZPX },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "adc", AM_ZP },
    { "ror", AM_ZP },
    { NULL, 0 },
    { "pla", AM_ACCU },
    { "adc", AM_IMM },
    { "ror", AM_ACCU },
    { NULL, 0 },
    { "jmp", AM_INDI },
    { "adc", AM_ABS },
    { "ror", AM_ABS },
    { NULL, 0 },
    { "bvs", AM_BRANCH },
    { "adc", AM_IZPY },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "adc", AM_ZPX },
    { "ror", AM_ZPX },
    { NULL, 0 },
    { "sei", AM_ACCU },
    { "adc", AM_ABSY },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "adc", AM_ABSX },
    { "ror", AM_ABSX },
    { NULL, 0 },
    { NULL, 0 },
    { "sta", AM_IZPX },
    { NULL, 0 },
    { NULL, 0 },
    { "sty", AM_ZP },
    { "sta", AM_ZP },
    { "stx", AM_ZP },
    { NULL, 0 },
    { "dey", AM_ACCU },
    { NULL, 0 },
    { "txa", AM_ACCU },
    { NULL, 0 },
    { "sty", AM_ABS },
    { "sta", AM_ABS },
    { "stx", AM_ABS },
    { NULL, 0 },
    { "bcc", AM_BRANCH },
    { "sta", AM_IZPY },
    { NULL, 0 },
    { NULL, 0 },
    { "sty", AM_ZPX },
    { "sta", AM_ZPX },
    { "stx", AM_ZPY },
    { NULL, 0 },
    { "tya", AM_ACCU },
    { "sta", AM_ABSY },
    { "txs", AM_ACCU },
    { NULL, 0 },
    { "sty", AM_ABSX },
    { "sta", AM_ABSX },
    { "stx", AM_ABSY },
    { NULL, 0 },
    { "ldy", AM_IMM },
    { "lda", AM_IZPX },
    { "ldx", AM_IMM },
    { NULL, 0 },
    { "ldy", AM_ZP },
    { "lda", AM_ZP },
    { "ldx", AM_ZP },
    { NULL, 0 },
    { "tay", AM_ACCU },
    { "lda", AM_IMM },
    { "tax", AM_ACCU },
    { NULL, 0 },
    { "ldy", AM_ABS },
    { "lda", AM_ABS },
    { "ldx", AM_ABS },
    { NULL, 0 },
    { "bcs", AM_BRANCH },
    { "lda", AM_IZPY },
    { NULL, 0 },
    { NULL, 0 },
    { "ldy", AM_ZPX },
    { "lda", AM_ZPX },
    { "ldx", AM_ZPY },
    { NULL, 0 },
    { "clv", AM_ACCU },
    { "lda", AM_ABSY },
    { "tsx", AM_ACCU },
    { NULL, 0 },
    { "ldy", AM_ABSX },
    { "lda", AM_ABSX },
    { "ldx", AM_ABSY },
    { NULL, 0 },
    { "cpy", AM_IMM },
    { "cmp", AM_IZPX },
    { NULL, 0 },
    { NULL, 0 },
    { "cpy", AM_ZP },
    { "cmp", AM_ZP },
    { "dec", AM_ZP },
    { NULL, 0 },
    { "iny", AM_ACCU },
    { "cmp", AM_IMM },
    { "dex", AM_ACCU },
    { NULL, 0 },
    { "cpy", AM_ABS },
    { "cmp", AM_ABS },
    { "dec", AM_ABS },
    { NULL, 0 },
    { "bne", AM_BRANCH },
    { "cmp", AM_IZPY },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "cmp", AM_ZPX },
    { "dec", AM_ZPX },
    { NULL, 0 },
    { "cld", AM_ACCU },
    { "cmp", AM_ABSY },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "cmp", AM_ABSX },
    { "dec", AM_ABSX },
    { NULL, 0 },
    { "cpx", AM_IMM },
    { "sbc", AM_IZPX },
    { NULL, 0 },
    { NULL, 0 },
    { "cpx", AM_ZP },
    { "sbc", AM_ZP },
    { "inc", AM_ZP },
    { NULL, 0 },
    { "inx", AM_ACCU },
    { "sbc", AM_IMM },
    { "nop", AM_ACCU },
    { NULL, 0 },
    { "cpx", AM_ABS },
    { "sbc", AM_ABS },
    { "inc", AM_ABS },
    { NULL, 0 },
    { "beq", AM_BRANCH },
    { "sbc", AM_IZPY },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "sbc", AM_ZPX },
    { "inc", AM_ZPX },
    { NULL, 0 },
    { "sed", AM_ACCU },
    { "sbc", AM_ABSY },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { "sbc", AM_ABSX },
    { "inc", AM_ABSX },
    { NULL, 0 }
};

struct operand_string {
    const char * str;
    int addrmode;
} operand_strings[] = {
    { "#", AM_IMM },
    { "(", AM_IZPX + AM_IZPY + AM_INDI },

    { ")", AM_IZPY },
    { ",", AM_ZPX + AM_IZPX + AM_IZPY + AM_ABSX + AM_ABSY },
    { "x", AM_ZPX + AM_IZPX + AM_ABSX },
    { "y", AM_ZPY + AM_IZPY + AM_ABSY },
    { ")", AM_IZPX + AM_INDI },
    { NULL, 0 }
};

// The one and only instance of cpu_6502.
cpu_6502 * cpu_6502::_instance;

const char *
print_operand_string (struct operand_string * s, int addrmode)
{
    if (s->addrmode & addrmode)
        cout << s->str;

    return s->str;
}

void
disassemble_word_operand (vsegment * seg, vaddr operand)
{
    vchunk * ch = seg->find (operand);

    if (ch)
        ch->dumper ()->label (ch);
    else {
        cout << "$";
        vdump::dump_hexword (operand);
    }
}

void
cpu_6502::disassemble (vsegment * seg, vaddr& pc)
{
    vimage * img = seg->image ();
    unsigned int opcode = img->get (pc);
    struct instruction * i = &opcode_map[opcode];
    struct operand_string * s = operand_strings;
    int operand;

    // Print byte constant if opcode is invalid.
    if (i->mnemonic == NULL) {
        cout << ".byte $";
        vdump::dump_hexbyte (opcode);
        cout << "\n";
        return;
    }

    cout << "        " << i->mnemonic;
    if (!(i->addrmode & AM_ACCU))
        cout << " ";

    print_operand_string (s++, i->addrmode);
    print_operand_string (s++, i->addrmode);

    if (i->addrmode & (BYTE_AMS | WORD_AMS | AM_BRANCH)) {
        if (i->addrmode & (BYTE_AMS | AM_BRANCH))
            operand = img->get (pc);
        else if (i->addrmode & WORD_AMS)
            operand = img->get_word (pc);

        if (i->addrmode & BYTE_AMS)
            vdump::dump_hexbyte (operand);
        else if (i->addrmode & WORD_AMS)
            disassemble_word_operand (seg, operand);
        else if (i->addrmode & AM_BRANCH)
            disassemble_word_operand (seg, pc + (char) operand);
    }

    while (print_operand_string (s++, i->addrmode));

    cout << "\n";
}

// Get vop of instruction.
//
// v   Instruction descriptor.
// seg Program segment to use.
// pc  Program counter
void
cpu_6502::get_vop (vop& v, vsegment * seg, vaddr& pc)
{
    vimage * img = seg->image ();
    unsigned int opcode = img->get (pc);
    struct instruction * i = &opcode_map[opcode];

    if (i->mnemonic == NULL)
        return;
    v.set_mnemonic (i->mnemonic);

    if (i->addrmode & (BYTE_AMS | AM_BRANCH))
        pc++;
    else if (i->addrmode & WORD_AMS)
        pc += 2;

    if (i->addrmode & AM_BRANCH)
        v.set_is_branch (true);
}

// Analyse instruction.
//
// v   Instruction descriptor.
// seg Program segment to use.
// pc  Program counter
void
cpu_6502::analyze (vop& v, vsegment * seg, vaddr& pc)
{
    vimage * img = seg->image ();
    vaddr strt = pc;
    get_vop (v, seg, strt);
    unsigned int opcode = img->get (pc);
    struct instruction * i = &opcode_map[opcode];
    int operand;

    if (v.is_invalid ())
        return;
    if (i->addrmode & AM_IMM)
        return;

    if (i->addrmode & BYTE_AMS)
        operand = img->get (pc);
    else
        operand = img->get_word (pc);

    vchunk * ch = 0;
    if (opcode == OPCODE_JMP)
        seg->insert_code (new vcode (seg, operand), strt);
    else if (opcode == OPCODE_JSR)
        seg->insert_subroutine (new vsubroutine (seg, operand), strt);
    else if (i->addrmode & AM_BRANCH)
        seg->insert_code (new vcode (seg, (vaddr) ((long) pc + (char) operand)), strt);
    else if (operand != 0xffff)
        seg->insert_data (ch = new vdata (seg, operand), strt);

    if (!ch)
        return;

    if (i->addrmode & BYTE_AMS)
        ch->set_datatype (1);
    else if (i->addrmode & WORD_AMS)
        ch->set_datatype (2);
}

// Return pointer to one and only instance of this class.
cpu_6502 *
cpu_6502::instance ()
{
   if (_instance)
       return _instance;
   return _instance = new cpu_6502 ();
}
