#include <stdlib.h>
#include <string>

#include "vcode.h"
#include "vdata.h"
#include "config-read.h"

void
type_error (int line)
{
    cerr << "Unknown label type in config file at "
         << "line " << line << ". Use 'c' for code, 'd' for data or "
         << "'v' to make it a code vector." << endl;
    exit (-1);
}

void
config_read_line (vsegment& seg, istream& in, int& line)
{
    int c;
    string type;
    string addr;
    string name;

    line++;

    // Skip empty and comment lines.
    c = in.get ();
    if (in.eof () || c == '\n')
        return;

    if (c == '#') {
        in.ignore (4096, '\n');
        return;
    }
    in.putback (c);

    in >> type;
    vaddr a;
    in >> std::hex >> a;
    in >> name;

    // Get type.
    if (type.length () != 1)
        type_error (line);

    bool code = false;
    bool data = false;
    bool vec = false;
    switch (type[0]) {
        case 'c':
            code = true;
            break;
        case 'd':
            data = true;
            break;
        case 'v':
            vec = true;
            code = true;
            break;
        default:
            type_error (line);
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

int
config_read (vsegment& seg, istream& in)
{
    int line = 0;

    while (in.good () && !in.eof ())
        config_read_line (seg, in, line);

    return 0;
}
