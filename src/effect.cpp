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

#include <joefx/effect.hpp>

#include <string>
#include <vector>
#include <joemath/joemath.hpp>
#include <joefile/input_stream.hpp>

using namespace JoeFile;
using namespace JoeMath;

namespace JoeFx
{
    Effect::Effect ()
    :m_initialized( false )
    {
    }

    Effect::~Effect ()
    {
    }

    std::vector<Technique>::iterator Effect::begin()
    {
        return m_techniques.begin();
    }

    std::vector<Technique>::iterator Effect::end()
    {
        return m_techniques.end();
    }

    bool Effect::LoadFromCompiledEffect ( const std::string filename )
    {
        InputStream input_stream;
        if( !input_stream.Read( filename ) )
            return false;

        u32 magic;
        u32 version;

        input_stream >> magic;
        if( magic != JFXC_FILE_MAGIC )
            return false;

        input_stream >> version;
        if( version != JFXC_FILE_VERSION )
            return false;

        u32 num_techniques;
        input_stream >> num_techniques;
        
        m_techniques.resize( num_techniques );

        for( u32 i = 0; i < num_techniques; ++i )
        {
            if( !m_techniques[i].LoadFromInputStream( input_stream ) )
                return false;
        }

        m_initialized = true;
        return true;
    }
}
