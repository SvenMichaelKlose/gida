#ifndef _VCODE_H
#define _VCODE_H

class vsegment;

#include "vchunk.h"
#include "vdump_code.h"

// A vchunk is a piece of segment containing code or data.
class vcode : public vchunk {

   public:
      vcode (vsegment* segment, unsigned long start);
};

#endif
