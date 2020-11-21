set(LIBRARY_OUTPUT_NAME "craudio")

set(CRaudio_SRCS
    craudio.c
)

set(CRaudio_HDRS
    ../include/craudio.h
)

set(CRaudio_LIBS
    ogg
    vorbis
    vorbisenc
    vorbisfile
    modplug
)

set(OUTPUT_DIR CRaudio)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT_DIR})
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/include)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/lib)
foreach(INC_FILE ${CRaudio_HDRS})
    get_filename_component(BASE ${INC_FILE} NAME)
    configure_file(${INC_FILE} ${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/include/${BASE} COPYONLY)
endforeach(INC_FILE)

add_library(
    CRaudio_shared SHARED
    ${CRaudio_HDRS}
    ${CRaudio_SRCS}
)

if(WIN32)
    target_link_libraries(
        CRaudio_shared
        ${CRaudio_LIBS}
    )
else(WIN32)
    target_link_libraries(
        CRaudio_shared
        -Wl,--whole-archive
        ${CRaudio_LIBS}
        -Wl,--no-whole-archive
    )
endif(WIN32)
    
set_target_properties(CRaudio_shared PROPERTIES
    OUTPUT_NAME ${LIBRARY_OUTPUT_NAME}
    CLEAN_DIRECT_OUTPUT 1
    VERSION "1.0"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/bin"
)

set_target_properties(CRaudio_shared PROPERTIES LINKER_LANGUAGE C)
