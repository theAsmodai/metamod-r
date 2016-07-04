#!/bin/sh
PATH=/bin:/usr/bin:/sbin:/usr/sbin

nm -CDg $1 | egrep " T " | egrep -v "::|\(| T [A-Z]| _|type_info|anglemod" | cut -d' ' -f 3,3
