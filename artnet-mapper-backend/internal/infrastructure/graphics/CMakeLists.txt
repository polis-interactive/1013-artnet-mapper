
set(
    RENDER_SOURCES renderer/renderer.cpp renderer/headless_renderer.cpp
)

set (
    GL_SOURCES gl/pixel_buffer.cpp
)

set(
    SOURCES graphics.cpp
)
set(TARGET_LIBS pthread EGL GL dl X11)

if (FEATURE_GRAPHICS_GLFW)
    set(RENDER_SOURCES ${RENDER_SOURCES} renderer/glfw_renderer.cpp)
    set(TARGET_LIBS ${TARGET_LIBS} ${GLFW3_LINK_LIBRARIES})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLFW_RENDERER_")
endif()

add_library(graphics STATIC graphics.cpp ${RENDER_SOURCES} ${GL_SOURCES})
target_link_libraries(graphics PRIVATE ${TARGET_LIBS})