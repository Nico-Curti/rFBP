cmake_minimum_required (VERSION 3.8.2)

# rFBP Version
set (RFB_MAJOR    1)
set (RFB_MINOR    0)
set (RFB_REVISION 0)
set (RFB_VERSION ${RFB_MAJOR}.${RFB_MINOR}.${RFB_REVISION})

project (rFBP LANGUAGES CXX VERSION ${RFB_VERSION} DESCRIPTION "Replicated Focusing Belief Propagation")
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions (-DMAJOR=${RFB_MAJOR} -DMINOR=${MINOR} -DREVISION=${REVISION})

#################################################################
#                         COMPILE OPTIONS                       #
#################################################################

option (OMP     "Enable OpenMP                support" ON)
option (VERBOSE "Enable verbose stdout        support" ON)
option (STATS   "Enable verbose statistics    support" ON)

#################################################################
#                         SETTING VARIABLES                     #
#################################################################

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules/" ${CMAKE_MODULE_PATH})

if ( NOT APPLE )
  set (CMAKE_SKIP_BUILD_RPATH             FALSE )
  set (CMAKE_BUILD_WITH_INSTALL_RPATH     FALSE )
  set (CMAKE_INSTALL_RPATH_USE_LINK_PATH  TRUE  )
endif()

# make sure that the default is a RELEASE
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if ( CMAKE_COMPILER_IS_GNUCXX )
  if ( OMP )
    add_compile_options (-Wall -Wextra -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors)
  endif()
  string (REGEX REPLACE "-O3" "-Ofast" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  string (REGEX REPLACE "-O0" "-Og" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG" )
  list (APPEND linked_libs stdc++fs)
endif()


if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set (CMAKE_CXX_FLAGS "-Wno-deprecated -Wno-writable-strings ${CMAKE_CXX_FLAGS}")
  string (REGEX REPLACE "-O0" "-Og" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
  string (REGEX REPLACE "-O3" "-Ofast" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  list (APPEND linked_libs c++experimental)
endif()

if (MSVC)
  set (CMAKE_CXX_FLAGS "/wd4013 /wd4018 /wd4028 /wd4047 /wd4068 /wd4090 /wd4101 /wd4113 /wd4133 /wd4190 /wd4244 /wd4267 /wd4305 /wd4477 /wd4996 /wd4819 /fp:fast ${CMAKE_CXX_FLAGS}")
  string (REGEX REPLACE "/O2" "/Ox" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  add_definitions (-D_CRT_RAND_S)
  add_definitions (-DNOMINMAX)
  add_definitions (-D_USE_MATH_DEFINES)
  add_definitions (-D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
  set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if (COMPILER_SUPPORTS_MARCH_NATIVE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()


#################################################################
#                         PARSE OPTIONS                         #
#################################################################

if (OMP)
  find_package(OpenMP REQUIRED)
  if (OPENMP_FOUND)
    message(STATUS "OpenMP found")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    if (APPLE)
      list(APPEND linked_libs OpenMP::OpenMP_CXX)
    endif()
  endif()
else()
  message(STATUS "OpenMP disabled")
endif()

if (VERBOSE)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVERBOSE" )
  add_definitions (-DVERBOSE)
  message (STATUS "Verbose state: ON")
else()
  message (STATUS "Verbose disabled")
endif()

if (STATS)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTATS" )
  add_definitions (-DSTATS)
  message (STATUS "Statistics print state: ON")
else()
  message (STATUS "Statistics print disabled")
endif()

#################################################################
#                         SETTING DIRECTORIES                   #
#################################################################

set(SRC_DIR  ${CMAKE_SOURCE_DIR}/src                                 CACHE PATH "Path where find cpp files"                 )
set(INC_DIR  ${CMAKE_SOURCE_DIR}/include                             CACHE PATH "Path where find header files"              )
set(TST      ${CMAKE_SOURCE_DIR}/tst                                 CACHE PATH "Path where find test files"                )
set(PYC_DIR  ${CMAKE_SOURCE_DIR}/ReplicatedFocusingBeliefPropagation CACHE PATH "Path where find cython files"              )         # cython directory
set(OUT_DIR  ${CMAKE_SOURCE_DIR}/bin                                 CACHE PATH "Path where outputs will be installed" FORCE)

set(rfbplib rfbp)
set(rfbpmain main)

file(GLOB SRC    "${SRC_DIR}/*.cpp")
file(GLOB HEADER "${INC_DIR}/*.h"  )
file(GLOB EXE    "${TST}/*.cpp")

include_directories(${INC_DIR})

# Make the scripts available in the 'cmake' directory available for the
# 'include()' command, 'find_package()' command.
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)

# Include the CMake script UseCython.cmake.  This defines add_cython_module().
# Instruction for use can be found at the top of cmake/UseCython.cmake.
include( UseCython )

# With CMake, a clean separation can be made between the source tree and the
# build tree.  When all source is compiled, as with pure C/C++, the source is
# no-longer needed in the build tree.  However, with pure *.py source, the
# source is processed directly.  To handle this, we reproduce the availability
# of the source files in the build tree.

#################################################################
#                          SUMMARY                              #
#################################################################

message(STATUS ""                                                                   )
message(STATUS "=================== rFBP configuration Summary =================="  )
message(STATUS "   rFBP version: ${MAJOR}.${MINOR}.${REVISION}"                     )
message(STATUS ""                                                                   )
message(STATUS "   C++ :"                                                           )
message(STATUS "      C++ Compiler : ${CMAKE_CXX_COMPILER}"                         )
message(STATUS "      C++ flags    :"                                               )
foreach(FLAG ${CMAKE_CXX_FLAGS})
  message(STATUS "                    * ${FLAG}"                                    )
endforeach(FLAG)
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  foreach(FLAG ${CMAKE_CXX_FLAGS_RELEASE})
    message(STATUS "                    * ${FLAG}"                                  )
  endforeach(FLAG)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  foreach(FLAG ${CMAKE_CXX_FLAGS_DEBUG})
    message(STATUS "                    * ${FLAG}"                                  )
  endforeach(FLAG)
endif()
message(STATUS "      Linker flags : "                                              )
foreach(FLAG ${linked_libs})
  message(STATUS "                    * ${FLAG}"                                    )
endforeach(FLAG)
message(STATUS ""                                                                   )
message(STATUS "   OpenMP support : ${OMP}"                                         )
message(STATUS "   VERBOSE level  : ${VERBOSE}"                                     )
message(STATUS "   STATS   level  : ${STATS}"                                       )
message(STATUS ""                                                                   )

#################################################################
#                         MAIN RULES                            #
#################################################################

add_subdirectory(${PYC_DIR})

add_library(${rfbplib} SHARED ${SRC} ${HEADER})
set_property(TARGET ${rfbplib} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(${rfbplib} ${linked_libs})

add_executable(${rfbpmain} ${EXE})
target_link_libraries(${rfbpmain} ${linked_libs} ${rfbplib})

#################################################################
#                          INSTALLERS                           #
#################################################################

install(TARGETS ${rfbplib}         DESTINATION ${CMAKE_SOURCE_DIR})
install(TARGETS ${rfbpmain}        DESTINATION ${OUT_DIR})
