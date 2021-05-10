#----------------------------------------------------------------------

SET(INCLUDE_LIB_SRC tracker.cpp)

add_library(include ${INCLUDE_LIB_SRC})

target_include_directories(include PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
