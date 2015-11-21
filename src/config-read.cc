#include <stdlib.h>
#include <string>

#include "vcode.h"
#include "vdata.h"
#include "config-read.h"

int config_read (vsegment& seg, istream& is)
{
   int c;
   int line = 0;
   string type;
   string addr;
   string name;

   while (is.good () && !is.eof ()) {
      line++;

      // Skip empty and comment lines.
      c = is.get ();
      if (c == '\n')
         continue;

      if (c == '#') {
         is.ignore (4096, '\n');
         continue;
      }
      is.putback (c);

      is >> type;
      vaddr a;
      is >> std::hex >> a;
      is >> name;

      // Get type.
      int l = type.length ();
      bool code = false;
      bool data = false;
      bool vec = false;
      for (int i = 0; i < l; i++) {
         c = type[i];
         switch (c) {
         case 'c':
            code = true;
            continue;
         case 'd':
            data = true;
            continue;
         case 'v':
            vec = true;
            code = true;
            continue;
         default:
            cerr << "Unknown label type in config file at "
                 << "line " << line << ". Use 'c' for code, 'd' for data or "
                 << "'v' to make it a code vector." << endl;
            exit (-1);
         }
      }
      if (vec) {
         vdata* rv = new vdata (&seg, a);
         rv->set_name ((char*) name.data ());
         rv->set_datatype (2);
         seg.insert_data (rv);
         name.append ("_destination");
         a = seg.image ()->get_word (a);
      }
      vcode* rv = new vcode (&seg, a);
      rv->set_name ((char*) name.data ());
      rv->set_datatype (2);
      seg.insert_code (rv);
   }

   return 0;
}
