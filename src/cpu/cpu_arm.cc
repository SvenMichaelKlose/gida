// arm CPU support
//
// (c) 2002 Sven Klose <sven@devcon.net> 2002/04/16
//
// About this file:
//
// This file contains a virtual CPU for disassembly of binaries that run on
// Motorola arm micro controllers.

#include <fstream>

#include "vop.h"
#include "vcpu.h"
#include "vdump.h"
#include "vchunk.h"
#include "vcode.h"
#include "vdata.h"
#include "vsegment.h"
#include "vsubroutine.h"
#include "cpu_arm.h"

// The one and only instance of cpu_arm.
cpu_arm* cpu_arm::_instance;

cpu_arm::cpu_arm ()
{
}

// Disassemble to plain text line.
void cpu_arm::disassemble (vsegment* seg, vaddr& pc)
{
}

// Get vop of instruction.
//
// v   Instruction descriptor.
// seg Program segment to use.
// pc  Program counter
void cpu_arm::get_vop (vop& v, vsegment* seg, vaddr& pc)
{
}

// Analyse instruction.
//
// v   Instruction descriptor.
// seg Program segment to use.
// pc  Program counter
void cpu_arm::analyze (vop& v, vsegment* seg, vaddr& pc)
{
}

// Return pointer to one and only instance of this class.
cpu_arm* cpu_arm::instance ()
{
   if (_instance)
      return _instance;
   return _instance = new cpu_arm ();
}

const char* cpu_arm::name ()
{
   return "arm";
}
