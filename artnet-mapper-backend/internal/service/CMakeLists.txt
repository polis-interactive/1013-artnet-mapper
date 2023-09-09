
set(SOURCES service.cpp)
set(TARGET_LIBS art_net graphics controls)

add_library(service STATIC ${SOURCES})
target_link_libraries(service PRIVATE ${TARGET_LIBS})