prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include

Name: joefx
Description: Fx for opengl
Version: 0.0.1
URL: https://github.com/expipiplus1/joefx
Libs: -L${libdir} -ljoefx
Cflags: -I${includedir}
