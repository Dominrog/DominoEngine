cmake_minimum_required(VERSION 3.15)
Project(demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/extern/imgui)

set(SRC_FILES
	src/main.cpp
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
	${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
)

file(GLOB SCRIPT_SOURCES
  ecs/scripts/*.cpp
)
list(APPEND SRC_FILES ${SCRIPT_SOURCES})

file(GLOB LUA_SOURCES ${CMAKE_SOURCE_DIR}/extern/lua-5.4.8/src/*.c)
list(FILTER LUA_SOURCES EXCLUDE REGEX ".*lua\\.c$")
list(FILTER LUA_SOURCES EXCLUDE REGEX ".*luac\\.c$")

add_library(lua STATIC ${LUA_SOURCES})


file(GLOB HEADER_FILES
	include/*.h
	include/*.hpp
	ecs/*.h
	ecs/systems/*.h
	ecs/components/*.h
	ecs/scripts/*.h
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

target_include_directories(lua PUBLIC ${CMAKE_SOURCE_DIR}/extern/lua-5.4.8/src)
add_dependencies(${PROJECT_NAME} lua)

target_link_libraries(${PROJECT_NAME} PRIVATE lua)

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/extern/glm-1.0.1
	${CMAKE_SOURCE_DIR}/extern/lua-5.4.8/src
	${CMAKE_SOURCE_DIR}/ecs
	${CMAKE_SOURCE_DIR}/ecs/systems
	${CMAKE_SOURCE_DIR}/ecs/components
	${CMAKE_SOURCE_DIR}/ecs/scripts
	${IMGUI_DIR}
  ${IMGUI_DIR}/backends 
)

if(WIN32)
	set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/extern/glfw-3.4/include")
 	set(GLFW_LIBRARY "${CMAKE_SOURCE_DIR}/extern/glfw-3.4/build/src/libglfw3dll.a")
  	set(GLFW_DLL "${CMAKE_SOURCE_DIR}/extern/glfw-3.4/build/src/glfw3.dll")

  	target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIR})
  	target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARY} opengl32 gdi32)

  	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different
    	${GLFW_DLL}
    	$<TARGET_FILE_DIR:${PROJECT_NAME}>
  	)
elseif(UNIX)
	find_package(OpenGL REQUIRED)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GLFW REQUIRED glfw3)
	find_package(Freetype REQUIRED)

	target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL ${GLFW_LIBRARIES} ${FREETYPE_LIBRARIES} dl pthread X11)
endif()
