#ifndef _VOP_H
#define _VOP_H

// An address in a vimage binary.
typedef unsigned long vaddr;

// vcpus generate vops to describe a single instruction at a particular
// address.
class vop {

   public:
      inline vop () : _is_branch (false), _mnemonic (0) {}
      inline bool is_branch () { return _is_branch; }
      inline void set_is_branch (bool is_branch) { _is_branch = is_branch; }
      inline bool is_invalid () { return _mnemonic ? false : true; }
      inline const char* mnemonic () { return _mnemonic; }
      inline void set_mnemonic (const char*  mnemonic) { _mnemonic = mnemonic; }

   private:
      bool _is_branch;
      const char* _mnemonic;
};

#endif
