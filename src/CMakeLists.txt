enable_language(C)

add_library(${TARGET} STATIC
		../include/rfc7748_precomputed.h
		../include/fp25519_x64.h
		../include/fp448_x64.h
		fp25519_x64.c x25519_x64.c
		fp448_x64.c x448_x64.c)

INSTALL(TARGETS ${TARGET} DESTINATION "lib")
INSTALL(FILES
		../include/${TARGET}.h
		../include/fp25519_x64.h
		../include/fp448_x64.h
		DESTINATION "include" )
