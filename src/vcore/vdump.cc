#include "vdump.h"
#include "vchunk.h"
#include "vsegment.h"

bool vdump::_last_type;

void
vdump::dump_hexlabel (char type, vchunk* chunk)
{
    string * s = chunk->name ();
    if (s) {
        cout << *s;
        return;
    }

    cout << type << "_";

    dump_hex (chunk->start (), 4);
}

void
vdump::xref_list (vchunk* chunk)
{
    vaddr  strt = chunk->start ();
    if (!strt) {
        cout << endl;
        return;
    }

    vsegment *  seg = chunk->segment ();
    vxref_map::iterator i;
    vxref_map::iterator end;
    seg->get_xref (i, end, strt);

    if (i != end && i->first == strt)
        cout << "               ; XREFS:";

    while (i != end && i->first == strt) {
        cout << " ";

        vchunk * ch = seg->find (i->second);
        if (ch)
            ch->dumper ()->label (ch);
        else
            dump_hexword (i->second);

        i++;
   }

   cout << endl;
}
