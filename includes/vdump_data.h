#ifndef _VDUMP_DATA_H
#define _VDUMP_DATA_H

#include "vdump.h"

// A vchunk is a piece of segment containing data or data.
class vdump_data : public vdump {

   public:
      inline vdump_data () : vdump () {}

      void dump (vchunk* chunk);
      void label (vchunk* chunk)
      {
         dump_dataref (chunk);
      }
};

#endif
