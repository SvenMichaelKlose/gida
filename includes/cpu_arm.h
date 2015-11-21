// Motorola arm CPU support

#ifndef _CPU_ARM_H
#define  _CPU_ARM_H

#include "vcpu.h"

class cpu_arm : public vcpu {

   public:
      static cpu_arm* instance ();
      const char* name ();
      const char* vendor () { return "MIPS"; }
      const char* classtype () { return "Micro processor (RISC, 32 bit)"; }

      void analyze (vop& v, vsegment* seg, unsigned long& pc);
      void get_vop (vop& v, vsegment* seg, unsigned long& pc);

      void disassemble (vsegment* seg, unsigned long& pc);

      void init_image (vsegment& seg, vimage& rom) {}

   private:
      cpu_arm ();
      static cpu_arm* _instance;
};

#endif // _CPU_arm_H
