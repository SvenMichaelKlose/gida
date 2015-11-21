#ifndef _VSUBROUTINE_H
#define _VSUBROUTINE_H

class vsegment;
class vcpu;

#include "vcode.h"

// A vchunk is a piece of segment containing code or data.
class vsubroutine : public vcode {

   public:
      vsubroutine (vsegment* segment, vaddr start);

      void dump ();

      static inline int num () { return _num_subs; }

   private:
      static int _num_subs;
};

#endif
