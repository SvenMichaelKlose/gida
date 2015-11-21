// $Id$
//
// Main file
//
// (c) 2002 Sven Klose <sven@devcon.net>

#include "config.h"
#include "vop.h"
#include "vdump.h"
#include "vchunk.h"
#include "vcode.h"
#include "vdata.h"
#include "vcpu.h"
#include "vsegment.h"
#include "cpu_6802.h"

void print_version ()
{
   cout << PACKAGE << " interactive disassembler version " << VERSION << endl;
}

void print_help ()
{
   print_version ();
   cout << "Usage: " << PACKAGE << " MYROM.BIN <MYROM.CONF" << endl;
   exit (0);
}

int main (int argc, char** argv)
{
   bool no_trace = false;
   bool no_unreached_code = false;
   vaddr offset = 0x8000;

   if (argc < 2)
      print_help ();

   // Create vsegment from ROM image for 6802 CPU.
   vcpu* cpu = cpu_6802::instance ();
   fstream infile;
   infile.open (argv[1], ios::in | ios::binary);
   if (!infile.is_open ()) {
        cerr << "Couldn't find file '" << argv[1] << "'." << endl;
	exit (-1);
   }
   vimage rom (infile, offset);
   vsegment seg (&rom, cpu);
   infile.close ();

   // Process configuration file for image.
   config_read (seg, cin);

   // Trace image.
   if (no_trace == false)
      seg.trace ();

   //if (no_unreached_code == false)
      //seg.trace_unreached ();

   // Dump everything to stdout.
   seg.dump ();

   return 0;
}
