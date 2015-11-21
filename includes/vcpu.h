#ifndef _VCPU_H
#define _VCPU_H

class vimage;
class vsegment;

class vcpu {

   public:
      vcpu () {}

      // General information.
      virtual const char* name () = 0;
      virtual const char* vendor () = 0;
      virtual const char* classtype () = 0;

      // Initialize vop object for instruction at address pc
      // in segment seg.
      virtual void get_vop (vop& v, vsegment* seg, unsigned long& pc) = 0;

      // Like get_vop() but adds new chunks for code or data accessed by
      // the instruction.
      virtual void analyze (vop& v, vsegment* seg, unsigned long& pc) = 0;

      // Disassemble a single instruction.
      virtual void disassemble (vsegment* seg, unsigned long& pc) = 0;
};

#endif
