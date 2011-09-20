#!/usr/bin/env python3

import struct

frag_shader = """
                #version 330

                out vec4 output_color;

                void main()
                {
                    output_color = vec4( 0.0f, 0.0f, 1.0f, 1.0f );
                }
              """

vert_shader = """
                #version 330

                layout (location = 0) in vec4 position;

                void main()
                {
                    gl_Position = position;
                }
              """

int_struct = struct.Struct( "I" )

def main():
    fout = open( "blue.jfxc", "wb" ) 

    jfxc_magic = int_struct.pack( 0x4358464A )
    jfxc_version = int_struct.pack( 1 )

    fout.write( jfxc_magic )
    fout.write( jfxc_version )

    vert_length = int_struct.pack( len( vert_shader ) )
    frag_length = int_struct.pack( len( frag_shader ) )

    fout.write( vert_length )
    fout.write( bytes( vert_shader, "ascii" ) )
    fout.write( frag_length )
    fout.write( bytes( frag_shader, "ascii" ) )
    fout.close()

if __name__ == "__main__":
    main()
