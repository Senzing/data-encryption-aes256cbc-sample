
set(ENCRYPT_DATA_CLEARTEXT_PLUGIN_SOURCE_FILES 
    EncryptDataClearTextPlugin.c )

set(ENCRYPT_DATA_REVERSETEXT_PLUGIN_SOURCE_FILES 
    EncryptDataReverseTextPlugin.c )

set(ENCRYPT_DATA_G2_AES_256_CBC_PLUGIN_SOURCE_FILES 
    EncryptDataAES256CBCPlugin.c )

add_library(g2EncryptDataClearText SHARED ${ENCRYPT_DATA_CLEARTEXT_PLUGIN_SOURCE_FILES})
add_library(g2EncryptDataReverseText SHARED ${ENCRYPT_DATA_REVERSETEXT_PLUGIN_SOURCE_FILES})
add_library(g2EncryptDataAES256CBC SHARED ${ENCRYPT_DATA_G2_AES_256_CBC_PLUGIN_SOURCE_FILES})

set_target_properties(g2EncryptDataClearText PROPERTIES LINK_FLAGS "${LINK_FLAGS}")
set_target_properties(g2EncryptDataReverseText PROPERTIES LINK_FLAGS "${LINK_FLAGS}")
set_target_properties(g2EncryptDataAES256CBC PROPERTIES LINK_FLAGS "${LINK_FLAGS}")

#if ( WIN32 )
#else ()
#  set_target_properties(g2EncryptDataClearText PROPERTIES SUFFIX ".so")
#  set_target_properties(g2EncryptDataReverseText PROPERTIES SUFFIX ".so")
#  set_target_properties(g2EncryptDataAES256CBC PROPERTIES SUFFIX ".so")
#endif ()

target_link_libraries(g2EncryptDataClearText ${OSCORELIBS} )
target_link_libraries(g2EncryptDataReverseText ${OSCORELIBS} )
target_link_libraries(g2EncryptDataAES256CBC ${OSCORELIBS} )

install(TARGETS g2EncryptDataClearText
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib )
install(TARGETS g2EncryptDataReverseText
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib )
install(TARGETS g2EncryptDataAES256CBC
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib )

