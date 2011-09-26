#!/usr/bin/env python3

from modgrammar import *
from modgrammar.extras import *
from modgrammar import Terminal, error_result
import re
from sys import *

shader_enumerations = { "FragmentProgram" : 0,
                        "VertexProgram"   : 1 }

class END_OF_WORD( Terminal ):
    grammar_collapse = True
    grammar_collapse_skip = True
    grammar_whitespace = False
    grammar_desc = "End of word"
  
    @classmethod
    def grammar_parse(cls, text, index, sessiondata):
        string = text.string[index:]
        if len( string ) == 0:
            yield (0, cls(""))
        end_of_word_pattern = re.compile( "[^a-zA-Z0-9_]" )
        match = end_of_word_pattern.match( string )
        if match:
            yield (0, cls(""))
        yield error_result(index, cls)
  
    @classmethod
    def grammar_ebnf_lhs(cls, opts):
        return ("(*end of word*)", ())
  
    @classmethod
    def grammar_ebnf_rhs(cls, opts):
        return None

class Keyword( Grammar ):
    grammar = OR( "technique",
                  "pass" ), END_OF_WORD
    grammar_whitespace = False

class Identifier( Grammar ):
    grammar = EXCEPT( RE( "[A-Za-z_][A-Za-z0-9_]*" ), Keyword )
    grammar_whitespace = False

class SimpleEscapeSequence( Grammar ):
    grammar = "\\", OR( "'",
                        "\"",
                        "?",
                        "\\",
                        "a",
                        "b",
                        "f",
                        "n",
                        "r",
                        "t",
                        "v" )
    grammar_collapse = True 
            
class EscapeSequence( Grammar ):
    grammar = SimpleEscapeSequence
    grammar_collapse = True

class SCharSequence( Grammar ):
    grammar = REPEAT( OR( RE( "[^\"\\\\\\n]+" ),
                          EscapeSequence ), collapse = True )
    grammar_collapse = True

class StringLiteral( Grammar ):
    grammar = "\"", OPTIONAL( SCharSequence ), "\""

    def elem_init( self, k ):
        if self[1]:
            self.value = self[1].string
        else:
            self.value = ""

class Literal( Grammar ):
    grammar = OR( StringLiteral )

    def elem_init( self, k ):
        self.value = self[0].value

class StateAssignment( Grammar ):
    grammar = Identifier, "=", Literal, ";"
    
    def elem_init( self, k ):
        self.assignee = self[0].string
        self.value = self[2].value

class ShaderType( Grammar ):
    grammar = OR( "FragmentProgram",
                  "VertexProgram" )

    def elem_init( self, k ):
        self.shader_type = shader_enumerations[ self.string ]

class ShaderAssignment( Grammar ):
    grammar = ShaderType, "=", "load", StringLiteral, ";"

    def elem_init( self, k ):
        self.shader_type = self[0].shader_type
        self.filename = self[3].value

class Pass( Grammar ):
    grammar = "pass", Identifier, "{", REPEAT( OR( StateAssignment,
                                                   ShaderAssignment ), min = 0 ), "}"

    def elem_init( self, k ):
        self.name = self[1].string 
        self.state_assignments = []
        self.shader_assignments = []
        for s in self[3].elements:
            if isinstance( s, ShaderAssignment ):
                self.shader_assignments.append( s )
            elif isinstance( s, StateAssignment ):
                self.state_assignments.append( s )

class Technique( Grammar ):
    grammar = "technique", Identifier, "{", REPEAT( Pass, min = 0 ), "}"
    
    def elem_init( self, k ):
        self.name = self[1].string
        self.passes = self[3].elements

class TranslationUnit( Grammar ):
    grammar = REPEAT( Technique, collpase = True, min = 0 ), EOF
   
    def elem_init( self, k ):
        self.techniques = self.elements[0].elements

#
# Functions
#

def PrintIndented( string, indentation ):
    for i in range( 0, indentation ):
        stdout.write( " " )
    for c in string:
        stdout.write( c )
        if c == "\n":
            for i in range( 0, indentation ):
                stdout.write( " " )
    stdout.write( "\n" )

def PrintElements( element, indentation = 0 ):
    PrintIndented( element.__repr__(), indentation )
    if not element:
        return
    #PrintIndented( "POSITION: " + str( element.start ) , indentation )
    for e in element.elements:
        PrintElements( e, indentation + 4 )

#
# JoeFx classes
#

def Parse( filename ):
    f = open( filename, "r" )
    string = f.read()

    parser = TranslationUnit.parser()
    result = parser.parse_string( string, reset = True, eof = True )

    if not result:
        print( "Failed to parse " + filename )
        return None

    return result

