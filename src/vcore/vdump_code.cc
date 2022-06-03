#include "vop.h"
#include "vcpu.h"
#include "vdump.h"
#include "vchunk.h"
#include "vcode.h"
#include "vdata.h"
#include "vsegment.h"
#include "vdump_code.h"

void
vdump_code::dump (vchunk * chunk)
{
    vsegment        * seg = chunk->segment ();
    vcpu            * cpu = seg->cpu ();
    unsigned long   pc = chunk->start ();
    unsigned long   cend = chunk->end ();

    if (!_last_type) {
        cout << endl
             << "; ### CODE" << endl;
        _last_type = true;
    }

    if (pc < 0x8000)
        return;

    dump_codelabel (chunk);
    xref_list (chunk);

    while (pc < cend)
        cpu->disassemble (seg, pc);
}
