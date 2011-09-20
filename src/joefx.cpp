/*
    Copyright 2011 Joe Hermaszewski. All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are
    permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of
    conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list
    of conditions and the following disclaimer in the documentation and/or other materials
    provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY Joe Hermaszewski "AS IS" AND ANY EXPRESS OR IMPLIED
    WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Joe Hermaszewski OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
    ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    The views and conclusions contained in the software and documentation are those of the
    authors and should not be interpreted as representing official policies, either expressed
    or implied, of Joe Hermaszewski.
*/

#include <joefx/joefx.hpp>

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace NJoeFX
{
    const int joefx_magic =  0x4358464A;
    const int joefx_version =  1;

    std::string LoadFragmentShader( const std::string filename )
    {
        std::ifstream fin( filename, std::ios_base::binary | std::ios_base::in );
        assert( fin.good() );
        
        int magic;
        fin.read( reinterpret_cast<char*>( &magic ), sizeof( magic ) );
        assert( magic == joefx_magic ); 
        
        int version;
        fin.read( reinterpret_cast<char*>( &version ), sizeof( version ) );
        assert( version == joefx_version );

        int vert_length;
        fin.read( reinterpret_cast<char*>( &vert_length ), sizeof( vert_length ) );

        std::vector<char> bytes( vert_length );
        fin.read( &bytes[0], vert_length );

        std::string ret( &bytes[0], vert_length );
        return ret;
    }

    std::string LoadVertexShader( const std::string filename )
    {
        return filename;
    }
}
