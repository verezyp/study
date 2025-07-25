cmake_minimum_required(VERSION 3.14)

project(sqlite3 VERSION 3.26.0)

enable_language(C)


set (LIB_SOURCE
  ${CMAKE_CURRENT_SOURCE_DIR}/sqlite-amalgamation-3260000/sqlite-amalgamation-3260000
)

set(LIB_SOURCE_FILES
  ${LIB_SOURCE}/sqlite3.c
  ${LIB_SOURCE}/sqlite3.h
  ${LIB_SOURCE}/sqlite3ext.h
)

set(BUILD_SHARED_LIBS ON)

set(OPTIMIZATION_FLAG "")

if(OPTIMIZE_SIZE)
    set(OPTIMIZATION_FLAG "-Os")
elseif(OPTIMIZE_SPEED)
    set(OPTIMIZATION_FLAG "-O2")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OPTIMIZATION_FLAG}")


option(OPTIMIZE_SIZE "Use -Os (optimize for size)" OFF)
option(OPTIMIZE_SPEED "Use -O2 (optimize for speed)" ON)
option(ENABLE_FTS4 "Enable FTS4 extension" ON)
option(ENABLE_RTREE "Enable R-Tree extension" ON)
option(ENABLE_COLUMN_METADATA "Enable column metadata" ON)


add_library(sqlite3 ${LIB_SOURCE_FILES})

set_target_properties(sqlite3 PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(ENABLE_FTS4)
    target_compile_definitions(sqlite3 PRIVATE SQLITE_ENABLE_FTS4)
endif()

if(ENABLE_RTREE)
    target_compile_definitions(sqlite3 PRIVATE SQLITE_ENABLE_RTREE)
endif()

if(ENABLE_COLUMN_METADATA)
    target_compile_definitions(sqlite3 PRIVATE SQLITE_ENABLE_COLUMN_METADATA)
endif()

set_property(TARGET sqlite3 PROPERTY POSITION_INDEPENDENT_CODE ON)
