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
    cout << "\n; !!! NOT REACHED !!!" << endl
         << ";" << endl
         << "; The following " << end () - start ()
         << " bytes are not accessed." << endl;

    vchunk::dump ();

    cout << "; !!! END OF GAP !!!" << endl
         << endl;
}
