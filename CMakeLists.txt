cmake_minimum_required(VERSION 3.16)
project(GameBoyEmulator VERSION 1.0)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 소스 디렉토리 추가
add_subdirectory(src)
#add_subdirectory(tests)

# 외부 라이브러리 디렉토리 추가
add_subdirectory(external/googletest)
target_link_libraries(GameBoyEmulator gtest gtest_main)
#add_subdirectory(third_party)

# 헤더 파일 경로 추가
include_directories(include)

# 구글 테스트 추가
enable_testing()
add_executable(run_tests tests/main.cpp tests/test_cpu.cpp)
target_link_libraries(run_tests gtest gtest_main)
add_test(NAME RunAllTests COMMAND run_tests)
