// $Id$
//
// Virtual subroutine vchunk.
//
// (c) 2002 Sven Klose <sven@devcon.net>
//
// About this file:
//
// This vchunk prints extra comments for its subroutne vchunk.

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

vsubroutine::vsubroutine (vsegment* seg, vaddr start)
   : vcode (seg, start)
{
}

void vsubroutine::dump ()
{
   cout << "\n"
        << "; ### SUBROUTINE #############################################\n"
        << ";" << endl;

   vchunk::dump ();
}
