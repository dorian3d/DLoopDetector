cmake_minimum_required(VERSION 2.8)
project(DLoopDetector)

option(BUILD_DemoBRIEF  "Build demo application with BRIEF features" OFF)
option(BUILD_DemoSURF   "Build demo application with SURF features"  OFF)

set(HDRS
  include/DLoopDetector/DLoopDetector.h         include/DLoopDetector/TemplatedLoopDetector.h)

find_package(OpenCV REQUIRED)
find_package(DLib REQUIRED)
find_package(DBoW2 REQUIRED)

include_directories(include/DLoopDetector/ ${OpenCV_INCLUDE_DIRS} ${DLib_INCLUDE_DIRS} ${DBoW2_INCLUDE_DIRS})

if(BUILD_DemoBRIEF)
  add_executable(demo_brief demo/demo_brief.cpp)
  target_link_libraries(demo_brief ${OpenCV_LIBS} ${DLib_LIBS} ${DBoW2_LIBS})
endif(BUILD_DemoBRIEF)

if(BUILD_DemoSURF)
  add_executable(demo_surf demo/demo_surf.cpp)
  target_link_libraries(demo_surf ${OpenCV_LIBS} ${DLib_LIBS} ${DBoW2_LIBS})
endif(BUILD_DemoSURF)

if(BUILD_DemoBRIEF OR BUILD_DemoSURF)
  set(RESOURCE_FILE ${CMAKE_BINARY_DIR}/resources.tar.gz)
  if(NOT EXISTS ${CMAKE_BINARY_DIR}/resources/)
    if(NOT EXISTS ${RESOURCE_FILE})
      file(DOWNLOAD http://doriangalvez.com/resources/DLoopDetector/resources.tar.gz 
        ${RESOURCE_FILE} STATUS status SHOW_PROGRESS EXPECTED_MD5 c001da68ddaceed1de9c16aaac22ed80)
      if(${status})
        message(FATAL_ERROR "Error downloading resources for demo applications")
      endif(${status})
    endif(NOT EXISTS ${RESOURCE_FILE})
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${RESOURCE_FILE})
    file(REMOVE ${RESOURCE_FILE})
  endif(NOT EXISTS ${CMAKE_BINARY_DIR}/resources/)
endif(BUILD_DemoBRIEF OR BUILD_DemoSURF)

configure_file(src/DLoopDetector.cmake.in
  "${PROJECT_BINARY_DIR}/DLoopDetectorConfig.cmake" @ONLY)

install(DIRECTORY include/DLoopDetector DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/DLoopDetectorConfig.cmake"
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})
install(FILES "${PROJECT_BINARY_DIR}/DLoopDetectorConfig.cmake" 
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/DLoopDetector/)
