add_library(
  Yaz0 STATIC
  yaz0Format.cpp ${PROJECT_SOURCE_DIR}/include/yaz0/yaz0Format.hpp
  compress/yaz0_compress.cpp
  ${PROJECT_SOURCE_DIR}/include/yaz0/compress/yaz0_compress.hpp
  decompress/yaz0_decompress.cpp
  ${PROJECT_SOURCE_DIR}/include/yaz0/decompress/yaz0_decompress.hpp
  )

target_link_libraries(Yaz0 PRIVATE Array_Tools)
target_include_directories(Yaz0 PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(
  Yaz0 PRIVATE ${PROJECT_SOURCE_DIR}/lib/array_tools/include)

target_compile_definitions(Yaz0 PUBLIC Yaz0_PUBLIC)
target_compile_definitions(Yaz0 PRIVATE Yaz0_PRIVATE)

target_compile_features(Yaz0 PUBLIC cxx_std_20)