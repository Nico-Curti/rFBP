# If the pyx file is a C++ file, we should specify that here.

set(PYC_SRC_DIR    ${PYC_DIR}/source       CACHE PATH "Path where find pyx files" )
set(PYC_INC_DIR    ${PYC_DIR}/lib          CACHE PATH "Path where find pxd files" )

set_source_files_properties( ${PYC_SRC_DIR}/FocusingProtocol.pyx PROPERTIES CYTHON_IS_CXX TRUE )
set_source_files_properties( ${PYC_SRC_DIR}/Patterns.pyx PROPERTIES CYTHON_IS_CXX TRUE )
set_source_files_properties( ${PYC_SRC_DIR}/rFBP.pyx PROPERTIES CYTHON_IS_CXX TRUE )

# Include *.pxd directory

include_directories(${PYC_INC_DIR})
add_custom_target(PyRFBP)

# Multi-file cython modules do not appear to be working at the moment.
cython_add_module( atanherf  ${PYC_SRC_DIR}/atanherf.pyx
                                                                 ${SRC_DIR}/atanherf.cpp
                                                                 ${SRC_DIR}/spline.cpp
                                                                 ${SRC_DIR}/utils.cpp
                                                                 )
target_include_directories(atanherf PRIVATE ${Python_NumPy_INCLUDE_DIRS})
target_compile_definitions(atanherf PRIVATE -DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION)

cython_add_module( FocusingProtocol  ${PYC_SRC_DIR}/FocusingProtocol.pyx
                                                                         ${SRC_DIR}/fprotocol.cpp
                                                                         ${SRC_DIR}/utils.cpp
                                                                         )
target_include_directories(FocusingProtocol PRIVATE ${Python_NumPy_INCLUDE_DIRS})
target_compile_definitions(FocusingProtocol PRIVATE -DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION)

cython_add_module( Patterns  ${PYC_SRC_DIR}/Patterns.pyx
                                                         ${SRC_DIR}/pattern.cpp
                                                         ${SRC_DIR}/utils.cpp
                                                         )
target_include_directories(Patterns PRIVATE ${Python_NumPy_INCLUDE_DIRS})
target_compile_definitions(Patterns PRIVATE -DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION)

cython_add_module( rFBP  ${PYC_SRC_DIR}/rFBP.pyx
                                                 ${SRC_DIR}/rfbp.cpp
                                                 ${SRC_DIR}/atanherf.cpp
                                                 ${SRC_DIR}/cavity_message.cpp
                                                 ${SRC_DIR}/fprotocol.cpp
                                                 ${SRC_DIR}/magnetization.cpp
                                                 ${SRC_DIR}/params.cpp
                                                 ${SRC_DIR}/pattern.cpp
                                                 ${SRC_DIR}/spline.cpp
                                                 ${SRC_DIR}/utils.cpp
                                                 )
target_include_directories(rFBP PRIVATE ${Python_NumPy_INCLUDE_DIRS})
target_compile_definitions(rFBP PRIVATE -DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION)

add_dependencies(PyRFBP rFBP Patterns FocusingProtocol atanherf)


# Installation of targets
install(TARGETS atanherf          DESTINATION ${PYC_INC_DIR}/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/atanherf.cpp              DESTINATION ${PYC_SRC_DIR})
install(TARGETS FocusingProtocol  DESTINATION ${PYC_INC_DIR}/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/FocusingProtocol.cpp      DESTINATION ${PYC_SRC_DIR})
install(TARGETS Patterns          DESTINATION ${PYC_INC_DIR}/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Patterns.cpp              DESTINATION ${PYC_SRC_DIR})
install(TARGETS rFBP              DESTINATION ${PYC_INC_DIR}/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/rFBP.cpp                  DESTINATION ${PYC_SRC_DIR})
