// Copyright (c) 2002 Sven Michael Klose <sven@hugbox.org>
//
// vgap prints an extra label for unreferenced vchunks. This could be replaced
// by a common reference map.

#include <fstream>

#include "vop.h"
#include "vcpu.h"
#include "vdump.h"
#include "vchunk.h"
#include "vcode.h"
#include "vdata.h"
#include "vgap.h"
#include "vsegment.h"

void
vgap::dump ()
{
    cout << "\n; !!! NOT REACHED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl
         << ";" << endl
         << "; The following " << end () - start ()
         << " bytes are not accessed by any displayed code." << endl;

    vchunk::dump ();

    cout << "; !!! END OF GAP !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
         << endl << endl;
}
