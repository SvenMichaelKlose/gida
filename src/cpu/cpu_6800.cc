// Copyright (c) 2002,2015 Sven Klose <sven@devcon.net> 2002/04/16

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
#include "cpu_6800.h"

// The one and only instance of cpu_6800.
cpu_6800 * cpu_6800::_instance;

//////////////////////////////////////////////////////////////////////

// Create opcode from base and offset.
#define OPCODE(base,offset) ((unsigned char) (base - offset))

const unsigned char opcode_bra = 0x20;
const unsigned char opcode_jmp = 0x7e;
const unsigned char opcode_jsr = 0xbd;
const unsigned char opcode_rti = 0x3b;
const unsigned char opcode_rts = 0x39;

// List of descriptors for all instructions.
const cpu_6800_inst instab[] = {
    { "ABA ", 0x1b, g_implied },
    { "ADCA", 0xb9, g_immediate8 },
    { "ADCB", 0xf9, g_immediate8 },
    { "ADDA", 0xbb, g_immediate8 },
    { "ADDB", 0xfb, g_immediate8 },
    { "ANDA", 0xb4, g_immediate8 },
    { "ANDB", 0xf4, g_immediate8 },
    { "ASL ", 0x78, g_extended },
    { "ASLA", 0x48, g_implied },
    { "ASLB", 0x58, g_implied },
    { "ASR ", 0x77, g_extended },
    { "ASRA", 0x47, g_implied },
    { "ASRB", 0x57, g_implied },

    { "BCC ", 0x24, g_relative },
    { "BCS ", 0x25, g_relative },
    { "BEQ ", 0x27, g_relative },
    { "BGE ", 0x2c, g_relative },
    { "BGT ", 0x2e, g_relative },
    { "BHI ", 0x22, g_relative },
    { "BITA", 0xb5, g_immediate8 },
    { "BITB", 0xf5, g_immediate8 },
    { "BLE ", 0x2f, g_relative },
    { "BLS ", 0x23, g_relative },
    { "BLT ", 0x2d, g_relative },
    { "BMI ", 0x2b, g_relative },
    { "BNE ", 0x26, g_relative },
    { "BPL ", 0x2a, g_relative },
    { "BRA ", opcode_bra, g_relative },
    { "BSR ", 0x8d, g_relative },
    { "BVC ", 0x28, g_relative },
    { "BVS ", 0x29, g_relative },

    { "CBA ", 0x11, g_implied },
    { "CLC ", 0x0c, g_implied },
    { "CLI ", 0x0e, g_implied },
    { "CLR ", 0x7f, g_extended },
    { "CLRA", 0x4f, g_implied },
    { "CLRB", 0x5f, g_implied },
    { "CLV ", 0x0a, g_implied },
    { "CMPA", 0xb1, g_immediate8 },
    { "CMPB", 0xf1, g_immediate8 },
    { "COM ", 0x63, g_extended },
    { "COMA", 0x43, g_implied },
    { "COMB", 0x53, g_implied },
    { "CPX ", 0xbc, g_immediate16 | g_word },

    { "DAA ", 0x19, g_implied },
    { "DEC ", 0x7a, g_extended },
    { "DECA", 0x4a, g_implied },
    { "DECB", 0x5a, g_implied },
    { "DES ", 0x34, g_implied },
    { "DEX ", 0x09, g_implied },

    { "EORA", 0xb8, g_immediate8 },
    { "EORB", 0xf8, g_immediate8 },
    { "INC ", 0x7c, g_extended },
    { "INCA", 0x4c, g_implied },
    { "INCB", 0x5c, g_implied },
    { "INS ", 0x31, g_implied },
    { "INX ", 0x08, g_implied },

    { "JMP ", opcode_jmp, g_extended },
    { "JSR ", opcode_jsr, g_extended },

    { "LDAA", 0xb6, g_immediate8 },
    { "LDAB", 0xf6, g_immediate8 },
    { "LDS ", 0xbe, g_immediate16 | g_word },
    { "LDX ", 0xfe, g_immediate16 | g_word },
    { "LSR ", 0x74, g_extended },
    { "LSRA", 0x44, g_implied },
    { "LSRB", 0x54, g_implied },

    { "NEG ", 0x70, g_extended },
    { "NEGA", 0x40, g_implied },
    { "NEGB", 0x50, g_implied },
    { "NOP ", 0x01, g_implied },

    { "ORAA", 0xba, g_immediate8 },
    { "ORAB", 0xfa, g_immediate8 },

    { "PSHA", 0x36, g_implied },
    { "PSHB", 0x37, g_implied },
    { "PULA", 0x32, g_implied },
    { "PULB", 0x33, g_implied },

    { "ROL ", 0x79, g_extended },
    { "ROLA", 0x49, g_implied },
    { "ROLB", 0x59, g_implied },
    { "ROR ", 0x76, g_extended },
    { "RORA", 0x46, g_implied },
    { "RORB", 0x56, g_implied },
    { "RTI ", opcode_rti, g_implied },
    { "RTS ", opcode_rts, g_implied },

    { "SBA ", 0x10, g_implied },
    { "SBCA", 0xb2, g_immediate8 },
    { "SBCB", 0xf2, g_immediate8 },
    { "SEC ", 0x0d, g_implied },
    { "SEI ", 0x0f, g_implied },
    { "SEV ", 0x0b, g_implied },
    { "STAA", 0xb7, g_direct },
    { "STAB", 0xf7, g_direct },
    { "STS ", 0xbf, g_direct | g_word },
    { "STX ", 0xff, g_direct | g_word },
    { "SUBA", 0xb0, g_immediate8 },
    { "SUBB", 0xf0, g_immediate8 },
    { "SWI ", 0x3f, g_implied },

    { "TAB ", 0x16, g_implied },
    { "TAP ", 0x06, g_implied },
    { "TBA ", 0x17, g_implied },
    { "TPA ", 0x07, g_implied },
    { "TST ", 0x7d, g_extended },
    { "TSTA", 0x4d, g_implied },
    { "TSTB", 0x5d, g_implied },
    { "TSX ", 0x30, g_implied },
    { "TXS ", 0x35, g_implied },

    { "WAI ", 0x3e, g_implied }
};

