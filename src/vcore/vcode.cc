#include "vcode.h"
#include "vdump_code.h"

vdump_code* vcode_default_dumper = new vdump_code ();

vcode::vcode (vsegment* segment, unsigned long start) :
   vchunk (segment, start)
{
   set_dumper (vcode_default_dumper);
}
