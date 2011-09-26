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

#include <joefx/pass.hpp>

#include <iostream>
#include <string>
#include <joefile/input_stream.hpp>
#include <joemath/joemath.hpp>

using namespace NJoeMath;
using namespace JoeFile;

namespace JoeFx
{
    Pass::Pass ()
    {
    }

    Pass::Pass ( Pass&& other )
    {
        *this = std::move( other );
    }

    Pass&   Pass::operator= ( Pass&& other )
    {
        bool initialized = other.m_initialized;
        other.m_initialized = false;
        m_initialized = initialized;
        m_name = std::move( other.m_name );
        m_shaders = std::move( other.m_shaders );
        m_stateAssignments = std::move( other.m_stateAssignments );
        m_program = std::move( other.m_program );

        return *this;
    }

    Pass::~Pass ()
    {
    }

    bool Pass::LoadFromInputStream( InputStream& input_stream )
    {
        input_stream >> m_name;

        u32 num_shaders;
        input_stream >> num_shaders;

        m_shaders.resize( num_shaders );
         
        std::cout << "num shaders: " << num_shaders << std::endl;

        for( auto& s : m_shaders )
        {
            ShaderDomain domain;
            input_stream >> domain;

            std::string filename;
            input_stream >> filename;

            s = LoadShader( domain, filename ); 
        }

        u32 num_state_assignments;
        input_stream >> num_state_assignments;

        m_stateAssignments.resize( num_state_assignments );

        for( auto& s : m_stateAssignments )
        {
            std::string assignee;
            input_stream >> assignee;

            std::string value;
            input_stream >> value;
        }

        m_initialized = true;
        return true;
    }

    GLuint Pass::LoadShader( ShaderDomain domain, std::string filename )
    {
        std::cout << "Loading shader: Domain: " << domain << " file: " << filename << std::endl;
        return 5;
    }
}
