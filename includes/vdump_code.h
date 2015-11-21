// Copyright (c) 2002 Sven Michael Klose <pixel@hugbox.org>

#ifndef _VDUMP_CODE_H
#define _VDUMP_CODE_H

#include "vdump.h"

// A vchunk is a piece of segment containing code or data.
class vdump_code : public vdump {
    public:
        inline vdump_code () : vdump () {}

        void dump (vchunk* chunk);

        void label (vchunk* chunk)
        {
            dump_coderef (chunk);
        }
};

#endif
