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

# Get shaders
file(GLOB_RECURSE SHADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/**/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets/Shaders/**/*.frag"
)

# Compile shaders
set(COMPILED_SHADERS "")
foreach(SHADER ${SHADERS})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    get_filename_component(FILE_EXT ${SHADER} EXT)
    set(SPV_OUTPUT "${CMAKE_BINARY_DIR}/CompiledShaders/${FILE_NAME}.spv")

    if("${FILE_EXT}" STREQUAL ".vert")
        set(SHADER_STAGE "vertex")
    elseif("${FILE_EXT}" STREQUAL ".frag")
        set(SHADER_STAGE "fragment")
    else()
        message(FATAL_ERROR "Unknown shader stage for ${SHADER}")
    endif()

    add_custom_command(
        OUTPUT ${SPV_OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/CompiledShaders"
        COMMAND glslc -fshader-stage=${SHADER_STAGE} ${SHADER} -o ${SPV_OUTPUT}
        DEPENDS ${SHADER}
        COMMENT "Compiling ${SHADER} as ${SHADER_STAGE}"
    )

    list(APPEND COMPILED_SHADERS ${SPV_OUTPUT})
endforeach()

# Copy shaders
add_custom_target(CompileShaders ALL
    DEPENDS ${COMPILED_SHADERS}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/CompiledShaders"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders"
    # Only try to copy if source exists
    COMMAND ${CMAKE_COMMAND} -E echo "Copying compiled shaders to runtime directory..."
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_BINARY_DIR}/CompiledShaders"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders"
    COMMENT "Compiling shaders..."
)
# Copy config.ini
add_custom_command(TARGET SilverlightEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Assets/config.ini"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.ini"
    COMMENT "Copying config.ini to runtime directory"
)

# Shaders compile before Silverlight builds
add_dependencies(SilverlightEngine CompileShaders)
