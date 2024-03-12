// Copyright (c) 2002 Sven Michael Klose <pixel@hugbox.org>
//
// Virtual program segment and related.
//
// A vsegment maps vchunks ordered by address.  They are
// guaranteed to not overlap.
// Don't set the end of a chunk on your own.

#ifndef _VSEGMENT_H
#define _VSEGMENT_H

#include <map>

#include "vchunk.h"
#include "vimage.h"

typedef multimap<vaddr, vaddr> vxref_map ;
typedef std::map<vaddr, vchunk *> vchunk_map;

// Virtual program segment
//
// vsegment holds all vchunks of a program and the raw binary.
class vsegment {
    public:
        vsegment (vimage * image, vcpu * cpu);

        // Add a chunk.
        bool insert_code (vchunk * chunk, vaddr referrer = 0);
        bool insert_data (vchunk * chunk, vaddr referrer = 0);
        bool insert_subroutine (vchunk * chunk, vaddr referrer = 0);

        inline vcpu * cpu () { return _cpu; }
        inline vimage * image () { return _image; }

        void trace ();
        void mark_processed (vaddr start, vaddr end);
        void dump ();

        vchunk * find (vaddr addr)
        {
            vchunk_map::iterator i = _map.lower_bound (addr);
            if (i == _map.end () || i->second->start () != addr)
                return 0;
            return i->second;
        }

        void add_xref (vaddr start, vaddr referrer);
        void get_xref (vxref_map::iterator & begin, vxref_map::iterator & end, vaddr addr);

   private:
        bool insert (vchunk * chunk, vaddr referrer);

        vchunk * get_unprocessed_chunk ();

        void _fill_gaps ();

        vimage *    _image;
        vcpu *      _cpu;
        vchunk_map  _map;
        vchunk_map  _unprocessed_code;

        // TOOD: If anyone really wants to know count them outside this class.
        // Also make insert() overloaded instead of insert_code(), insert_data
        // and insert_subroutine(). (pixel)
        int _code_labels;
        int _data_labels;
        int _subroutines;
};

#endif
