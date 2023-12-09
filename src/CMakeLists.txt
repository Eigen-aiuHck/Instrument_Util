# add_subdirectory(bcsv)
add_subdirectory(rarc)
add_subdirectory(yaz0)

add_library(Instrument instrument.cpp)
target_link_libraries(Instrument PUBLIC fileData_class Yaz0)

target_compile_definitions(Instrument PUBLIC fileData_class_PUBLIC)
target_compile_definitions(Instrument PRIVATE fileData_class_PRIVATE)

target_compile_features(Instrument PUBLIC cxx_std_20)

add_executable(test test.cpp)
target_compile_features(test PRIVATE cxx_std_20)

target_link_libraries(test PUBLIC Array_Tools Yaz0)
target_include_directories(test PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(
  test PRIVATE ${PROJECT_SOURCE_DIR}/lib/array_tools/include)
