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

#include <fstream>
#include <iostream>
#include <string>
#include <GL/glfw3.h>
#include <joefile/input_stream.hpp>
#include <joemath/joemath.hpp>

using namespace JoeMath;
using namespace JoeFile;

namespace JoeFx
{
    Pass::Pass ()
    :m_initialized( false )
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

    bool Pass::Validate()
    {
        return m_initialized;
    }

    void Pass::SetState() const
    {
        //TODO should this throw an error?
        if( !m_initialized )
            return;

        // This will still work when m_program is 0
        glUseProgram( m_program );
    }

    void Pass::ResetState() const
    {
        if( !m_initialized )
            return;

        glUseProgram( 0 );
    }

    bool Pass::LoadFromInputStream( InputStream& input_stream )
    {
        input_stream >> m_name;

        u32 num_shaders;
        input_stream >> num_shaders;

        m_shaders.resize( num_shaders );
         
        for( auto& s : m_shaders )
        {
            ShaderDomain domain;
            input_stream >> domain;

            std::string filename;
            input_stream >> filename;

            s = LoadShader( domain, filename ); 
        }

        m_program = CreateProgram( m_shaders ); 

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
        GLuint shader_type;
        switch( domain )
        {
            case FRAGMENT_DOMAIN:
                shader_type = GL_FRAGMENT_SHADER;
                break;
            case VERTEX_DOMAIN:
                shader_type = GL_VERTEX_SHADER;
                break;
            default:
                return 0;
        }

        std::ifstream file( filename );
        std::string program_string;

        file.seekg( 0, std::ios::end );   
        program_string.reserve( file.tellg() );
        file.seekg( 0, std::ios::beg );

        program_string.assign( ( std::istreambuf_iterator<char>( file ) ),
                               std::istreambuf_iterator<char>() );

        const char* c_str = program_string.c_str();


        GLuint shader = glCreateShader( shader_type );
        
        glShaderSource( shader, 1, &c_str, nullptr );

        glCompileShader( shader );

        GLint status;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
        if ( status == GL_FALSE )
        {
            GLint info_log_size;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &info_log_size );
            
            GLchar *info_log = new GLchar[info_log_size + 1];
            glGetShaderInfoLog( shader, info_log_size, nullptr, info_log );
            
            std::cerr << "Error compiling shader: \"" << filename << "\"\n" << info_log << std::endl;
            delete[] info_log;
            return 0;
        }
                 
        return shader;
    }

    GLuint Pass::CreateProgram ( const std::vector<GLuint>& shaders )
    {
        GLuint program = glCreateProgram();

        for( auto s : shaders )
            glAttachShader( program, s );

        glLinkProgram( program );

        GLint status;
        glGetProgramiv ( program, GL_LINK_STATUS, &status );
        if ( status == GL_FALSE )
        {
            GLint info_log_size;
            glGetProgramiv( program, GL_INFO_LOG_LENGTH, &info_log_size );
            
            GLchar *info_log = new GLchar[info_log_size + 1];
            glGetProgramInfoLog( program, info_log_size, nullptr, info_log );
            
            std::cerr << "Error linking program:\n" << info_log << std::endl;
            delete[] info_log;
        }
       
        return program; 
    }
}
