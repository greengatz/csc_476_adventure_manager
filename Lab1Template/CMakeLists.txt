cmake_minimum_required(VERSION 2.8)
project(mercenaryManager)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/source")
set(DEP_DIR "${PROJECT_SOURCE_DIR}/dependencies")

# Source / header files
include("sources.txt")

#fmod
set(FMOD_DIR "${DEP_DIR}/fmod_api")
set(FMOD_DIR_MAC "${DEP_DIR}/fmod_mac")
if(APPLE)
    #include apple fmod library
    list(APPEND INCLUDES
      ${FMOD_DIR_MAC}/api/inc
   )
   list(APPEND LIBS
      ${FMOD_DIR_MAC}/api/lib/libfmodex.dylib)
elseif(UNIX)
    list(APPEND INCLUDES
        ${FMOD_DIR}/api/inc
    )
    list(APPEND LIBS
        ${FMOD_DIR}/api/lib/libfmodex64-4.44.52.so
    )
endif(APPLE)

# GLFW
set(GLFW_DIR "${DEP_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
add_subdirectory(${GLFW_DIR})
list(APPEND INCLUDES
   ${GLFW_DIR}/include
)
list(APPEND LIBS
   glfw ${GLFW_LIBRARIES}
)

# glad
set(GLAD_DIR "${DEP_DIR}/glad")
list(APPEND LIB_SOURCES
   ${GLAD_DIR}/src/glad.c
)
list(APPEND INCLUDES
   ${GLAD_DIR}/include
)

# dl libs
list(APPEND LIBS
   ${CMAKE_DL_LIBS}
)

foreach(HEADER ${HEADERS})
   list(APPEND ABS_HEADERS
      "${SRC_DIR}/${HEADER}"
   )
endforeach(HEADER)

foreach(SOURCE ${SOURCES})
   list(APPEND ABS_SOURCES
      "${SRC_DIR}/${SOURCE}"
   )
endforeach(SOURCE)

#add_definitions(-std=c++11)

include_directories(${INCLUDES})
add_executable(${PROJECT_NAME} ${ABS_SOURCES} ${ABS_HEADERS} ${LIB_SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
