// Copyright (c) 2002,2015 Sven Michael Klose <sven@hugbox.org>

#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "config-read.h"
#include "vop.h"
#include "vdump.h"
#include "vchunk.h"
#include "vcode.h"
#include "vdata.h"
#include "vcpu.h"
#include "vsegment.h"
#include "cpu_6502.h"
#include "cpu_6800.h"

void
print_version ()
{
    cout << PACKAGE << " interactive disassembler version " << VERSION << endl;
}

void
print_help ()
{
    print_version ();
    cout << "Usage: " << PACKAGE << " [-6502|-6802] MYROM.BIN <MYROM.CONF" << endl;
    exit (-1);
}

int
main (int argc, char** argv)
{
    bool no_trace           = false;
    bool no_unreached_code  = false;
    vcpu *        cpu = NULL;
    vaddr         offset = 0x8000;
    const char *  pathname_in = NULL;

    if (argc == 1)
        print_help ();

    int a;
    for (a = 1; a < argc; a++) {
         if (!strcmp (argv[a], "-6502"))
             cpu = new cpu_6502 ();
         else if (!strcmp (argv[a], "-6800"))
             cpu = new cpu_6800 ();
         else
             break;
    }
    if (!cpu) {
        cerr << "No CPU type specified." << endl;
        print_help ();
    }
    if (a < argc -1) {
        cerr << "Too many arguments." << endl;
        print_help ();
    }
    pathname_in = argv[a];

    fstream infile;
    infile.open (pathname_in, ios::in | ios::binary);
    if (!infile.is_open ()) {
        cerr << "Couldn't find file '" << pathname_in << "'." << endl;
        exit (-1);
    }
    vimage image (infile, offset);
    vsegment seg (&image, cpu);
    infile.close ();

    config_read (seg, cin);

    if (no_trace == false)
        seg.trace ();

    //if (no_unreached_code == false)
       //seg.trace_unreached ();

    seg.dump ();

    return 0;
}