// Opcode descriptor.
typedef struct _cpu_6800_opcode {
    const char *      mnem;    // Mnemonic
    cpu_6800_adrmode  mode; // Addressing mode
    int               datatype;
} cpu_6800_opcode;

// Opcode descriptor map.
cpu_6800_opcode opcodemap[256];

// Create opcode for each addressing mode in group.
void cpu_6800::create_opcode (const cpu_6800_inst * d)
{
    const char * mnem = d->mnem;
    const unsigned char op = d->opcode;
    const int modes = d->modes;
    opcodemap[op].datatype = modes & g_byte ? 1 : modes & g_word ? 2 : 0;

    // Opcode offsets.
    const unsigned char op_indexed = 0x10;
    const unsigned char op_direct = 0x20;
    const unsigned char op_immediate = 0x30;

    switch (modes & g_mask) {
        case g_implied:
            opcodemap[op].mode = implied;
            break;

        case g_relative:
            opcodemap[op].mode = relative;
            break;

        case g_immediate8:
        case g_immediate16:
            opcodemap[OPCODE(op, op_immediate)].mode =
                modes == g_immediate8 ? immediate8 : immediate16;
            opcodemap[OPCODE(op, op_immediate)].mnem = mnem;

        case g_direct:
            opcodemap[OPCODE(op, op_direct)].mode = direct;
            opcodemap[OPCODE(op, op_direct)].mnem = mnem;

        case g_extended:
            opcodemap[OPCODE(op, op_indexed)].mode = indexed;
            opcodemap[OPCODE(op, op_indexed)].mnem = mnem;
            opcodemap[op].mode = extended;
            break;
    }
    opcodemap[op].mnem = mnem;
}

// Initialise. Create map of all opcodes.
cpu_6800::cpu_6800 ()
{
    // Mark all opcodes as invalid.
    for (int i = 0; i < 256; i++)
        opcodemap[i].mnem = 0;

    // Create opcode descriptors for each instruction.
    int instructions = sizeof (instab) / sizeof (cpu_6800_inst);
    for (int i = 0; i < instructions; i++)
        create_opcode (&instab[i]);
}

void
cpu_6800::disassemble_operand (vsegment * seg, vaddr & pc, cpu_6800_adrmode mode)
{
    vimage * img = seg->image ();
    unsigned char l;
    unsigned int operand = 0; // Damn. Need to initialize this because gcc will
                              // not clear the high word when converting from
                              // unsigned char if optimization is > 1.
    vchunk * ch;
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
void
cpu_6800::disassemble (vsegment * seg, vaddr & pc)
{
    vimage * img = seg->image ();
    unsigned char c = img->get (pc);

    cpu_6800_opcode * opcode = &opcodemap[c];

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
void
cpu_6800::get_vop (vop & v, vsegment * seg, vaddr& pc)
{
    vimage * img = seg->image ();
    unsigned char c;

    c = img->get (pc);
    cpu_6800_opcode * opcode = &opcodemap[c];

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
void
cpu_6800::analyze (vop & v, vsegment * seg, vaddr & pc)
{
    vimage * img = seg->image ();
    vaddr strt = pc;
    get_vop (v, seg, strt);
    strt = pc;

    unsigned char c;

    c = img->get (pc);
    cpu_6800_opcode * opcode = &opcodemap[c];

    if (v.is_invalid ())
        return;

    unsigned char b;
    signed char d;
    vaddr w = 0;

    vchunk * ch = 0;
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
cpu_6800 *
cpu_6800::instance ()
{
    if (_instance)
        return _instance;
    return _instance = new cpu_6800 ();
}
