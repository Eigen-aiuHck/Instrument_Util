cmake_minimum_required(VERSION 3.15...3.26)

project(
  Instrument_Util
  VERSION 1.0
  DESCRIPTION "C++ tool coresystems for smg2"
  LANGUAGES CXX)

add_subdirectory(lib/Dynamic_Type)
add_subdirectory(lib/array_tools)
add_subdirectory(lib/B_tree)

add_subdirectory(src)
