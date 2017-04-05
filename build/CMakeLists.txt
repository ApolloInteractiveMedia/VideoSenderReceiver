cmake_minimum_required(VERSION 3.7)
include(ExternalProject)
project(VideoSenderReceiver)

set(bd ${CMAKE_CURRENT_LIST_DIR}/..)
set(ed ${bd}/examples)
set(vsr_name "VideoSenderReceiver")
set(vsr_version "v1")
set(vsr_lib_name "${vsr_name}-${COMPILER}-${CMAKE_BUILD_TYPE}-${vsr_version}")

# Glad
# ---------------------------------------------
ExternalProject_Add(
  glad
  GIT_REPOSITORY  https://github.com/Dav1dde/glad.git
  GIT_SHALLOW 1
  UPDATE_COMMAND ""
  CMAKE_ARGS -DGLAD_INSTALL=On -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
  )

ExternalProject_Get_Property(glad install_dir)
include_directories(${CMAKE_INSTALL_PREFIX}/include)

if (UNIX)
  list(APPEND libs ${CMAKE_INSTALL_PREFIX}/lib/libglad.a)
elseif(WIN32)
  list(APPEND libs ${CMAKE_INSTALL_PREFIX}/lib/libglad.lib)
endif()

# GLFW
# ---------------------------------------------
ExternalProject_Add(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_SHALLOW 1
  UPDATE_COMMAND ""
  CMAKE_ARGS -DGLFW_BUILD_TESTS=Off -DGLFW_BUILD_DOCS=Off -DGLFW_BUILD_EXAMPLES=Off -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> 
  )

ExternalProject_Get_Property(glfw install_dir)
include_directories(${install_dir}/include)

if (UNIX)
  list(APPEND libs ${install_dir}/lib/libglfw3.a)
elseif(WIN32)
  list(APPEND libs ${install_dir}/lib/glfw3.lib)
endif()

# Opus
# ---------------------------------------------
if (APPLE)
  ExternalProject_Add(
    opus
    URL http://downloads.xiph.org/releases/opus/opus-1.1.4.tar.gz
    UPDATE_COMMAND ""
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./configure --prefix=<INSTALL_DIR> --enable-custom-modes
    )
elseif(WIN32)  
  ExternalProject_Add(
    opus
    GIT_REPOSITORY https://github.com/roxlu/opus.git
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
    )
endif()

ExternalProject_Get_Property(opus install_dir)
include_directories(${install_dir}/include)

if (APPLE)
  list(APPEND libs ${install_dir}/lib/libopus.a)
elseif(WIN32)
  list(APPEND libs ${install_dir}/lib/opus.lib)
endif()

# VideoSenderReceiver
# ---------------------------------------------
ExternalProject_Add(
  vsr
  URL https://github.com/ApolloInteractiveMedia/VideoSenderReceiver/releases/download/v1.0/VideoSenderReceiver-v1.zip
  CONFIGURE_COMMAND ""
  UPDATE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  )

ExternalProject_Get_Property(vsr source_dir)
include_directories(${source_dir}/include)
link_directories(${source_dir}/lib)

if (APPLE)
  list(APPEND libs "${vsr_lib_name}.a")
elseif(WIN32)
endif()

# OS dependencies
# ---------------------------------------------
if (APPLE)
    
  find_library(fr_corefoundation CoreFoundation)
  find_library(fr_cocoa Cocoa)
  find_library(fr_iokit IOKit)
  find_library(fr_coreaudio CoreAudio)
  find_library(fr_audiounit AudioUnit)
  find_library(fr_audiotb AudioToolbox)
  find_library(fr_coremedia CoreMedia)
  find_library(fr_corevideo CoreVideo)
  find_library(fr_videokit VideoToolbox)
  find_library(fr_vda VideoDecodeAcceleration)
  
  list(APPEND libs
    ${fr_corefoundation}
    ${fr_iokit}
    ${fr_cocoa}
    ${fr_coreaudio}
    ${fr_audiounit}                       
    ${fr_audiotb}
    ${fr_coremedia}
    ${fr_corevideo}
    ${fr_videokit}
    ${fr_vda}
    ${fr_security}
    z
    iconv
    bz2
    )

elseif(WIN32)
endif()

# Project
# ---------------------------------------------
include_directories(
  ${bd}/include
  ${ed}
  )

macro(create_example name)
  set(example_name "example_${name}${debug_flag}")
  add_executable(${example_name} ${ed}/example_${name}.cpp)
  target_link_libraries(${example_name} ${vsr_lib_name} ${libs})
  add_dependencies(${example_name} glad glfw vsr opus)
  install(TARGETS ${example_name} DESTINATION bin/)
endmacro()

create_example("sender")
create_example("receiver")