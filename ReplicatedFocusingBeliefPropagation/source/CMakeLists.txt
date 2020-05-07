# If the pyx file is a C++ file, we should specify that here.

set(PYC_SRC_DIR    ${PYC_DIR}/source       CACHE PATH "Path where find pyx files" )
set(PYC_INC_DIR    ${PYC_DIR}/lib          CACHE PATH "Path where find pxd files" )

set_source_files_properties( ${PYC_SRC_DIR}/FocusingProtocol.pyx PROPERTIES CYTHON_IS_CXX TRUE )
set_source_files_properties( ${PYC_SRC_DIR}/Patterns.pyx PROPERTIES CYTHON_IS_CXX TRUE )
set_source_files_properties( ${PYC_SRC_DIR}/rFBP.pyx PROPERTIES CYTHON_IS_CXX TRUE )

# Include *.pxd directory

include_directories(${PYC_INC_DIR})

# Multi-file cython modules do not appear to be working at the moment.
cython_add_module( atanherf  ${PYC_SRC_DIR}/FocusingProtocol.pyx
                                                                 ${SRC_DIR}/atanherf.cpp
                                                                 ${SRC_DIR}/spline.cpp
                                                                 ${SRC_DIR}/utils.cpp
                                                                 )

cython_add_module( FocusingProtocol  ${PYC_SRC_DIR}/FocusingProtocol.pyx
                                                                         ${SRC_DIR}/fprotocol.cpp
                                                                         ${SRC_DIR}/utils.cpp
                                                                         )

cython_add_module( Patterns  ${PYC_SRC_DIR}/Patterns.pyx
                                                         ${SRC_DIR}/pattern.cpp
                                                         ${SRC_DIR}/utils.cpp
                                                         )

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


# Installation of targets
install(TARGETS FocusingProtocol  DESTINATION ${PYC_SRC_DIR}/)
install(TARGETS Patterns          DESTINATION ${PYC_SRC_DIR}/)
install(TARGETS rFBP              DESTINATION ${PYC_SRC_DIR}/)
