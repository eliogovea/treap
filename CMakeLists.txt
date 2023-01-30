cmake_minimum_required(VERSION 3.11)

project(treap)

option(TREAP_BUILD_EXAMPLES "Build examples")

add_library(treap INTERFACE)
add_library(treap::treap ALIAS treap)

target_compile_features(treap INTERFACE cxx_std_20)
target_include_directories(treap INTERFACE
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

if (TREAP_BUILD_EXAMPLES)
    enable_testing()
    include(CTest)
    add_subdirectory(example)
endif()