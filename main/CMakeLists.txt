# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/main/*.*)
FILE(GLOB_RECURSE ui ${CMAKE_SOURCE_DIR}/main/ui/*.*)
FILE(GLOB_RECURSE imgs ${CMAKE_SOURCE_DIR}/main/ui/images/*.c)



idf_component_register(SRCS ${app_sources} ${ui}  ${imgs}
                        INCLUDE_DIRS ".")

