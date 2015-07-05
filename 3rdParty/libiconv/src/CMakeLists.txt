#######################################################
## Source & Header
set (ICONV_SOURCES ../lib/iconv.c
                   ../lib/relocatable.c
                   ../libcharset/lib/localcharset.c
 )
                    

source_group("Sourcefiles" FILES ${ICONV_SOURCES})


#######################################################
## ANDROID ARM32
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL android_arm32)
  set(PREPROCESSOR_DEF -DGCC4 -D_ANDROID -DLIBDIR="c" -Wno-multichar -DBUILDING_LIBICONV -DIN_LIBRARY -DNO_XMALLOC)
  
  set(TARGET iconv)
endif (${TML_TARGET_PLATFORM} STREQUAL android_arm32)

#######################################################
## The target is a shared library 
ADD_LIBRARY( iconv SHARED ${ICONV_SOURCES})

INCLUDE_DIRECTORIES (.. )
INCLUDE_DIRECTORIES (../include)
INCLUDE_DIRECTORIES (../srclib)
INCLUDE_DIRECTORIES (../libcharset/include)

#######################################################
## Preprocessor- definitions:
ADD_DEFINITIONS( ${PREPROCESSOR_DEF})

