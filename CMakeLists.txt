cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)

# setup the basic stuff
set(PROJECT_NAME uefi)
set(CMAKE_BUILD_TYPE "Release")

add_definitions("/wd4804")

# get the directory
get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)

# set the output directory
set(OUTPUT_DIR ${PROJECT_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_CURRENT_BINARY_DIR ${OUTPUT_DIR})
set(CMAKE_BINARY_DIR ${OUTPUT_DIR})
set(PROJECT_BINARY_DIR ${OUTPUT_DIR})

# set the cmake state files directory
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# setup the project
project(${PROJECT_NAME})

# set the link directories
link_directories(AFTER ${PROJECT_DIR}/vendor/EDK-II/x64/Release/)

# set the glob recursives
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE UEFI_LIBS ${PROJECT_DIR}/vendor/EDK-II/x64/Release/*.lib)

# setup the library
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

# set the include directories
target_include_directories(${PROJECT_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/src/
  ${PROJECT_SOURCE_DIR}/vendor/edk2
  ${PROJECT_SOURCE_DIR}/vendor/edk2/MdePkg/Include
  ${PROJECT_SOURCE_DIR}/vendor/edk2/MdePkg/Include/X64
  ${PROJECT_SOURCE_DIR}/vendor/edk2/ShellPkg/Include
  ${PROJECT_SOURCE_DIR}/vendor/edk2/CryptoPkg/Include
)

# set the extension and language
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".efi")

# set the properties and compile options
target_compile_definitions(${PROJECT_NAME} PRIVATE
  -D_CRT_SECURE_NO_WARNINGS   # #define _CRT_SECURE_NO_WARNINGS
  -DNDEBUG                    # #define NDEBUG
  -DUNICODE                   # #define UNICODE
  -D_UNICODE                  # #define _UNICODE
  -D__UD_STANDALONE__         # #define __UD_STANDALONE__
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/LTCG:INCREMENTAL /IGNORE:4001 /IGNORE:4254 /IGNORE:4281 /NXCOMPAT:NO /DYNAMICBASE:NO /MANIFEST:NO /Driver /RELEASE /MACHINE:X64 /ENTRY:EfiMain /SUBSYSTEM:EFI_BOOT_SERVICE_DRIVER /MERGE:.rdata=.text /ALIGN:4096 /NODEFAULTLIB /SECTION:.xdata,D")
target_compile_options(${PROJECT_NAME} PRIVATE /MP /GS- /W4 /Gy /Zi /Gm- /Od /Ob2 /Zc:inline /GF /WX /GR- /Oy /Oi /MD /FC /nologo /diagnostics:column)

# set the libraries
target_link_libraries(${PROJECT_NAME}
  ${UEFI_LIBS}
)