cmake_minimum_required(VERSION 3.0)
project(WIREHEAD VERSION 2.0)


add_compile_options("-O3")
add_compile_options("-march=native")

find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

add_executable(WIREHEAD main.c io/stlloader.cpp)

# First try openxr.pc from OpenXR SDK
include(FindPkgConfig)
pkg_search_module(OPENXR openxr)
if (OPENXR_FOUND)
  MESSAGE("OpenXR found with pkg-config")
  target_link_libraries(WIREHEAD PRIVATE ${OPENXR_LIBRARIES})

# Second, try OpenXRConfig.cmake from OpenXR SDK
else()
  MESSAGE("OpenXR not found with pkg-config, trying cmake script")

  # current issue in upstream OpenXR cmake files requires us to find Threads on our own
  find_package(Threads REQUIRED)

  find_package(OpenXR REQUIRED)
  if (NOT OpenXR_FOUND)
     MESSAGE(FATAL_ERROR "OpenXR not found!")
  endif()

  target_include_directories(WIREHEAD PRIVATE ${OpenXR_INCLUDE_DIR})
  target_link_libraries(WIREHEAD PRIVATE OpenXR::openxr_loader)
endif()

target_link_libraries(WIREHEAD PRIVATE Xrandr ${X11_LIBRARIES} ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} m)
target_include_directories(WIREHEAD PRIVATE ${SDL2_INCLUDE_DIR})

if(MSVC)
  target_compile_options(WIREHEAD PRIVATE /W4 /WX)
else(MSVC)
  target_compile_options(WIREHEAD PRIVATE -pedantic -Wall -Wextra -Wno-unused-parameter)
endif(MSVC)


install(TARGETS WIREHEAD RUNTIME DESTINATION bin)
