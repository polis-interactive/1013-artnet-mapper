

set(SOURCES controls.cpp iono_controls.cpp input_controls.cpp)
set(TARGET_LIBS pthread)

add_library(controls STATIC ${SOURCES})
target_link_libraries(controls PRIVATE ${TARGET_LIBS})