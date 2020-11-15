set(SHARED_OUTPUT "craudio")
set(STATIC_OUTPUT "craudio")
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
)
    
if(WIN32)
    set (STATIC_OUTPUT ${LIBRARY_OUTPUT_NAME}_s)
else(WIN32)
    set (STATIC_OUTPUT ${LIBRARY_OUTPUT_NAME})
endif(WIN32)

if(MSVC)
    set(OUTPUT_DIR CRaudio-VS)
elseif(MINGW)
    set(OUTPUT_DIR CRaudio-MINGW)
else()
    set(OUTPUT_DIR CRaudio)
endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT_DIR})
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/include)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/lib)
foreach(INC_FILE ${CRaudio_HDRS})
    get_filename_component(BASE ${INC_FILE} NAME)
    configure_file(${INC_FILE} ${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/include/${BASE} COPYONLY)
endforeach(INC_FILE)

if(BUILD_SHARED)
    add_library(
        CRaudio_shared SHARED
        ${CRaudio_HDRS}
        ${CRaudio_SRCS}
    )
    target_link_libraries(
        CRaudio_shared
        ${CRaudio_LIBS}
    )
    
    set_target_properties(CRaudio_shared PROPERTIES
        OUTPUT_NAME ${SHARED_OUTPUT}
        CLEAN_DIRECT_OUTPUT 1
        VERSION "1.0"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/bin"
    )

    set_target_properties(CRaudio_shared PROPERTIES LINKER_LANGUAGE C)
endif(BUILD_SHARED)