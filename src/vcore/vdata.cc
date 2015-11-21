// Copyright (c) 2002 Sven Michael Klose <pixel@hugbox.org>

#include "vchunk.h"
#include "vdata.h"
#include "vdump_data.h"

vdump_data * vdata_default_dumper = new vdump_data ();

vdata::vdata (vsegment * segment, unsigned long start) : vchunk (segment, start)
{
   set_dumper (vdata_default_dumper);
}
