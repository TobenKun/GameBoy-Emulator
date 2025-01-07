# src 디렉토리의 하위 디렉토리 추가
# add_subdirectory(cpu)
# add_subdirectory(mmu)
# add_subdirectory(gpu)
# add_subdirectory(apu)
# add_subdirectory(mbc)
# add_subdirectory(common)

# 실행 파일 생성
file (GLOB_RECURSE SOURCES "*.cpp" "*/*.cpp")
add_executable(GameBoyEmulator  ${SOURCES})

# 컴파일 옵션 추가
target_compile_options(GameBoyEmulator PUBLIC -Wall -Wextra -Werror)

# 실행 파일 출력 디렉토리 설정
#set_target_properties(GameBoyEmulator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 컴포넌트 링크
# target_link_libraries(GameBoyEmulator 
#     CPU 
#     MMU 
#     GPU 
#     APU 
#     MBC 
#     Common
# )
