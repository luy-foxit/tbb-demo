
add_executable(
    demo_dependency_graph
    demo_dependency_graph.cpp
)

target_link_libraries(
    demo_dependency_graph
    ${TBB_IMPORTED_TARGETS}
)

add_executable(
    demo_message_flow_graph
    demo_message_flow_graph.cpp
)

target_link_libraries(
    demo_message_flow_graph
    ${TBB_IMPORTED_TARGETS}
)



