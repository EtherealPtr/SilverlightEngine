project(SilverlightEngine)

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    Source/*.cpp
    Source/*.h
)

add_library(SilverlightEngine SHARED ${ENGINE_SOURCES})

target_include_directories(SilverlightEngine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glfw/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/vulkan/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/stb_image
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/tinygltf
)

if (MSVC)
    target_compile_options(SilverlightEngine PRIVATE /MP)
endif()

target_compile_definitions(SilverlightEngine
    PRIVATE SILVERLIGHT_EXPORTS
)

target_link_libraries(SilverlightEngine
    vulkan-1
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/glfw/lib/glfw3.lib
)

# Group source files neatly
foreach(file IN LISTS ENGINE_SOURCES)
    get_filename_component(dir "${file}" PATH)
    file(RELATIVE_PATH rel_dir "${CMAKE_CURRENT_SOURCE_DIR}/Source" "${dir}")
    string(REPLACE "/" "\\" group "${rel_dir}")
    source_group("${group}" FILES "${file}")
endforeach()

if (MSVC)
    add_custom_command(
        TARGET SilverlightEngine PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Running windows shader compiler..."
        COMMAND ${CMAKE_COMMAND} -E chdir "${CMAKE_SOURCE_DIR}/Tools" cmd /c CompileShaders_Windows.bat
        COMMENT "Running CompileShaders.bat before build"
    )
endif()

add_custom_command(TARGET SilverlightEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Assets/config.ini"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.ini"
    COMMENT "Copying config.ini to runtime directory"
)
