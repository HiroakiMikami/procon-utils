# Enable google test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB files *.cc)
foreach(file ${files})
    get_filename_component(exec ${file} NAME_WE)

    # Generate the main file for testing
    add_custom_command(
            OUTPUT ${exec}-test.cc
            COMMAND cat
                ${PROJECT_SOURCE_DIR}/template/template.cc
                ${PROJECT_SOURCE_DIR}/template/test/test-body.cc.part
                ${file} > ${exec}-test.cc
            DEPENDS
                ${PROJECT_SOURCE_DIR}/template/template.cc
                ${PROJECT_SOURCE_DIR}/template/test/test-body.cc.part
                ${file}
    )
    add_custom_target(generate-${exec}-test DEPENDS ${exec}-test.cc)

    # Compile the test file
    add_executable(${exec}-test ${exec}-test.cc)
    target_link_libraries(${exec}-test ${GTEST_BOTH_LIBRARIES})
    add_test(
            NAME google-test-${exec}
            COMMAND ${exec}-test)

    add_dependencies(${exec}-test generate-${exec}-test)
endforeach(file)
