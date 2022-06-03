#include <fstream>

#include "vop.h"
#include "vcpu.h"
#include "vdump.h"
#include "vchunk.h"
#include "vcode.h"
#include "vdata.h"
#include "vsegment.h"

#include <stdio.h>

bool
vchunk::is_asciiz ()
{
    vsegment *  seg = segment ();
    vimage *    img = seg->image ();
    vaddr       pc = start ();
    vaddr       tend = end ();
    vaddr       imgend = img->end ();
    int n = 0;

    while (pc < tend && pc < imgend) {
        char c = img->get (pc);

        if (!c)
            return n ? true : false;
        if (!IS_ASCII(c))
            return false;

        n++;
    }

    return false;
}

bool
vchunk::is_valid_code (vsegment* seg, vaddr & pc, vaddr & end)
{
    vcpu * cpu = seg->cpu ();
    vaddr imgend = seg->image ()->end ();

    while (pc < end && pc < imgend) {
        vop v;
        cpu->get_vop (v, seg, pc);

        if (v.is_branch ())
            return true;
        if (v.is_invalid ())
            return false;
    }

    return true;
}

void
vchunk::get_dumper ()
{
    vaddr       pc = start ();
    vaddr       origend = end ();
    vsegment *  seg = segment ();

    bool is_code = is_valid_code (seg, pc, origend);
    vdump* dmp = is_code ? (vdump *) new vdump_code () : (vdump *) new vdump_data ();
    set_dumper (dmp);
}

// Trace code in chunk.
//
// Adds code and data chunks to the vsegment.
void
vchunk::trace ()
{
    vsegment *  seg = segment ();
    vcpu *      cpu = seg->cpu ();
    vaddr       imgend = seg->image ()->end ();
    unsigned long pos = start ();
    unsigned long origend = end (); // end() might change in loop.
    unsigned long pc = pos;

    while (pc < origend && pc < imgend) {
       vop v;
       cpu->analyze (v, seg, pc);

       if (v.is_branch () || v.is_invalid ())
           break;
    }

    set_end (pc);

    seg->mark_processed (pos, pc);
}
