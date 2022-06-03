#include <fstream>
#include <map>

#include "vop.h"
#include "vcpu.h"
#include "vdump.h"
#include "vchunk.h"
#include "vcode.h"
#include "vdata.h"
#include "vsegment.h"
#include "vsubroutine.h"

int vsubroutine::_num_subs = 0;

vsubroutine::vsubroutine (vsegment * seg, vaddr start) : vcode (seg, start)
{
}

void
vsubroutine::dump ()
{
    cout << "\n"
         << "; ### SUBROUTINE" << endl
         << ";" << endl;

    vchunk::dump ();
}
