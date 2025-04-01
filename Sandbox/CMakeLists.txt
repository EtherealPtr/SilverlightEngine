project(Sandbox)

file(GLOB_RECURSE SANDBOX_SOURCES CONFIGURE_DEPENDS
    Source/*.cpp
    Source/*.h
)

add_executable(Sandbox ${SANDBOX_SOURCES})

target_include_directories(Sandbox
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

target_link_libraries(Sandbox
    PRIVATE SilverlightEngine
)

foreach(file IN LISTS SANDBOX_SOURCES)
    get_filename_component(dir "${file}" PATH)
    file(RELATIVE_PATH rel_dir "${CMAKE_CURRENT_SOURCE_DIR}/Source" "${dir}")
    string(REPLACE "/" "\\" group "${rel_dir}")
    source_group("${group}" FILES "${file}")
endforeach()
