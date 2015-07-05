#######################################################
## Source & Header
set (AXL_SOURCES axl.c
        axl_decl.c
        axl_doc.c
        axl_dtd.c
        axl_error.c
        axl_factory.c
        axl_hash.c
        axl_list.c
        axl_log.c
        axl_node.c
        axl_stack.c
        axl_stream.c )
                    

source_group("Sourcefiles" FILES ${AXL_SOURCES})


#######################################################
## ANDROID ARM32
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL android_arm32)
  set(PREPROCESSOR_DEF -DGCC4)
  
  set(TARGET axl)
endif (${TML_TARGET_PLATFORM} STREQUAL android_arm32)

#######################################################
## The target is a shared library 
ADD_LIBRARY( axl SHARED ${AXL_SOURCES})

INCLUDE_DIRECTORIES ( . )

#######################################################
## Preprocessor- definitions:
ADD_DEFINITIONS( ${PREPROCESSOR_DEF})

