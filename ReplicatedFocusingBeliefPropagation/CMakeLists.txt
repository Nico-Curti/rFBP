# If the pyx file is a C++ file, we should specify that here.

set_source_files_properties( ${PYC_DIR}/Fprotocol.pyx                                  PROPERTIES CYTHON_IS_CXX TRUE )
set_source_files_properties( ${PYC_DIR}/Pattern.pyx                                    PROPERTIES CYTHON_IS_CXX TRUE )
set_source_files_properties( ${PYC_DIR}/ReplicatedFocusingBeliefPropagation.pyx        PROPERTIES CYTHON_IS_CXX TRUE )

# Multi-file cython modules do not appear to be working at the moment.
cython_add_module( Fprotocol                                 Fprotocol.pyx                                 ${INC_DIR}/fprotocol.h  ${SRC_DIR}/fprotocol.cpp)
cython_add_module( Pattern                                   Pattern.pyx                                   ${INC_DIR}/pattern.h     ${SRC_DIR}/pattern.cpp)
cython_add_module( ReplicatedFocusingBeliefPropagation       ReplicatedFocusingBeliefPropagation.pyx       ${INC_DIR}/rfbp.h        ${SRC_DIR}/rfbp.cpp)
