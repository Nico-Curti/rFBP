cmake_minimum_required (VERSION 3.8.2)
project (rFBP LANGUAGES CXX VERSION 2.0.1 DESCRIPTION "Replicated Focusing Belief Propagation")
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Byron Version
set (MAJOR    1)
set (MINOR    0)
set (REVISION 0)
add_definitions (-DMAJOR=${MAJOR} -DMINOR=${MINOR} -DREVISION=${REVISION})

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


if ( CMAKE_COMPILER_IS_GNUCC )
  add_compile_options (-Wall -Wextra -Wno-unused-result)
  string (REGEX REPLACE "-O3" "-Ofast" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
endif()
if ( MSVC )
  add_compile_options (/wd4028)
  add_compile_options (/wd4244)
  add_compile_options (/wd4267)
  add_compile_options (/wd4305)
  add_compile_options (/wd4477)
  add_compile_options (/wd4996)
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRTD")
  #set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:MSVCRT")
  set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
  list (APPEND linked_libs stdc++fs)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  list (APPEND linked_libs c++experimental)
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
  add_definitions (-DSTDOUT)
  message (STATUS "Verbose state: ON")
else()
  message (STATUS "Verbose disabled")
endif()

if (STATS)
  add_definitions (-DSTATS)
  message (STATUS "Statistics print state: ON")
else()
  message (STATUS "Statistics print disabled")
endif()

if (MSVC)
  add_definitions (-D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
endif()

#################################################################
#                         SETTING DIRECTORIES                   #
#################################################################

set(SRC_DIR    ./src)
set(INC_DIR    ./include)
set(EXAMPLE    ./example)
set(OBJ_DIR    ./obj)
set(OUT_DIR    ./bin)
set(DEP_DIR    ./.dep)

set(rfbplib rfbp)

file(GLOB SRC    "${SRC_DIR}/*.cpp")
file(GLOB HEADER "${INC_DIR}/*.h"  )
file(GLOB EXE    "${EXAMPLE}/*.cpp")

include_directories(${INC_DIR})

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
foreach(FLAG ${CMAKE_CXX_FLAGS_RELEASE})
  message(STATUS "                    * ${FLAG}"                                    )
endforeach(FLAG)
message(STATUS "      Linker flags : "                                              )
foreach(FLAG ${linked_libs})
  message(STATUS "                    * ${FLAG}"                                    )
endforeach(FLAG)
message(STATUS ""                                                                   )
message(STATUS "   OpenMP support : ${OMP}"                                         )
message(STATUS "   VERBOSE level  : ${VERBOSE}"                                     )
message(STATUS "   STATIS  level  : ${STATS}"                                       )
message(STATUS ""                                                                   )

#################################################################
#                         MAIN RULES                            #
#################################################################

add_library(${rfbplib} SHARED ${SRC} ${HEADER})
set_property(TARGET ${rfbplib} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(${rfbplib} ${linked_libs})

#################################################################
#                          INSTALLERS                           #
#################################################################

install(TARGETS ${rfbplib}         DESTINATION ${CMAKE_SOURCE_DIR})
