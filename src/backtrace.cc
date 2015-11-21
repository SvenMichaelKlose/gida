#include "backtrace.h"

// Execute a chunk to get the result of a vref.
//
// This function creates a tree of vops. Unitialized operands are
// set to 0 and the tree is executed. The result is stored to the
// instructions vref which the vcpu should already know.
//
// chunk: The chunk to convert.
// pc:    The root instruction of the tree.
void vcpu::exec (vchunk* chunk, vaddr pc)
{
   tree = vops_to_tree (chunk, pc);
   tree->();
   delete tree;
}

// Create vop tree from chunk.
//
//
// chunk: The chunk to convert.
// pc:    The root instruction of the tree.
// Returns: Pointer to root vop of the tree the user must delete himself.
vop* vchunk::tree (vchunk* chunk, vaddr pc)
{
}
/*
   // Calculate variable operands.
   for (int i = 0; i < v.num_ops (); i++) {
      vref& ref = v.op[i];
      if (ref.is_constant () == false)
         exec (chunk, pc_start);
      operand = ref->get ();
   }
*/
