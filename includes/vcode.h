// Copyright (c) 2002 Sven Michael Klose <pixel@hugbox.org>

#ifndef _VCODE_H
#define _VCODE_H

class vsegment;

#include "vchunk.h"
#include "vdump_code.h"

class vcode : public vchunk {
   public:
      vcode (vsegment * segment, unsigned long start);
};

#endif
