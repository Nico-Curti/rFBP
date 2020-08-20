cmake_minimum_required (VERSION 3.9.0)

# rFBP Version
set (RFBP_MAJOR    1)
set (RFBP_MINOR    0)
set (RFBP_REVISION 1)
set (RFBP_VERSION ${RFBP_MAJOR}.${RFBP_MINOR}.${RFBP_REVISION})

project (rFBP LANGUAGES CXX VERSION ${RFBP_VERSION} DESCRIPTION "Replicated Focusing Belief Propagation")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5")
  set (CMAKE_CXX_STANDARD 14)
  set (CMAKE_CXX_STANDARD_REQUIRED ON)
else()
  set (CMAKE_CXX_STANDARD 17)
  set (CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

add_definitions (-DMAJOR=${RFBP_MAJOR} -DMINOR=${RFBP_MINOR} -DREVISION=${RFBP_REVISION})
add_definitions (-DPWD="${CMAKE_SOURCE_DIR}")

#################################################################
#                         COMPILE OPTIONS                       #
#################################################################

option (OMP        "Enable OpenMP                support" OFF)
option (SCORER     "Enable scorer                support" OFF)
option (VERBOSE    "Enable verbose stdout        support" ON)
option (STATS      "Enable verbose statistics    support" ON)
option (BUILD_TEST "Enable tests build           support" ON)
option (PYWRAP     "Enable Python wrap compilation      " OFF)

#################################################################
#                         SETTING VARIABLES                     #
#################################################################

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/" ${CMAKE_MODULE_PATH})

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
  add_compile_options (-Wall -Wextra -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -Wno-narrowing -Wpedantic)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    string (REGEX REPLACE "-O[0123]" "-Og" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG" )
    set (CMAKE_CXX_FLAGS_RELEASE "")
  elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    string (REGEX REPLACE "-O3" "-Ofast" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG" )
    set (CMAKE_CXX_FLAGS_DEBUG "")
  endif()
  #list (APPEND linked_libs stdc++fs)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  add_compile_options(-Wno-deprecated -Wno-writable-strings)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    string (REGEX REPLACE "-O0" "-Og" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG" )
  elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    string (REGEX REPLACE "-O3" "-Ofast" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG" )
  endif()
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

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}" CACHE PATH "Install prefix" FORCE)
endif()

#################################################################
#                         PARSE OPTIONS                         #
#################################################################

if (OMP)
  find_package(OpenMP REQUIRED)
  if (OPENMP_FOUND)
    message(STATUS "OpenMP found")
    if (OpenMP_CXX_VERSION_MAJOR LESS_EQUAL 4 AND OpenMP_CXX_VERSION_MINOR LESS 5)
      message(FATAL_ERROR " Your OpenMP is too old. Required OpenMP 4.5. Please upgrade.")
    endif()
    message(STATUS "OpenMP found")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    if (APPLE)
      list(APPEND linked_libs OpenMP::OpenMP_CXX)
    endif()
  endif()
else()
  message(STATUS "OpenMP disabled")
endif()

if (SCORER)
  find_package(Scorer REQUIRED)
  include_directories (${Scorer_INCLUDE_DIR})
  list(APPEND linked_libs Scorer::scorer)
  add_definitions (-DWITH_SCORER)
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

set(SRC_DIR  ${CMAKE_SOURCE_DIR}/src                                 CACHE PATH "Path where find cpp files"                        )
set(INC_DIR  ${CMAKE_SOURCE_DIR}/include                             CACHE PATH "Path where find header files"                     )
set(HPP_DIR  ${CMAKE_SOURCE_DIR}/hpp                                 CACHE PATH "Path where find hpp files"                        )
set(EXAMPLE  ${CMAKE_SOURCE_DIR}/example                             CACHE PATH "Path where find main files"                       )
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test                                CACHE PATH "Path where find test files"                       )
set(PYC_DIR  ${CMAKE_SOURCE_DIR}/ReplicatedFocusingBeliefPropagation CACHE PATH "Path where find cython files"                     )         # cython directory
set(LIB_DIR  ${CMAKE_SOURCE_DIR}/lib                                 CACHE PATH "Path where lib will be installed"            FORCE)
set(INST_DIR ${CMAKE_SOURCE_DIR}/share/rfbp                          CACHE PATH "Path where cmake configs will be installed"  FORCE)
set(OUT_DIR  ${CMAKE_SOURCE_DIR}/bin                                 CACHE PATH "Path where outputs will be installed"        FORCE)
set(OUT_TEST ${CMAKE_SOURCE_DIR}/test/bin                            CACHE PATH "Path where test outputs will be installed"   FORCE)

set(INSTALL_INCLUDE_DIR  "${CMAKE_CURRENT_LIST_DIR}/share/include/rFBP"  CACHE PATH "Path where headers will be installed")
set(INSTALL_CMAKE_DIR    "${CMAKE_CURRENT_LIST_DIR}/share/rFBP"          CACHE PATH "Path where cmake configs will be installed")

set(CMAKE_DEBUG_POSTFIX d)

set(rfbplib rfbp)
set(rfbptestmain test_main)
set(rfbptrainmain train_main)

file(GLOB SRC           "${SRC_DIR}/*.cpp" )
file(GLOB HEADER        "${INC_DIR}/*.h"   )
file(GLOB HPP           "${INC_DIR}/*.hpp" )

include_directories(${INC_DIR} ${HPP_DIR} ${TEST_DIR})

if (PYWRAP)
  include( UseCython )
  find_package (Python COMPONENTS Interpreter NumPy)
endif()

set(rFBP_INSTALL_INCLUDE_DIR ${INSTALL_INCLUDE_DIR})

configure_file(${CMAKE_SOURCE_DIR}/rFBP.pc.in ${CMAKE_SOURCE_DIR}/rFBP.pc @ONLY)
message(STATUS "Pkg-config generated")

#################################################################
#                          SUMMARY                              #
#################################################################

message(STATUS ""                                                                    )
message(STATUS "=================== rFBP configuration Summary =================="   )
message(STATUS "   rFBP version: ${RFBP_MAJOR}.${RFBP_MINOR}.${RFBP_REVISION}"       )
message(STATUS ""                                                                    )
message(STATUS "   Build type : ${CMAKE_BUILD_TYPE}"                                 )
message(STATUS "   C++ :"                                                            )
message(STATUS "      C++ Compiler : ${CMAKE_CXX_COMPILER}"                          )
message(STATUS "      C++ flags    :"                                                )
foreach(FLAG ${CMAKE_CXX_FLAGS_LIST})
  message(STATUS "                    * ${FLAG}"                                     )
endforeach(FLAG)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "      C++ Debug flags:"                                            )
  foreach(FLAG ${CMAKE_CXX_FLAGS_DEBUG})
    message(STATUS "                    * ${FLAG}"                                   )
  endforeach(FLAG)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
  message(STATUS "      C++ Release flags  :"                                        )
  foreach(FLAG ${CMAKE_CXX_FLAGS_RELEASE})
    message(STATUS "                    * ${FLAG}"                                   )
  endforeach(FLAG)
endif()
message(STATUS "      Linker flags : "                                               )
foreach(FLAG ${linked_libs})
  message(STATUS "                    * ${FLAG}"                                     )
endforeach(FLAG)
message(STATUS ""                                                                    )
message(STATUS "   OpenMP support : ${OMP}"                                          )
message(STATUS "   Scorer support : ${SCORER}"                                       )
message(STATUS "   VERBOSE level  : ${VERBOSE}"                                      )
message(STATUS "   STATS   level  : ${STATS}"                                        )
message(STATUS "   BUILD_TEST     : ${BUILD_TEST}"                                   )
message(STATUS "   Compile Pythonize version : ${PYWRAP}"                            )
message(STATUS ""                                                                    )


#################################################################
#                         MAIN RULES                            #
#################################################################

if (PYWRAP)
  add_subdirectory(${PYC_DIR}/source)
endif()

add_library(${rfbplib} SHARED ${SRC} ${HEADER} ${HPP} ${SCORER_HEADER})
set_property(TARGET ${rfbplib} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(${rfbplib} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include> $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/hpp> $<INSTALL_INTERFACE:${rFBP_INSTALL_INCLUDE_DIR}>)
target_link_libraries(${rfbplib} ${linked_libs})

list (APPEND ALL_HEADER ${HEADER})
list (APPEND ALL_HEADER ${HPP})
set_target_properties(${rfbplib} PROPERTIES PUBLIC_HEADER "${ALL_HEADER}")

add_executable(${rfbptrainmain} ${EXAMPLE}/train_main.cpp)
target_link_libraries(${rfbptrainmain} ${linked_libs} ${rfbplib})

add_executable(${rfbptestmain} ${EXAMPLE}/test_main.cpp)
target_link_libraries(${rfbptestmain} ${linked_libs} ${rfbplib})

if (BUILD_TEST)
  add_subdirectory(${TEST_DIR})
endif()

#################################################################
#                          INSTALLERS                           #
#################################################################

install(TARGETS ${rfbplib}          EXPORT rFBPTargets
                                    RUNTIME DESTINATION ${OUT_DIR}
                                    LIBRARY DESTINATION ${LIB_DIR}
                                    ARCHIVE DESTINATION ${LIB_DIR}
                                    PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR}
                                    COMPONENT dev
                                    )
install(TARGETS ${rfbptrainmain}   DESTINATION ${OUT_DIR})
install(TARGETS ${rfbptestmain}    DESTINATION ${OUT_DIR})


#################################################################
#                          EXPORT RFBP                          #
#################################################################

install (EXPORT rFBPTargets
         FILE rFBPTargets.cmake
         NAMESPACE rFBP::
         DESTINATION "${INSTALL_CMAKE_DIR}"
        )

# Export the package for use from the build-tree (this registers the build-tree with a global CMake-registry)
export (PACKAGE rFBP)

# Create the rFBPConfig.cmake
# First of all we compute the relative path between the cmake config file and the include path
file (RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_INCLUDE_DIR}")
set (CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file (rFBPConfig.cmake.in "${PROJECT_BINARY_DIR}/rFBPConfig.cmake" @ONLY)
set (CONF_INCLUDE_DIRS "\${rFBP_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file (rFBPConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/rFBPConfig.cmake" @ONLY)

# Create the rFBPConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file ("${PROJECT_BINARY_DIR}/rFBPConfigVersion.cmake"
                                  COMPATIBILITY SameMajorVersion
                                 )

install (FILES
         "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/rFBPConfig.cmake"
         "${PROJECT_BINARY_DIR}/rFBPConfigVersion.cmake"
         DESTINATION "${INSTALL_CMAKE_DIR}"
         )
