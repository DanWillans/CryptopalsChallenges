cmake_minimum_required(VERSION 3.10.2)
project(cryptopals)

include(GoogleTest)
find_package(OpenSSL)
find_package(OpenSSL REQUIRED)

add_compile_options(-Wall -Werror)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE INTERNAL "")

include_directories(include)

add_library(encoding src/encoding.cpp)
add_library(decoding src/decoding.cpp)
add_library(conversion src/conversion.cpp)
add_library(utilities src/utilities.cpp)
add_library(set_1 src/set_1.cpp)

target_link_libraries(set_1 PUBLIC decoding utilities OpenSSL::SSL)

# ##################### TESTING ######################
enable_testing()

add_executable(base64_decoding_test test/base64_decoding_tests.cpp)
target_link_libraries(base64_decoding_test PUBLIC gtest pthread conversion decoding)

add_executable(base64_encoding_test test/base64_encoding_tests.cpp)
target_link_libraries(base64_encoding_test PUBLIC gtest pthread conversion encoding)

add_executable(conversion_test test/conversion_tests.cpp)
target_link_libraries(conversion_test PUBLIC gtest pthread conversion)

add_executable(set_1_tests test/set_1/set_1_tests.cpp)
target_link_libraries(set_1_tests PUBLIC gtest pthread conversion encoding decoding set_1 utilities)

add_executable(utilities_tests test/utilities_tests.cpp)
target_link_libraries(utilities_tests PUBLIC gtest pthread utilities conversion)

gtest_discover_tests(base64_decoding_test)
gtest_discover_tests(base64_encoding_test)
gtest_discover_tests(conversion_test)
gtest_discover_tests(set_1_tests)
gtest_discover_tests(utilities_tests)