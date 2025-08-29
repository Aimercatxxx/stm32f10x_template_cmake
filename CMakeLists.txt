cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# 目标平台系统名称，裸机嵌入式一般就写 Generic
set(CMAKE_SYSTEM_NAME Generic)
# 目标平台的体系结构
set(CMAKE_SYSTEM_PROCESSOR arm)


# 编译器
set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY      arm-none-eabi-objcopy)


# 跳过编译器 -rdynamic 检查
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)


project(test)
enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED OFF)
set(CMAKE_C_EXTENSIONS OFF)


# ELF 文件名称
set(ELF_TARGET ${CMAKE_PROJECT_NAME}.elf)


# 预定义宏
add_definitions(
    -D STM32F10X_HD
    -D USE_STDPERIPH_DRIVER
)

# CPU 架构
set(CPU_FLAGS
    -mcpu=cortex-m3
)


# 链接脚本
set(LINK_SCRIPT
    ${CMAKE_SOURCE_DIR}/STM32F103ZETx_FLASH.ld
)


# 启动文件
set(STARTUP_SCRIPT
    ${CMAKE_SOURCE_DIR}/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_hd.s
)


# CMSIS 内核文件
set(CMSIS_CORE
    ${CMAKE_SOURCE_DIR}/Libraries/CMSIS/CM3/CoreSupport/core_cm3.c
    ${CMAKE_SOURCE_DIR}/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c
)


# 标准库外设源文件
file(GLOB STDPERIPH_DRIVER
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F10x_StdPeriph_Driver/src/*.c
)


# 用户程序的源文件（递归查找）
file(GLOB_RECURSE USER_SRC
    ${CMAKE_SOURCE_DIR}/User/*.c
)

# BSP 相关源文件（递归查找）
file(GLOB_RECURSE BSP
    ${CMAKE_SOURCE_DIR}/BSP/*.c
)

# 添加 ELF 目标文件
add_executable(${ELF_TARGET}
    ${CMSIS_CORE}
    ${STARTUP_SCRIPT}
    ${STDPERIPH_DRIVER}
    ${USER_SRC}
)


# 头文件路径
target_include_directories(${ELF_TARGET} PRIVATE
    ${CMAKE_SOURCE_DIR}/Libraries/CMSIS/CM3/CoreSupport/
    ${CMAKE_SOURCE_DIR}/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/
    ${CMAKE_SOURCE_DIR}/Libraries/STM32F10x_StdPeriph_Driver/inc/
    ${CMAKE_SOURCE_DIR}/User/
    ${CMAKE_SOURCE_DIR}/BSP/
)


#  编译参数
target_compile_options(${ELF_TARGET} PRIVATE
    ${CPU_FLAGS}
    -Wall
    -Wextra
    -g 
    -gdwarf-2 
    -mthumb 
    -Os 
    -fdata-sections 
    -ffunction-sections
    -fmessage-length=0
)


# 链接参数
target_link_options(${ELF_TARGET} PRIVATE
    -T${LINK_SCRIPT}
    ${CPU_FLAGS}
    --specs=nano.specs
    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -Wl,--cref
    -Wl,--print-memory-usage
)


# 添加自定义命令，用于生成 hex bin 格式文件，POST_BUILD 表示生成 TARGET 后执行命令
add_custom_command(TARGET ${ELF_TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${ELF_TARGET} ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary -S ${ELF_TARGET} ${CMAKE_PROJECT_NAME}.bin
	COMMAND arm-none-eabi-objdump -d ${ELF_TARGET} > ${CMAKE_PROJECT_NAME}.asm
)