add_subdirectory(formatBlock)

add_library(
  bcsv STATIC bcsvFormat.cpp ${PROJECT_SOURCE_DIR}/include/bcsv/bcsvFormat.hpp
              bcsv.cpp ${PROJECT_SOURCE_DIR}/include/bcsv/bcsv.hpp)

target_include_directories(bcsv PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_compile_definitions(bcsv PUBLIC bcsv_PUBLIC)
target_compile_definitions(bcsv PRIVATE bcsv_PRIVATE)

target_compile_features(bcsv PUBLIC cxx_std_20)
target_compile_options(bcsv PRIVATE -pthread)
