
add_executable(
    demo_parallel_for
    demo_parallel_for.cpp
)

target_link_libraries(
    demo_parallel_for 
    ${TBB_IMPORTED_TARGETS}
)

