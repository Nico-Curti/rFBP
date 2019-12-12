# Test files

add_executable(test_spline ${TEST}/test_spline.cpp)
target_link_libraries(test_spline ${linked_libs} ${rfbplib})

add_executable(test_atanherf ${TEST}/test_atanherf.cpp)
target_link_libraries(test_atanherf ${linked_libs} ${rfbplib})

add_executable(test_fprotocol ${TEST}/test_fprotocol.cpp)
target_link_libraries(test_fprotocol ${linked_libs} ${rfbplib})

add_executable(test_pattern ${TEST}/test_pattern.cpp)
target_link_libraries(test_pattern ${linked_libs} ${rfbplib})

add_executable(test_params ${TEST}/test_params.cpp)
target_link_libraries(test_params ${linked_libs} ${rfbplib})

add_executable(test_magp ${TEST}/test_magp.cpp)
target_link_libraries(test_magp ${linked_libs} ${rfbplib})

add_executable(test_magt ${TEST}/test_magt.cpp)
target_link_libraries(test_magt ${linked_libs} ${rfbplib})

# Installation of targets

install(TARGETS test_spline             DESTINATION ${OUT_TEST})
install(TARGETS test_atanherf           DESTINATION ${OUT_TEST})
install(TARGETS test_fprotocol          DESTINATION ${OUT_TEST})
install(TARGETS test_pattern            DESTINATION ${OUT_TEST})
install(TARGETS test_params             DESTINATION ${OUT_TEST})
install(TARGETS test_magp               DESTINATION ${OUT_TEST})
install(TARGETS test_magt               DESTINATION ${OUT_TEST})
