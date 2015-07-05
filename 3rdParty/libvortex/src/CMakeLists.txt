#######################################################
## Source & Header
set (VORTEX_SOURCES vortex.c
        vortex_channel.c
        vortex_channel_pool.c
        vortex_connection.c
        vortex_ctx.c
        vortex_dtds.c
        vortex_errno.c
        vortex_frame_factory.c
        vortex_greetings.c
        vortex_handlers.c
        vortex_hash.c
        vortex_io.c
        vortex_listener.c
        vortex_payload_feeder.c
        vortex_profiles.c
        vortex_queue.c
        vortex_reader.c
        vortex_sequencer.c
        vortex_support.c
        vortex_thread.c
        vortex_thread_pool.c
        vortex_win32.c )
                    

source_group("Sourcefiles" FILES ${VORTEX_SOURCES})


#######################################################
## ANDROID ARM32
#######################################################
if (${TML_TARGET_PLATFORM} STREQUAL android_arm32)
  set(AXLLIB libaxl.so)
  set(PREPROCESSOR_DEF -DGCC4 -DPACKAGE_DTD_DIR="/usr/local/share" -DPACKAGE_TOP_DIR="..")
  
  set(TARGET vortex-1.1)
endif (${TML_TARGET_PLATFORM} STREQUAL android_arm32)

#######################################################
## The target is a shared library 
ADD_LIBRARY( vortex-1.1 SHARED ${VORTEX_SOURCES})
#######################################################
## AXL library 
TARGET_LINK_LIBRARIES(vortex-1.1 ${AXLLIB} )    


INCLUDE_DIRECTORIES ( . )
INCLUDE_DIRECTORIES (${AXLINCLUDE})

#######################################################
## Preprocessor- definitions:
ADD_DEFINITIONS( ${PREPROCESSOR_DEF})

