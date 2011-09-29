#
# CMakeLists.txt for joefx
# Adapted from glfw (http://glfw.org)
#

project(joefx)
cmake_minimum_required(VERSION 2.8)

include_directories(include)

set(joefx_SOURCES   ${joefx_SOURCE_DIR}/src/joefx.cpp
                    ${joefx_SOURCE_DIR}/include/joefx/joefx.hpp
                    src/context.cpp
                    include/joefx/context.hpp
                    src/effect.cpp
                    include/joefx/effect.hpp
                    src/pass.cpp
                    include/joefx/pass.hpp
                    src/technique.cpp
                    include/joefx/technique.hpp
                    src/state_assignment.cpp
                    include/joefx/state_assignment.hpp )

#
# Create shared configuration header
#

#configure_file(${joefx_SOURCE_DIR}/include/joefx/config.hpp.in 
               #${joefx_SOURCE_DIR}/include/joefx/config.hpp @ONLY)

configure_file(${joefx_SOURCE_DIR}/libjoefx.pc.cmake 
               ${joefx_BINARY_DIR}/libjoefx.pc @ONLY)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W ${CXX11_FLAGS}")

add_library(libjoefxStatic    STATIC    ${joefx_SOURCES})
add_library(libjoefxShared    SHARED    ${joefx_SOURCES})

set_target_properties(libjoefxStatic libjoefxShared PROPERTIES 
                      CLEAN_DIRECT_OUTPUT 1
                      OUTPUT_NAME joefx)

install(TARGETS libjoefxStatic libjoefxShared DESTINATION lib)
install(FILES ${joefx_BINARY_DIR}/libjoefx.pc DESTINATION lib/pkgconfig)
  
#
# Add example and test programs
#
add_subdirectory(tests)

#
# Install standard files 
#

install(DIRECTORY include/joefx DESTINATION include 
        FILES_MATCHING REGEX "(.*\\.hpp$|.*\\.inl$)")

#install(FILES license.txt readme.txt 
#        DESTINATION share/doc/joefx/)

#
# Uninstall operation
#
if( NOT TARGET uninstall )
    configure_file(${joefx_SOURCE_DIR}/cmake_uninstall.cmake.in
                   ${joefx_BINARY_DIR}/cmake_uninstall.cmake IMMEDIATE @ONLY)

    add_custom_target(uninstall
                      ${CMAKE_COMMAND} -P
                      ${joefx_BINARY_DIR}/cmake_uninstall.cmake)
endif()

