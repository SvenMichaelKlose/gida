#ifndef _VGAP_H
#define _VGAP_H

// A vchunk is a piece of segment containing code or data.
class vgap : public vchunk {

   public:
      inline vgap (vsegment* seg, vaddr start) : vchunk (seg, start) {}
      void dump ();
};

#endif
