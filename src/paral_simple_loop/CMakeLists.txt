
add_executable(
    demo_parallel_for
    demo_parallel_for.cpp
)

target_link_libraries(
    demo_parallel_for 
    ${TBB_IMPORTED_TARGETS}
)

add_executable(
    demo_parallel_reduce
    demo_parallel_reduce.cpp
)

target_link_libraries(
    demo_parallel_reduce
    ${TBB_IMPORTED_TARGETS}
)

