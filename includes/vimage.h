// $Id$
//
// Virtual program segment and related.

#ifndef _VIMAGE_H
#define _VIMAGE_H

#include <deque>
#include <fstream>

#include "vop.h"

// Virtual program segment
//
// vimage holds all vchunks of a program and the raw binary.
class vimage {

   public:
      vimage (fstream& infile, unsigned long offset);
      virtual ~vimage ();

      virtual const char* type () { return "raw image"; }

      // Get a raw byte from code image.
      inline unsigned char get (unsigned long& pc)
      {
        return _image [pc++];
      }

      // Get a raw word from code image.
      inline unsigned short get_word (unsigned long& pc)
      {
        unsigned char l, h;

        h = get (pc);
        l = get (pc);
        return (h << 8) + l;
      }

      inline vaddr start () { return _start; }
      inline vaddr end () { return _end; }

   private:

      vaddr _start;
      vaddr _end;
      char* _image;
};

#endif
