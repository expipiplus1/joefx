include_directories( ${joefx_SOURCE_DIR}/include )

add_executable( test   main.cpp )

target_link_libraries( test libjoefxStatic )

