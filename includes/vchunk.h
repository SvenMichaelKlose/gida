// Copyright (c) 2002 Sven Michael Klose <pixel@hugbox.org>
//
// Virtual chunk of code or data.
//
// A vchunk contains some bytes of an image. It can be given a name which is
// used in labels when displaying it. Each chunk points to a dumper that
// is reponsible of displaying a chunk.
// A vchunk also contains the data size of accesses to the chunks starting
// address.

#ifndef _VCHUNK_H
#define _VCHUNK_H

#include <string>

class vsegment;
class vcpu;
#include "vop.h"
#include "vdump.h"

// A vchunk is a piece of segment containing code or data.
class vchunk {
    public:
        inline vchunk (vsegment * segment, vaddr start) :
            _segment (segment), _start (start), _end (0x100000), _dumper (0),
            _name (0), _datatype (0)
        {}

        void trace ();

        inline vsegment * segment () { return _segment; }
        inline vaddr start () { return _start; }
        inline void set_start (vaddr start) { _start = start; }

        inline vaddr end () { return _end; }
        inline void set_end (vaddr end) { _end = end; }

        inline int datatype () { return _datatype; }
        inline void set_datatype (int datatype) { _datatype = datatype; }

        inline void set_dumper (vdump * d) { _dumper = d; }
        inline vdump * dumper () { return _dumper; }
        void get_dumper ();

        virtual void dump () { _dumper->dump (this); }

        bool is_asciiz ();
        static bool is_valid_code (vsegment * seg, vaddr & pc, vaddr & end);

        inline std::string * name () { return _name; }
        inline void set_name (std::string * name) { _name = name; }
        inline void set_name (char * name) { _name = new std::string (name); }

    private:
        vsegment * _segment;
        vaddr _start;
        vaddr _end;
        vdump * _dumper;
        string * _name;
        int _datatype;
};

#endif // #ifndef _VCHUNK_H
