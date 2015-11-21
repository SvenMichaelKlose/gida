#ifndef _VDUMP_H
#define _VDUMP_H

using namespace std;

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#define IS_ASCII(c) (c >= ' ' && c <= '~')

class vchunk;

#include "vop.h"

class vdump {
   public:
      inline vdump () {}
      virtual void dump (vchunk* chunk) = 0;

      virtual void label (vchunk* vchunk) = 0;

      static inline void dump_hex (unsigned long val, int width)
      {
         cout << hex << setfill ('0') << setw (width) << val << dec;
      }

      static inline void dump_hexbyte (unsigned long val)
      {
         dump_hex (val, 2);
      }

      static inline void dump_hexword (unsigned long val)
      {
         dump_hex (val, 4);
      }

      static void xref_list (vchunk* chunk);

   protected:
      static void dump_hexlabel (char type, vchunk* chunk);

      static inline void dump_coderef (vchunk* chunk )
      {
         dump_hexlabel ('L', chunk);
      }

      static inline void dump_dataref (vchunk* chunk)
      {
         dump_hexlabel ('D', chunk);
      }

      static inline void dump_codelabel (vchunk* chunk )
      {
         dump_coderef (chunk);
         cout << ":";
      }

      static inline void dump_datalabel (vchunk* chunk )
      {
         dump_dataref (chunk);
         cout << ":";
      }

   protected:
      static bool _last_type;
};

#endif
