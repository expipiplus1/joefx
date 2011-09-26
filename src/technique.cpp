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

#include <joefx/technique.hpp>

#include <string>
#include <vector>
#include <joefile/input_stream.hpp>

namespace JoeFx
{
    Technique::Technique ()
    {
    }

    Technique::Technique ( Technique&& other )
    {
        *this = std::move( other );
    }
    
    Technique& Technique::operator = ( Technique&& other )
    {
        bool initialized = other.m_initialized;
        other.m_initialized = false;
        m_initialized = initialized;
        m_passes = std::move( other.m_passes );
        m_name   = std::move( other.m_name );
        //m_name   = other.m_name;

        return *this;
    }

    Technique::~Technique ()
    {
    }

    bool Technique::LoadFromInputStream ( InputStream& input_stream )
    {
        input_stream >> m_name;

        u32 num_passes;
        input_stream >> num_passes;

        m_passes.resize( num_passes );

        for( auto& p : m_passes )
        {
            if( !p.LoadFromInputStream( input_stream ) )
                return false;
        }

        m_initialized = true;
        return true;
    }
}
