// $Id$
//
// Virtual binary image.
//
// (c) 20002 Sven Klose <sven@devcon.net>
//
// About this file:
//
// vimage holds a binary and provides machine-independet accessors.

#include <algorithm>
#include <functional>
#include <iterator>
#include <fstream>

#include "vop.h"
#include "vcpu.h"
#include "vdump.h"
#include "vchunk.h"
#include "vcode.h"
#include "vdata.h"
#include "vimage.h"
#include "vsubroutine.h"

// Read in raw image and set CPU type.
vimage::vimage (fstream& infile, vaddr offset)
{
   _image = new char[6553600];
   vaddr p = offset;
   char c;
   while (!infile.eof ()) {
      infile.get (c);
      _image[p++] = c;
   }
   _start = offset;
   _end = p - 1;
}

vimage::~vimage ()
{
   delete _image;
}
