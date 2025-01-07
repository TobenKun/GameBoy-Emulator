cmake_minimum_required(VERSION 3.16)
project(GameBoyEmulator VERSION 1.0)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 실행 파일 출력 경로 설정
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 컴파일 명령어 파일 생성
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 소스 디렉토리 추가
add_subdirectory(src)
#add_subdirectory(tests)

# 외부 라이브러리 디렉토리 추가
add_subdirectory(external/googletest)
#target_link_libraries(GameBoyEmulator gtest gtest_main)
#add_subdirectory(third_party)

# 헤더 파일 경로 추가
include_directories(include)
include_directories(src/cpu)

# src 디렉토리의 모든 .cpp 파일 가져옴
file(GLOB SOURCES "src/*/*.cpp")

# 테스트 실행 파일
enable_testing()
add_executable(run_tests tests/main.cpp tests/test_cpu.cpp ${SOURCES})
target_link_libraries(run_tests gtest gtest_main)

# 테스트 추가
add_test(NAME RunAllTests COMMAND run_tests)

# 컴파일 커맨드 심링크 추가
if (PROJECT_IS_TOP_LEVEL AND UNIX)
    # Create symlink to compile_commands.json for IDE to pick it up
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
endif()
