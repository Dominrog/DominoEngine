cmake_minimum_required(VERSION 3.20)
project(demo LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(EXTERN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern")
set(IMGUI_DIR "${EXTERN_DIR}/imgui")
set(LUA_DIR "${EXTERN_DIR}/lua-5.4.8/src")
set(JOLT_DIR "${EXTERN_DIR}/JoltPhysics-5.6.0")

# --------------------------------------------------
# Jolt
# --------------------------------------------------

set(OVERRIDE_CXX_FLAGS OFF CACHE BOOL "" FORCE)
set(INTERPROCEDURAL_OPTIMIZATION OFF CACHE BOOL "" FORCE)
set(ENABLE_ALL_WARNINGS OFF CACHE BOOL "" FORCE)
set(ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(JPH_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(JPH_USE_DX12 OFF CACHE BOOL "" FORCE)
set(JPH_USE_VK OFF CACHE BOOL "" FORCE)
set(JPH_USE_MTL OFF CACHE BOOL "" FORCE)

add_subdirectory(
    "${JOLT_DIR}/build"
    "${CMAKE_CURRENT_BINARY_DIR}/jolt"
)

# --------------------------------------------------
# Lua
# --------------------------------------------------

file(GLOB LUA_SOURCES CONFIGURE_DEPENDS
    "${LUA_DIR}/*.c"
)

list(REMOVE_ITEM LUA_SOURCES
    "${LUA_DIR}/lua.c"
    "${LUA_DIR}/luac.c"
)

add_library(lua STATIC ${LUA_SOURCES})
target_include_directories(lua PUBLIC "${LUA_DIR}")

if(UNIX)
    target_compile_definitions(lua PRIVATE LUA_USE_POSIX)
    target_link_libraries(lua PUBLIC m ${CMAKE_DL_LIBS})
endif()

# --------------------------------------------------
# Engine & ImGui
# --------------------------------------------------

set(SRC_FILES
    src/main.cpp
    src/engine.cpp
    src/glad.c
    src/utils.cpp
    src/ShaderObject.cpp
    src/LuaBindings.cpp
    src/tinygltf_impl.cpp
    src/MeshLoader.cpp

    ecs/systems/CameraSystem.cpp
    ecs/systems/InputSystem.cpp
    ecs/systems/RenderSystem.cpp
    ecs/systems/TransformSystem.cpp
    ecs/systems/ScriptSystem.cpp
    ecs/systems/PhysicsSystem.cpp

    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
)

file(GLOB SCRIPT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/ecs/scripts/*.cpp"
)

add_executable(demo ${SRC_FILES} ${SCRIPT_SOURCES})

target_include_directories(demo PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/ecs"
    "${CMAKE_CURRENT_SOURCE_DIR}/ecs/systems"
    "${CMAKE_CURRENT_SOURCE_DIR}/ecs/components"
    "${CMAKE_CURRENT_SOURCE_DIR}/ecs/scripts"
    "${EXTERN_DIR}/glm-1.0.1"
    "${IMGUI_DIR}"
    "${IMGUI_DIR}/backends"
)

target_link_libraries(demo PRIVATE
    lua
    Jolt
)

# --------------------------------------------------

if(WIN32)
    set(GLFW_DIR "${EXTERN_DIR}/glfw-3.4")
    set(GLFW_DLL "${GLFW_DIR}/build/src/glfw3.dll")

    add_library(glfw_external SHARED IMPORTED)

    set_target_properties(glfw_external PROPERTIES
        IMPORTED_IMPLIB
            "${GLFW_DIR}/build/src/libglfw3dll.a"
        IMPORTED_LOCATION
            "${GLFW_DLL}"
        INTERFACE_INCLUDE_DIRECTORIES
            "${GLFW_DIR}/include"
    )

    target_link_libraries(demo PRIVATE
        glfw_external
        opengl32
        gdi32
    )

    add_custom_command(TARGET demo POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${GLFW_DLL}"
            "$<TARGET_FILE_DIR:demo>"
        VERBATIM
    )

elseif(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(PkgConfig REQUIRED)
    find_package(Threads REQUIRED)
    find_package(Freetype REQUIRED)

    pkg_check_modules(GLFW REQUIRED IMPORTED_TARGET glfw3)

    target_link_libraries(demo PRIVATE
        OpenGL::GL
        PkgConfig::GLFW
        Freetype::Freetype
        Threads::Threads
        ${CMAKE_DL_LIBS}
    )
endif()