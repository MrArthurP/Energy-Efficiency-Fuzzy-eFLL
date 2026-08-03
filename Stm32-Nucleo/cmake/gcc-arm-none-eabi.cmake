set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

set(CMAKE_C_COMPILER_ID GNU)
set(CMAKE_CXX_COMPILER_ID GNU)

# Some default GCC settings
# Prefer the explicit STM32 toolchain path provided by the presets when available.
if(DEFINED STM32_TOOLCHAIN_PATH AND NOT STM32_TOOLCHAIN_PATH STREQUAL "")
  set(TOOLCHAIN_BIN "${STM32_TOOLCHAIN_PATH}")
  if(NOT IS_ABSOLUTE "${TOOLCHAIN_BIN}")
    get_filename_component(TOOLCHAIN_BIN "${CMAKE_CURRENT_LIST_DIR}/${TOOLCHAIN_BIN}" ABSOLUTE)
  endif()
else()
  set(TOOLCHAIN_BIN "")
endif()

if(WIN32 AND NOT TOOLCHAIN_BIN STREQUAL "")
  set(CMAKE_C_COMPILER                "${TOOLCHAIN_BIN}/arm-none-eabi-gcc.exe")
  set(CMAKE_ASM_COMPILER              "${TOOLCHAIN_BIN}/arm-none-eabi-gcc.exe")
  set(CMAKE_CXX_COMPILER              "${TOOLCHAIN_BIN}/arm-none-eabi-g++.exe")
  set(CMAKE_LINKER                    "${TOOLCHAIN_BIN}/arm-none-eabi-g++.exe")
  set(CMAKE_OBJCOPY                   "${TOOLCHAIN_BIN}/arm-none-eabi-objcopy.exe")
  set(CMAKE_SIZE                      "${TOOLCHAIN_BIN}/arm-none-eabi-size.exe")
else()
  set(TOOLCHAIN_PREFIX                arm-none-eabi-)
  set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc)
  set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER})
  set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++)
  set(CMAKE_LINKER                    ${TOOLCHAIN_PREFIX}g++)
  set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
  set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)
endif()

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# MCU specific flags
set(TARGET_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard ")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TARGET_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fdata-sections -ffunction-sections -fstack-usage")

# The cyclomatic-complexity parameter must be defined for the Cyclomatic complexity feature in STM32CubeIDE to work.
# However, most GCC toolchains do not support this option, which causes a compilation error; for this reason, the feature is disabled by default.
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcyclomatic-complexity")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_C_FLAGS_RELEASE "-Os -g0")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -g0")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_EXE_LINKER_FLAGS "${TARGET_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T \"${CMAKE_SOURCE_DIR}/STM32F303xx_FLASH.ld\"")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=nano.specs")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--print-memory-usage")
set(TOOLCHAIN_LINK_LIBRARIES "m")
