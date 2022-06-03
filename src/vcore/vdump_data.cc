#include <fstream>
#include <iomanip>

#include "vop.h"
#include "vcpu.h"
#include "vdump.h"
#include "vchunk.h"
#include "vcode.h"
#include "vdata.h"
#include "vdump_data.h"
#include "vsegment.h"

void
vdump_data::dump (vchunk * chunk)
{
    vsegment *  seg = chunk->segment ();
    vimage *    img = seg->image ();
    vaddr       pc = chunk->start ();
    vaddr       tend = chunk->end ();

    if (_last_type) {
        cout << endl 
             << "; ### DATA"<< endl;
        _last_type = false;
    }

    dump_datalabel (chunk);
    xref_list (chunk);

    if (pc < img->start () || pc > img->end ())
        return;

    bool ascii = false;
    bool line = false;
    int cc = 0;
    char str[256];
    if (chunk->datatype () == 2) {
        while (pc < (tend - 1) && pc < 0x10000) {
            vaddr     tmp = pc;
            char      c1 = img->get (tmp);
            char      c2 = img->get (tmp);
            unsigned  c = (unsigned int) img->get_word (pc);

            if (cc != -1 && IS_ASCII(c1) && IS_ASCII(c2)) {
                if (cc < 255) {
                    str[cc++] = c1;
                    str[cc++] = c2;
                } else 
                    cc = -1;
            } else
                cc = -1;

            if (!line) {
                cout << "        FCW $";
                dump_hexword (c);
                line = true;
            } else {
                cout << ", $";
                dump_hexword (c);
            }
        }
    }
    if (cc && cc != -1) {
        str[cc++] = 0;
        cout << " ;'" << str << "'" << endl;
        line = false;
    }

    while (pc < tend && pc < 0x10000) {
        unsigned int c = (unsigned int) img->get (pc);
        if (IS_ASCII(c)) {
            if (!ascii) {
                if (line)
                    cout << "\n";
                else
                    line = true;
                cout << "        FCC \"";
                ascii = true;
            }
            cout << (char) c;
        } else {
            if (ascii) {
                cout << "\"\n        FCB $";
                dump_hexbyte (c);
                ascii = false;
            } else {
               if (!line) {
                   cout << "        FCB $";
                   dump_hexbyte (c);
                   line = true;
               } else {
                   cout << ", $";
                   dump_hexbyte (c);
               }
            }
        }
    }
    if (ascii)
        cout << "\"";
    cout << "\n";
}
