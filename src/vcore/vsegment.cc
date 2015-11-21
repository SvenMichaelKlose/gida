// $Id$
//
// Virtual program segment
//
// (c) 20002 Sven Klose <sven@devcon.net>
//
// About this file:
//
// vsegment hold all vchinks for a vimage. It provides insert functions that
// don't destroy the sort order.

#include <fstream>

#include "vop.h"
#include "vcpu.h"
#include "vdump.h"
#include "vchunk.h"
#include "vcode.h"
#include "vdata.h"
#include "vdump_code.h"
#include "vdump_data.h"
#include "vgap.h"
#include "vsegment.h"
#include "vsubroutine.h"

// Map of callee addresses keyed by subroutine address.
vxref_map xref_map;

vsegment::vsegment (vimage* image, vcpu* cpu)
   : _image (image), _cpu (cpu), _code_labels (0), _data_labels (0),
     _subroutines (0)
{
}

void vsegment::add_xref (vaddr start, vaddr referrer)
{
   if (!referrer)
      return;

   vxref_map::iterator xend = xref_map.end ();
   vxref_map::iterator i = xref_map.lower_bound (start);

   // Skip doubles.
   while (i != xend && i->first == start)
      if (i->second == referrer)
         return;
      else
         i++;

   xref_map.insert (i, vxref_map::value_type (start, referrer));
}

void vsegment::get_xref (vxref_map::iterator& begin,
                         vxref_map::iterator& end, vaddr addr)
{
   vxref_map::iterator xend = xref_map.end ();
   begin = end = xref_map.lower_bound (addr);

   while (end != xend && end->first == addr)
      end++;
}

// Fill gaps between chunks.
void vsegment::_fill_gaps ()
{
   vchunk_map::iterator i = _map.begin ();
   vchunk_map::iterator j = i;
   j++;
   while (j != _map.end ()) {
      vaddr pend = (i++)->second->end ();
      vaddr nstart = (j++)->second->start ();
      if (pend < nstart) {
         vchunk* ch = new vgap (this, pend);
         if (vchunk::is_valid_code (this, pend, nstart)) {
            ch->set_dumper (new vdump_code ());
            insert_code (ch);
            ch->trace ();
         } else {
            ch->set_dumper (new vdump_data ());
            insert_data (ch);
         }
      }
   }
}

// Analyse all unprocessed chunks.
void vsegment::trace ()
{
   while (vchunk* chunk = get_unprocessed_chunk ())
      chunk->trace ();

   _fill_gaps ();
}

// Insert chunk into segment and keep sort order.
bool vsegment::insert (vchunk* chunk, vaddr referrer)
{
   vaddr cstart = chunk->start ();
   vaddr cend = chunk->end ();
   vaddr nextstart = 0;

   add_xref (cstart, referrer);

   vchunk_map::iterator i = _map.lower_bound (chunk->start ());

   // Check if a chunk for this address already exists.
   if (i != _map.end ()) {
      nextstart = i->second->start ();
      if (nextstart == cstart)
         return false;
   }

   // Correct ending address of previous chunk.
   if (i != _map.begin ()) { // && !_map.empty ()) {
      vchunk_map::iterator j = i;
      j--;
      vchunk* q = j->second;
      if (q->end () > cstart)
         q->set_end (cstart);
   }

   // Make end of new chunk equal to start of next.
   if (nextstart && nextstart < cend)
         chunk->set_end (nextstart);

   _map.insert (i, vchunk_map::value_type (cstart, chunk));
   return true;
}

// Add chunk to segment and list of unprocessed code and Increment code chunk
// count.
bool vsegment::insert_code (vchunk* code, vaddr referrer)
{
   bool ret;

   if (ret = insert (code, referrer)) {
      _unprocessed_code.insert (vchunk_map::value_type (code->start (), code));
      _code_labels++;
   }
   return ret;
}

// Add chunk to segment and increment data chunk count.
bool vsegment::insert_data (vchunk* data, vaddr referrer)
{
   bool ret;

   if (ret = insert (data, referrer))
      _data_labels++;
   return ret;
}

// Add subroutine chunk to segment.
bool vsegment::insert_subroutine (vchunk* code, vaddr referrer)
{
   bool ret;

   if (ret = insert_code (code, referrer))
      _subroutines++;
   return ret;
}

// Find a chunk not yet processed by analyze().
vchunk* vsegment::get_unprocessed_chunk ()
{
   vchunk_map::iterator i = _unprocessed_code.begin ();

   if (i == _unprocessed_code.end ())
      return 0;
   return i->second;
}

// Remove chunks in range from list of unprocessed code.
void vsegment::mark_processed (vaddr start, vaddr end)
{
   vchunk_map::iterator i = _unprocessed_code.lower_bound (start);

   while (i != _unprocessed_code.end ()) {
      //vchunk* chunk = **i;
      vchunk* chunk = i->second;
      vaddr cstart = chunk->start ();

      // Break if were out of range.
      if (cstart >= end)
         break;

      _unprocessed_code.erase (i++);
   }
}

// Dump all chunks.
void vsegment::dump ()
{
   vcpu* cp = cpu ();
   vimage* img = image ();
   int code_labels = _code_labels - _subroutines;

   cout << "; GNU interactive diasassembler " << VERSION << endl
        << ";" << endl
        << "; CPU type:         " << cp->name () << endl
        << "; CPU vendor:       " << cp->vendor () << endl
        << "; CPU class:        " << cp->classtype () << endl
        << "; Source type:      " << img->type () << endl
        << "; Starting address: 0x" << hex << setw (8) << setfill ('0') << img->start () << endl
        << "; Ending address:   0x" << hex << setw (8) << setfill ('0') << img->end () << dec << endl
        << "; Subroutines:      " << _subroutines << endl
        << "; Code labels:      " << code_labels << endl
        << "; Data labels:      " << _data_labels << endl
        << endl;

   vchunk_map::iterator i;
   for (i = _map.begin (); i != _map.end (); i++)
      i->second->dump ();
}
