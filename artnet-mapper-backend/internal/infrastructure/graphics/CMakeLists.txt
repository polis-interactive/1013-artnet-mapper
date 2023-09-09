
set(
    RENDER_SOURCES renderer/renderer.cpp renderer/headless_renderer.cpp
)

set (
    GL_SOURCES gl/pixel_buffer.cpp gl/shader.cpp gl/image_texture.cpp gl/uniform.cpp gl/full_vbo.cpp gl/fbo.cpp
)

set (
    GLAD_SOURCES ${glad_dir}/src/glad.c ${glad_dir}/src/glad_egl.c
)

set(
    SOURCES graphics.cpp
)
set(TARGET_LIBS pthread EGL GL dl drm gbm GLESv2)

if (FEATURE_GRAPHICS_GLFW)
    set(RENDER_SOURCES ${RENDER_SOURCES} renderer/glfw_renderer.cpp renderer/glfw_headless_renderer.cpp renderer/dummy.cpp)
    set(TARGET_LIBS ${GLFW3_LINK_LIBRARIES} ${TARGET_LIBS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLFW_RENDERER_")
endif()

add_library(graphics STATIC graphics.cpp ${RENDER_SOURCES} ${GLAD_SOURCES} ${GL_SOURCES})
target_link_libraries(graphics PRIVATE ${TARGET_LIBS})