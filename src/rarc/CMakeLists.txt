add_library(RARC STATIC rarc.cpp)

target_link_libraries(RARC PRIVATE Array_Tools B_tree)
target_include_directories(
  RARC
  PUBLIC ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/lib/array_tools
         ${PROJECT_SOURCE_DIR}/lib/b_tree)

target_compile_definitions(RARC PUBLIC RARC_PUBLIC)
target_compile_definitions(RARC PRIVATE RARC_PRIVATE)

target_compile_features(RARC PUBLIC cxx_std_20)
