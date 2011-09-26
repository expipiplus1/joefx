#!/usr/bin/env python3

import struct
import sys

from jfx_parser import Parse

int_struct = struct.Struct( "I" )

def WriteInt( i, f ):
    s = int_struct.pack( i )
    f.write( s )

def WriteString( s, f ):
    l = int_struct.pack( len( s ) )
    f.write( l )
    f.write( bytes( s, "ascii" ) )

def main():
    if len( sys.argv ) < 3:
        print( "Need an input and output file" )
        return

    effect = Parse( sys.argv[1] )
    if effect is None:
        return
    
    jfxc_magic = 0x4358464A
    jfxc_version = 3
    
    fout = open( sys.argv[2], "wb" ) 

    WriteInt( jfxc_magic, fout )
    WriteInt( jfxc_version, fout )

    WriteInt( len( effect.techniques ), fout )

    for t in effect.techniques:
        WriteString( t.name, fout )

        WriteInt( len( t.passes ), fout )
        for p in t.passes:
            WriteString( p.name, fout )

            WriteInt( len( p.shader_assignments ), fout )
            for s in p.shader_assignments:
                WriteInt( s.shader_type, fout )
                WriteString( s.filename, fout )

            WriteInt( len( p.state_assignments ), fout )
            for s in p.state_assignments:
                WriteString( s.assignee, fout )
                WriteString( s.value, fout )

    fout.close()

if __name__ == "__main__":
    main()
