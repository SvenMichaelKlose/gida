// Copyright (c) 2002,2015 Sven Michael Klose <pixel@hugbox.org>

#ifndef _CPU_6502_H
#define  _CPU_6502_H

#include "vcpu.h"

class cpu_6502 : public vcpu {

    public:
        cpu_6502 () {}

        const char * name () { return "6502"; }
        const char * vendor () { return "MOS Technology"; }
        const char * classtype () { return "Microprocessor (8 bit/64k address space)"; }

        void get_vop (vop & v, vsegment * seg, unsigned long & pc);
        void analyze (vop & v, vsegment * seg, unsigned long & pc);
        void disassemble (vsegment * seg, unsigned long & pc);
};

#endif // _CPU_6502_H
