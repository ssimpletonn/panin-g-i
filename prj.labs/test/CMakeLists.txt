project(test)
add_executable(test test.cpp test.report.md.in.txt)
target_link_libraries(test ${OpenCV_LIBS})

file(READ test.cpp cpp_source)
# prepare report md file
configure_file(test.report.md.in.txt ${CMAKE_CURRENT_BINARY_DIR}/test.report.md @ONLY)
add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/test.report.md" "$<TARGET_FILE_DIR:${PROJECT_NAME}>")