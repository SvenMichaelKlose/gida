#ifndef _VDATA_H
#define _VDATA_H

class vsegment;
class vcpu;

#include "vdump_data.h"

// A vchunk is a piece of segment containing code or data.
class vdata : public vchunk {

   public:
      vdata (vsegment* segment, unsigned long start);
};

#endif
