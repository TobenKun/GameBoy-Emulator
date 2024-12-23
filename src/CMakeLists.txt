# src 디렉토리의 하위 디렉토리 추가
# add_subdirectory(cpu)
# add_subdirectory(mmu)
# add_subdirectory(gpu)
# add_subdirectory(apu)
# add_subdirectory(mbc)
# add_subdirectory(common)

# 실행 파일 생성
add_executable(GameBoyEmulator 
	main.cpp
	./cpu/CPU.cpp)

target_compile_options(GameBoyEmulator PUBLIC -Wall -Wextra -Werror)

# 컴포넌트 링크
# target_link_libraries(GameBoyEmulator 
#     CPU 
#     MMU 
#     GPU 
#     APU 
#     MBC 
#     Common
# )
