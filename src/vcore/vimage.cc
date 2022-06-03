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

vimage::vimage (fstream & infile, vaddr offset)
{
    vaddr  p = offset;
    char   c;

    // TODO: Isn't there something like this in the stdlib? (pixel)
    _image = new char[6553600];
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
