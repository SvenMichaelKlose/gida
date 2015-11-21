# What is GIDA?

gida is a tracing disassembler written in C++.

GIDA can only code for the MOS Technology 6800 CPU for which it

- finds code that is probably executed
- recognises subroutines

Try gida with the ROM, ripped from an ancient coke machine:

```
# gida -6800 rom.bin <rom.conf >rom.asm
```

Enjoy!

Support for the MOS Technology 6502 CPU is under contruction.

# Building

gida uses the GNU autotools to get built and installed.
Accordingly, you need to have autoconf and automake installed.

```
autoreconf -i
automake --add-missing
./configure
make
```

# The virtual chunk model

Binaries are traced to set up a list of 'vchunk's. Each vchunk contains a
code or data address, an optional user-defined name for its label, and a
pointer to a dumper (derived from 'vdump').
A vsegment holds all chunks of a binary and guarantees that they're sorted
in ascending order by their starting addresses and that consequent chunks
leave no gaps.

If the binary was traced, the dump() function of each vchunk in the vsegment
is called. Usually it invokes the dumper, the vchunk points to.
There two dumpers for code and data which are invoked when vchunk::dump()
is called. vchunk::dump is overloaded by 'vsubroutine' and 'vgap' to print
extra comments.

The vchunk class provides machine independent function to test and trace
chunks.

# Virtual CPU

You must add a new virtual CPU to support a new processor.
A vcpu generates a 'vop's of particular instructions or inserts new 'vchunk's
a traced instruction accesses. Additionally it must be able to insert
new vchunks to a segment if an instruction accesses it.
Finally, a cpu must be able to generate intermediate instruction object (vop's).

# Displaying diassemblies

The vprint interface is used by vdump objects to output diassemblies.
vprint provides functions to print code, data, labels, constants, comments and
red tape in a particular format vdump doesn't need to know of.

# How to contribute

If you want to implement new CPU supprt, see the vop.h and vcpu.h files
for starters.

# Backtracing code

!!!!!!!!!! FOLLOWING IS JUST PLANNED STUFF !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

Backtracing code is needed to recognize register arguments and jump tables.
The virtual cpu needs to create a intermediate description of
the operation it performs. A description is made from virtual operation
objects that are linked to trees that can be traversed and executed.

Let's take a look at the following real life example that needs recovery of
a jump table's address:

```
chunkstart:
        ASLA           ; Multiply by two for word alignment.
        LDX  #D_9081   ; Load base of jump table.
        JSR  add_xa    ; Add index to base.
        LDX  D_0000,X  ; Get address of jump.
        JMP  D_0000,X  ; Jump.

; ### SUBROUTINE #############################################
;
; Add A to X and store result in X.
add_xa:               ; XREFS: 8b99 8d7f 8dbf 8ddd 8ded 8e44 8e86 8e96 8eb3 8f24 8f4b 8f81 902f 9046 9063 93bc 97fe 9978 999a 99d7 99f9 a529 c937 d0e1 e0be e116
        CLRB
        STX  D_0248
        ADDA D_0249
        ADCB D_0248
        STAA D_0249
        STAB D_0248
        LDX  D_0248
        RTS

; ### DATA ###########################################

D_9081:               ; XREFS: 9043
        FCB $90, $8d, $91
        FCC "8"
        ...
```

From the perspective of the disassembler the first problem occurs at the
first LDX instruction which loads X with a value taken from a memory
location at an unknown address. It now needs to backtrace all
former instructions until a code label is reached or all source values
to calculate X is known. For simplicity let's assume that all vops are
already built and we have the following operations: 

```
vassign (&reg_a, vconstant (0));

chunkstart:
vop_shiftl (&reg_a, vconstant (1));
vop_assign (&reg_x, vconstant (0x9081));
vassign (&reg_s, &reg_pc);
vop_sub (&reg_s, &reg_s, vconstant (2));
vop_assign (&reg_pc, vconstant (add_ax));

add_ax:
vop_assign (&reg_b, vconstant (0), VR_BYTE);
vop_assign (vref (0x0248, VR_WORD), &reg_x);
a = vop_add (&reg_a, &carry_flag, vref (0x0249, VR_BYTE), &reg_a);
vop_addc (&reg_b, 0, vref (0x0248, VR_BYTE), &reg_b, $carry_flag);
vop_assign (vref (0x0249, VR_WORD), &reg_a);
vop_assign (vref (0x0248, VR_WORD), &reg_b);
vop_assign (&reg_x, vref (0x0248, VR_WORD));
vop_add (&reg_s, &reg_s, vconstant (2));
vassign (&reg_pc, &reg_s);

vop_assign (&reg_pc, vref (vconstant (0), &reg_x), VR_WORD);
vop_assign (&reg_pc, vref (vconstant (0), &reg_x), VR_WORD);
```

The operations were constructed the way shown above by the virtual cpu.
Register objects were constructed before while new objects for
constants and memory locations were created. There is no relationship
between a memory location and the last instruction that assigned a
value to it. To get that information, the set of vops is traversed
and the accessed references are linked to each other using a scoreboard
algorithm.
