cmake_minimum_required(VERSION 2.8)
project(DLoopDetector)
include(ExternalProject)

option(BUILD_DemoBRIEF  "Build demo application with BRIEF features" OFF)
option(BUILD_DemoSURF   "Build demo application with SURF features"  OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
endif()

set(HDRS
  include/DLoopDetector/DLoopDetector.h         include/DLoopDetector/TemplatedLoopDetector.h)

set(DEPENDENCY_DIR ${CMAKE_CURRENT_BINARY_DIR}/dependencies)
set(DEPENDENCY_INSTALL_DIR ${DEPENDENCY_DIR}/install)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

macro(GetDependency name other_dependency)
  find_package(${name} QUIET 
    PATHS ${DEPENDENCY_INSTALL_DIR})
  if(${${name}_FOUND})
    message("${name} library found, using it from the system")
    include_directories(${${name}_INCLUDE_DIRS})
    add_custom_target(${name})
  else(${${name}_FOUND})
    message("${name} library not found in the system, it will be downloaded on build")
    option(DOWNLOAD_${name}_dependency "Download ${name} dependency" ON)
    if(${DOWNLOAD_${name}_dependency})
      ExternalProject_Add(${name}
        PREFIX ${DEPENDENCY_DIR}
        GIT_REPOSITORY http://github.com/dorian3d/${name}
        INSTALL_DIR ${DEPENDENCY_INSTALL_DIR}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
        DEPENDS ${other_dependency})
    else()
      message(SEND_ERROR "Please, activate DOWNLOAD_${name}_dependency option or download manually")
    endif(${DOWNLOAD_${name}_dependency})
  endif(${${name}_FOUND})
endmacro(GetDependency)

GetDependency(DLib "")
GetDependency(DBoW2 DLib)
add_custom_target(Dependencies ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} DEPENDS DBoW2 DLib)

include_directories(include/DLoopDetector/)

if(BUILD_DemoBRIEF)
  add_executable(demo_brief demo/demo_brief.cpp)
  target_link_libraries(demo_brief ${OpenCV_LIBS} ${DLib_LIBS} ${DBoW2_LIBS})
endif(BUILD_DemoBRIEF)

if(BUILD_DemoSURF)
  add_executable(demo_surf demo/demo_surf.cpp)
  target_link_libraries(demo_surf ${OpenCV_LIBS} ${DLib_LIBS} ${DBoW2_LIBS})
endif(BUILD_DemoSURF)

if(BUILD_DemoBRIEF OR BUILD_DemoSURF)
  option(DOWNLOAD_Demo_Resources "Download resources for demo application" ON)
  if(DOWNLOAD_Demo_Resources)
    set(RESOURCES_DIR ${CMAKE_CURRENT_BINARY_DIR}/resources/)
    if(NOT EXISTS ${RESOURCES_DIR})
        ExternalProject_Add(Resources
          PREFIX ${CMAKE_BINARY_DIR}
          URL http://doriangalvez.com/resources/DLoopDetector/resources.tar.gz
          URL_MD5 c001da68ddaceed1de9c16aaac22ed80
          SOURCE_DIR ${RESOURCES_DIR}
          CONFIGURE_COMMAND ""
          BUILD_COMMAND ""
          INSTALL_COMMAND "")
        add_custom_target(DownloadResources ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} DEPENDS Resources)
    endif(NOT EXISTS ${RESOURCES_DIR})
  else()
    message("Demo resources are not going to be downloaded unless the option is activated. Demo applications will not work")
  endif(DOWNLOAD_Demo_Resources)
endif(BUILD_DemoBRIEF OR BUILD_DemoSURF)

configure_file(src/DLoopDetector.cmake.in
  "${PROJECT_BINARY_DIR}/DLoopDetectorConfig.cmake" @ONLY)

install(DIRECTORY include/DLoopDetector DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/DLoopDetectorConfig.cmake"
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
install(FILES "${PROJECT_BINARY_DIR}/DLoopDetectorConfig.cmake" 
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/DLoopDetector/)
install(DIRECTORY ${DEPENDENCY_INSTALL_DIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX} OPTIONAL)
