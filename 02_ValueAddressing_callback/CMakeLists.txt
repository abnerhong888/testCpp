cmake_minimum_required(VERSION 4.0.1)

set(PROJECTNAME "a.out")
set(INC_DIR "./")
set(SRC_DIR "./")

set(SRCS)
    
project(${PROJECTNAME})

aux_source_directory(${SRC_DIR} SRCS)

add_executable(${PROJECTNAME})

target_include_directories( 
    ${PROJECTNAME} 
    PUBLIC ${INC_DIR} 
)

target_sources( 
    ${PROJECTNAME} 
    PUBLIC ${SRCS}
)

target_compile_definitions( ${PROJECTNAME}  PUBLIC
    # "_CRT_SECURE_NO_WARNINGS"
    PUBLIC
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
    $<$<CONFIG:RelWithDebInfo>:DEBUG>
    $<$<CONFIG:MinSizeRel>:RELEASE>
)


