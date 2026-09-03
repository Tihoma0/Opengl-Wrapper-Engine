#include <iostream>
#include <memory>
#define DEBUG_MODE
#define QCONFIG_ENABLE_ALL



#include <cassert>
#include <thread>

#include "rendercore/Graphics.h"
#include "rendercore/window/Window.h"
#include "src/rendercore/opengl/GLContext.h"

#include "src/rendercore/material/Material.h"
#include "src/rendercore/render/Renderer.h"
#include "src/rendercore/material/Shader.h"

#include "rendercore/mesh/Mesh.h"
#include "src/rendercore/render/Framebuffer.h"

#include "rendercore/surface/images/ImageViewer.h"

#include "rendercore/surface/textures/Sampler.h"

#include "rendercore/surface/textures/Texture.h"

#include "rendercore/core/enums/mesh/DrawOptions.h"
#include "rendercore/core/math/Circle.h"
#include "rendercore/core/math/Rectangle.h"
#include "rendercore/surface/images/ImageProcessor.h"


void multicontexttest() {
    try {
        std::shared_ptr<Window> windowA = Window::create(
            "Window A",
            800, 600,
            WindowPositions::LEFT,
            WindowPositions::CENTER_Y,
            4, 5,
            false,
            true
        );

        std::shared_ptr<Window> windowB = Window::create(
            "Window B",
            800, 600,
            WindowPositions::RIGHT,
            WindowPositions::CENTER_Y,
            4, 5,
            false,
            true
        );

        std::cout << "Contexts created\n";


        windowA->make_current();

        if (GLContext::current_context != &windowA->getContext()) {
            throw std::runtime_error("Window A context not current");
        }

        glClearColor(1.0f, 0.0f, 0.0f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        windowA->flipBuffers();

        std::cout << "Window A OK\n";

        // -----------------------------
        // Switch to second context
        // -----------------------------
        windowB->make_current();

        if (GLContext::current_context != &windowB->getContext()) {
            throw std::runtime_error("Window B context not current");
        }

        glClearColor(0.0f, 0.0f, 1.0f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        windowB->flipBuffers();


        std::cout << "Window B OK\n";

        // -----------------------------
        // Stress-test switching
        // -----------------------------
        for (int i = 0; i < 500; ++i) {

            // ---- Window A ----
            windowA->make_current();

            if (GLContext::current_context != &windowA->getContext()) {
                throw std::runtime_error("Context switch failed (A)");
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            GLContext::current_context->state.current_framebuffer = 0;

            glBindVertexArray(0);
            GLContext::current_context->state.current_vertex_array = 0;

            glUseProgram(0);
            GLContext::current_context->state.current_shader = 0;


            glClearColor(
                (i % 255) / 255.0f,
                0.0f,
                0.0f,
                1.0f
            );

            glClear(GL_COLOR_BUFFER_BIT);

            windowA->flipBuffers();

            // ---- Window B ----
            windowB->make_current();

            if (GLContext::current_context != &windowB->getContext()) {
                throw std::runtime_error("Context switch failed (B)");
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            GLContext::current_context->state.current_framebuffer = 0;

            glBindVertexArray(0);
            GLContext::current_context->state.current_vertex_array = 0;

            glUseProgram(0);
            GLContext::current_context->state.current_shader = 0;

            glClearColor(
                0.0f,
                0.0f,
                (i % 255) / 255.0f,
                1.0f
            );

            glClear(GL_COLOR_BUFFER_BIT);

            windowB->flipBuffers();
        }

        windowA->make_current();
        glClearColor(0, 1, 0, 0.5);
        glClear(GL_COLOR_BUFFER_BIT);
        windowA->flipBuffers();
        windowB->make_current();
        glClearColor(0, 1, 0, 0.5);
        glClear(GL_COLOR_BUFFER_BIT);
        windowB->flipBuffers();

        std::cout << "Context stress test passed\n";

        // -----------------------------
        // Raw GL desync test
        // -----------------------------
        windowA->make_current();

        GLuint testFBO;
        glGenFramebuffers(1, &testFBO);

        glBindFramebuffer(GL_FRAMEBUFFER, testFBO);

        GLint framebuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);

        std::cout << "\nFramebuffer: " << framebuffer << std::endl;

        try {
            windowA->getContext().validate_state();
            std::cout << "ERROR: validation failed to detect desync\n";
        }
        catch (const std::exception& e) {
            std::cout << "Expected desync detected: "
                      << e.what() << std::endl;
        }

        // -----------------------------
        // Cache invalidation test
        // -----------------------------
        windowA->getContext().invalidate_state();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        GLContext::current_context->state.current_framebuffer = 0;

        windowA->getContext().validate_state();

        std::cout << "Invalidate state test passed\n";

        // -----------------------------
        // Idle loop
        // -----------------------------
        while (true) {
            windowA->flipBuffers();
            windowB->flipBuffers();
        }

    }
    catch (const std::exception& e) {
        std::cerr << "FATAL ERROR:\n";
        std::cerr << e.what() << std::endl;
    }

}

void testII() {
    auto window = Window::create("test", 800, 600, WindowPositions::CENTER_X, WindowPositions::CENTER_Y, 4, 5, false, true);
    auto shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/vertex.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/fragment.glsl")
        ->compile();
    auto mesh = Mesh::create_quad(-1, -1, 2, 2);
    auto texture = Texture2D::create();
    // texture->set_image(Image("../data/Head.png"));
    texture->allocate(100, 100, PixelStorageFormat::RGBA8, false);
    texture->fill(Color(0, 0, 255, 255));
    Material m = Material(shader);
    m.set_texture("u_texture", texture);
    while (true) {
        Renderer::clear({0, 0, 0, 100});
        Renderer::draw(mesh, m, {});
        window->flipBuffers();
    }
}

void rendererTest() {
    std::shared_ptr<Window> window = Window::create("Renderer Test", 800, 600, WindowPositions::CENTER_X, WindowPositions::CENTER_Y, 4, 5, false, true);
    std::shared_ptr<Shader> shader = Shader::create()->add_file_src(ShaderStage::Vertex, "../shaders/vertex.glsl")->add_file_src(ShaderStage::Fragment, "../shaders/fragment.glsl")->compile();
    Material material = Material(shader);
    std::shared_ptr<Mesh> mesh = Mesh::create_quad(-1, -1, 2, 2);
    std::shared_ptr<Texture2D> texture = Texture2D::create();
    Image i = Image("../data/Head.png");
    // texture->set_image(i);
    texture->allocate(100, 100, PixelStorageFormat::RGBA8, true);
    texture->fill(Color(0xffff0077));
    Image test = Image(Color(0xff00ffff), 200, 100, PixelStorageFormat::RGBA8);



    std::shared_ptr<Texture2D> tex2 = Texture2D::create();
    tex2->set_image(test);

    std::shared_ptr<Framebuffer> fbo = Framebuffer::create();
    fbo->add_color_attachment(tex2, 0);
    fbo->bind();

    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    Image img2 = fbo->getColorImage(0);

    material.set_texture("u_texture", tex2, 0);
    ImageViewer iv = ImageViewer();
    iv.showImage(img2);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    window->bind();
    while (true) {
        Renderer::clear(Color(0xff000088));
        // for (int i = 0; i < 100; ++i) {
            Renderer::draw(mesh, material, {});
        // }

        window->flipBuffers();
    }
}

void fbo_readback_test()
{
    auto window = Window::create(
        "FBO Readback Test",
        800, 600,
        WindowPositions::CENTER_X,
        WindowPositions::CENTER_Y,
        4, 5,
        false, true
    );

    // -------------------------
    // Create texture (color)
    // -------------------------
    Image colorImg(Color(0xffff00ff), 200, 100, PixelStorageFormat::RG16F);

    auto colorTex = Texture2D::create();
    colorTex->set_image(colorImg);

    // -------------------------
    // Create framebuffer
    // -------------------------
    auto fbo = Framebuffer::create();
    fbo->add_color_attachment(colorTex, 0);

    std::shared_ptr<Texture2D> depthBuffer = Texture2D::create();
    depthBuffer->allocate(200, 100, PixelStorageFormat::DEPTH24_STENCIL8);

    fbo->add_depth_stencil_attachment(depthBuffer);

    fbo->bind();

    GLenum drawBuf = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &drawBuf);

    // -------------------------
    // Clear to red
    // -------------------------
    glClearColor(1.f, 1.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // -------------------------
    // Read COLOR
    // -------------------------
    Image colorRead = fbo->getColorImage(0);

    std::cout << "Color[0]: "
              << (int)colorRead.buffer()[0] << ", "
              << (int)colorRead.buffer()[1] << ", "
              << (int)colorRead.buffer()[2] << ", "
              << (int)colorRead.buffer()[3] << "\n";

    // -------------------------
    // Optional: Depth test setup
    // -------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    float depthClear = 1.0f;
    int stencilClear = 200;

    glClearBufferfi(
        GL_DEPTH_STENCIL,
        0,
        depthClear,
        stencilClear
    );

    Image stencilRead = fbo->getStencilImage();
    Image depthRead = fbo->getDepthImage();
    std::cout << "Depth[0]: "
              << ((float*)depthRead.buffer().data())[0] << "\n";
    std::cout << "Stencil[0]: "
           << static_cast<int>(stencilRead.buffer().data()[0])
           << "\n";
    std::cout << "Image: "
            << static_cast<int>(colorImg.buffer().data()[0]) << ", "
            << static_cast<int>(colorImg.buffer().data()[1]) << ", "
            << static_cast<int>(colorImg.buffer().data()[2]) << ", "
            << static_cast<int>(colorImg.buffer().data()[3])
            << "\n";
    // -------------------------
    // Show result
    // -------------------------
    ImageViewer iv;
    iv.showImage(stencilRead);
    iv.showImage(depthRead);
    iv.showImage(colorRead);
}

const char* format_name(PixelStorageFormat fmt)
{
    switch (fmt)
    {
        case PixelStorageFormat::R8: return "R8";
        case PixelStorageFormat::RG8: return "RG8";
        case PixelStorageFormat::RGB8: return "RGB8";
        case PixelStorageFormat::RGBA8: return "RGBA8";

        case PixelStorageFormat::SRGB8: return "SRGB8";
        case PixelStorageFormat::SRGBA8: return "SRGBA8";

        case PixelStorageFormat::R16F: return "R16F";
        case PixelStorageFormat::RG16F: return "RG16F";
        case PixelStorageFormat::RGB16F: return "RGB16F";
        case PixelStorageFormat::RGBA16F: return "RGBA16F";

        case PixelStorageFormat::DEPTH16: return "DEPTH16";
        case PixelStorageFormat::DEPTH24: return "DEPTH24";
        case PixelStorageFormat::DEPTH32F: return "DEPTH32F";

        case PixelStorageFormat::DEPTH24_STENCIL8: return "DEPTH24_STENCIL8";
        case PixelStorageFormat::STENCIL8: return "STENCIL8";
    }
    return "UNKNOWN";
}

void print_pixel(const Image& img)
{
    Color c = Color::from_buffer(
        img.buffer().data(),
        img.format()
    );

    std::cout
        << "  Color = ("
        << c.r << ", "
        << c.g << ", "
        << c.b << ", "
        << c.a << ")\n";
}

void conversion_stress_test()
{
    Image img(
        Color(0.25f, 0.5f, 0.75f, 1.0f),
        1,
        1,
        PixelStorageFormat::RGBA8
    );

    std::cout
        << format_name(img.format())
        << "\n";

    print_pixel(img);

    const PixelStorageFormat chain[] =
    {
        PixelStorageFormat::RGB8,
        PixelStorageFormat::RG8,
        PixelStorageFormat::R8,

        PixelStorageFormat::R16F,
        PixelStorageFormat::RG16F,
        PixelStorageFormat::RGB16F,
        PixelStorageFormat::RGBA16F,

        PixelStorageFormat::RGBA8,

        PixelStorageFormat::DEPTH32F,

        PixelStorageFormat::RGBA16F,
        PixelStorageFormat::RG8,
        PixelStorageFormat::RGBA8,

        PixelStorageFormat::DEPTH24_STENCIL8,
        PixelStorageFormat::RGBA8,

        PixelStorageFormat::STENCIL8,
        PixelStorageFormat::RGBA8
    };

    for (auto fmt : chain)
    {
        img = ImageProcessor::convert(img, fmt);

        std::cout
            << "-> "
            << format_name(fmt)
            << "\n";

        print_pixel(img);
    }
}

void new_tex_test() {
    std::shared_ptr<Window> window = Window::create("Renderer Test", 800, 600, WindowPositions::CENTER_X, WindowPositions::CENTER_Y, 4, 5, false, true);
    std::cout << "starting" << std::endl;
    std::shared_ptr<Shader> shader = Shader::create()->add_file_src(ShaderStage::Vertex, "../shaders/vertex.glsl")->add_file_src(ShaderStage::Fragment, "../shaders/fragment.glsl")->compile();
    Material material = Material(shader);
    std::shared_ptr<Mesh> mesh = Mesh::create_quad(-1, -1, 2, 2);
    std::shared_ptr<Texture2D> texture = Texture2D::create();
    Image i = Image("../data/Head.png");
    texture->set_image(i);
    // texture->allocate(100, 100, PixelStorageFormat::RGBA8, true);
    // texture->fill(Color(0xffff0077));
    Image test = Image(Color(0xffffffff), 200, 100, PixelStorageFormat::RGBA8);

    std::shared_ptr<Texture2D> tex2 = Texture2D::create();
    tex2->set_image(i);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    window->bind();

    std::cout << "creating sampler" << std::endl;
    std::shared_ptr<Sampler> sampler = Sampler::create();
    sampler->set_wrap_mode(WrapMode::MirroredRepeat);
    material.set_texture("u_texture", tex2, sampler);

    while (true) {
        Renderer::clear(Color(0xff00ff88));
        // for (int i = 0; i < 100; ++i) {
        Renderer::draw(mesh, material, {});
        // }

        window->flipBuffers();
    }
}


void test()
{
    std::shared_ptr<Window> window = Window::create("Renderer Test", 800, 600, WindowPositions::CENTER_X, WindowPositions::CENTER_Y, 4, 5, false, true);
    std::cout << "starting" << std::endl;
    std::shared_ptr<Shader> shader = Shader::create()->add_file_src(ShaderStage::Vertex, "../shaders/vertex.glsl")->add_file_src(ShaderStage::Fragment, "../shaders/fragment.glsl")->compile();
    Material material = Material(shader);
    std::shared_ptr<Mesh> mesh = Mesh::create_quad(-1, -1, 2, 2);
    std::shared_ptr<Texture2D> texture = Texture2D::create();
    Image i = Image("../data/Head.png");
    texture->set_image(i);
    // texture->allocate(100, 100, PixelStorageFormat::RGBA8, true);
    // texture->fill(Color(0xffff0077));
    Image test = Image(Color(0xffffffff), 200, 100, PixelStorageFormat::RGBA8);

    std::shared_ptr<Texture2D> tex2 = Texture2D::create();
    tex2->set_image(i);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    window->bind();

    std::cout << "creating sampler" << std::endl;
    std::shared_ptr<Sampler> sampler = Sampler::create();
    sampler->set_wrap_mode(WrapMode::MirroredRepeat);
    material.set_texture("u_texture", tex2, sampler);

    while (true) {
        Renderer::clear(Color(0xff00ff88));
        // for (int i = 0; i < 100; ++i) {
        Renderer::draw(mesh, material, {});
        // }

        window->flipBuffers();
    }
}


void renderer_stress_test()
{
    auto window = Window::create(
        "Renderer Stress Test",
        1200,
        800,
        WindowPositions::CENTER_X,
        WindowPositions::CENTER_Y,
        4,
        5,
        false,
        true
    );

    window->bind();

    // ============================================================
    // Shader 1 - solid color
    // ============================================================

    auto shader_color =
        Shader::create()
            ->add_src(ShaderStage::Vertex, R"(
                #version 450

                layout(location = 0) in vec2 a_position;

                uniform vec2 u_offset;
                uniform vec2 u_scale;

                void main()
                {
                    vec2 position = a_position * u_scale + u_offset;
                    gl_Position = vec4(position, 0.0, 1.0);
                }
            )")
            ->add_src(ShaderStage::Fragment, R"(
                #version 450

                uniform vec4 u_color;

                out vec4 frag_color;

                void main()
                {
                    frag_color = u_color;
                }
            )")
            ->compile();


    // ============================================================
    // Shader 2 - animated color
    // ============================================================

    auto shader_animated =
        Shader::create()
            ->add_src(ShaderStage::Vertex, R"(
                #version 450

                layout(location = 0) in vec2 a_position;

                uniform vec2 u_offset;
                uniform vec2 u_scale;

                void main()
                {
                    vec2 position = a_position * u_scale + u_offset;
                    gl_Position = vec4(position, 0.0, 1.0);
                }
            )")
            ->add_src(ShaderStage::Fragment, R"(
                #version 450

                uniform vec4 u_color;
                uniform float u_time;

                out vec4 frag_color;

                void main()
                {
                    float factor = sin(u_time) * 0.5 + 0.5;

                    frag_color = vec4(
                        u_color.r * factor,
                        u_color.g,
                        u_color.b * (1.0 - factor),
                        u_color.a
                    );
                }
            )")
            ->compile();


    // ============================================================
    // Shader 3 - texture
    // ============================================================

    auto shader_texture =
        Shader::create()
            ->add_src(ShaderStage::Vertex, R"(
                #version 450

                layout(location = 0) in vec2 a_position;

                uniform vec2 u_offset;
                uniform vec2 u_scale;

                out vec2 v_uv;

                void main()
                {
                    vec2 position = a_position * u_scale + u_offset;

                    gl_Position = vec4(position, 0.0, 1.0);

                    v_uv = a_position * 0.5 + 0.5;
                }
            )")
            ->add_src(ShaderStage::Fragment, R"(
                #version 450

                in vec2 v_uv;

                uniform sampler2D u_texture;
                uniform vec4 u_tint;

                out vec4 frag_color;

                void main()
                {
                    frag_color = texture(u_texture, v_uv) * u_tint;
                }
            )")
            ->compile();


    // ============================================================
    // Materials
    // ============================================================

    Material material_color(shader_color);
    Material material_animated(shader_animated);
    Material material_texture(shader_texture);


    // ============================================================
    // Textures
    // ============================================================

    Image red_image(
        Color(0xff000099),
        64,
        64,
        PixelStorageFormat::RGBA8
    );

    Image green_image(
        Color(0x00ff0099),
        64,
        64,
        PixelStorageFormat::RGBA8
    );

    Image blue_image(
        Color(0x0000ff99),
        64,
        64,
        PixelStorageFormat::RGBA8
    );

    auto texture_red = Texture2D::create();
    texture_red->set_image(red_image);

    auto texture_green = Texture2D::create();
    texture_green->set_image(green_image);

    auto texture_blue = Texture2D::create();
    texture_blue->set_image(blue_image);


    auto sampler_repeat = Sampler::create();
    sampler_repeat->set_wrap_mode(WrapMode::Repeat);

    auto sampler_mirror = Sampler::create();
    sampler_mirror->set_wrap_mode(WrapMode::MirroredRepeat);


    // ============================================================
    // Meshes
    // ============================================================

    auto mesh_a = Mesh::create_quad(-1, -1, 2, 2);
    auto mesh_b = Mesh::create_quad(-1, -1, 2, 2);
    auto mesh_c = Mesh::create_quad(-1, -1, 2, 2);


    // ============================================================
    // Renderer
    // ============================================================


    // ============================================================
    // Main loop
    // ============================================================

    float time = 0.0f;

    while (true)
    {
        Renderer::clear(Color(0xff202020));

        time += 0.01f;


        // --------------------------------------------------------
        // Shader 1
        // --------------------------------------------------------

        for (int i = 0; i < 100; ++i)
        {
            float x = -0.95f + (i % 20) * 0.1f;
            float y = -0.9f + (i / 20) * 0.18f;

            material_color.set_uniform(
                "u_offset",
                Vec2(x, y)
            );

            material_color.set_uniform(
                "u_scale",
                Vec2(0.035f, 0.06f)
            );

            material_color.set_uniform(
                "u_color",
                Vec4(
                    (i % 3 == 0) ? 1.0f : 0.2f,
                    (i % 3 == 1) ? 1.0f : 0.2f,
                    (i % 3 == 2) ? 1.0f : 0.2f,
                    1.0f
                )
            );

            Renderer::draw(mesh_a, material_color, {});
        }


        // --------------------------------------------------------
        // Shader 2
        // --------------------------------------------------------

        for (int i = 0; i < 100; ++i)
        {
            float x = -0.95f + (i % 20) * 0.1f;
            float y = -0.9f + (i / 20) * 0.18f;

            material_animated.set_uniform(
                "u_offset",
                Vec2(x, y)
            );

            material_animated.set_uniform(
                "u_scale",
                Vec2(0.025f, 0.04f)
            );

            material_animated.set_uniform(
                "u_color",
                Vec4(
                    1.0f,
                    0.3f + (i % 10) * 0.05f,
                    0.8f,
                    1.0f
                )
            );

            material_animated.set_uniform(
                "u_time",
                time + static_cast<float>(i) * 0.05f
            );

            Renderer::draw(mesh_b, material_animated, {});
        }


        // --------------------------------------------------------
        // Shader 3
        // --------------------------------------------------------

        Material t(shader_texture);

        t.set_uniform(
            "u_tint",
            Vec4(1.0f, 1.0f, 1.0f, 1.0f)
        );

        t.set_uniform(
            "u_offset",
            Vec2(-0.25f, 0.0f)
        );

        t.set_uniform(
            "u_scale",
            Vec2(0.2f, 0.2f)
        );

        t.set_texture(
            "u_texture",
            texture_blue,
            sampler_mirror
        );

        Renderer::draw(mesh_c, t, {});

        t.set_uniform(
            "u_offset",
            Vec2(0.25f, 0.0f)
        );

        t.set_texture(
            "u_texture",
            texture_green,
            sampler_mirror
        );

        Renderer::draw(mesh_c, t, {});

        t.set_uniform(
            "u_offset",
            Vec2(0.0f, -0.25f)
        );

        t.set_texture(
            "u_texture",
            texture_red,
            sampler_mirror
        );

        Renderer::draw(mesh_c, t, {});
        window->flipBuffers();
    }
}


void verification_test() {
    auto window = Window::create(
        "Renderer Stress Test",
        1200,
        800,
        WindowPositions::CENTER_X,
        WindowPositions::CENTER_Y,
        4,
        5,
        false,
        true
    );
    auto shader = Shader::create();
    shader->add_file_src(ShaderStage::Vertex, "../shaders/shapes_vert.glsl");
    shader->add_file_src(ShaderStage::Fragment, "../shaders/shapes_frag.glsl");
    shader->compile();

    auto mesh = Mesh::create_quad(-1, -1, 1, 1);

    Material material = Material(shader);

    while (true) {
        Renderer::clear(Color(0x0000ffff));
        Renderer::draw(mesh, material, {});
        window->flipBuffers();
    }
}




void high_level_test() {
    auto window = Window::create(
        "",
        500, 500
    );
    int tick = 0;
    auto last = std::chrono::steady_clock::now();
    int frames = 0;
    auto texture = Texture2D::create();
    texture->set_image(Image("../data/Arm_back.png"));
    CircleGroup group = CircleGroup({});
    int num_rectangles = 1;
    for (int i = 0; i < num_rectangles; ++i) {
        group.add_circle(Circle(0, 0, 50));
    }
    while (true) {
        Renderer::clear(Color(0x777777ff));
        for (int i = 0; i < 1; ++i) {
            Graphics::draw_circle_group(window, group, Color(0xff00ffff));
        }

        window->flipBuffers();
        tick++;
        frames++;

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration<double>(now - last).count();

        if (elapsed >= 1.0) {
            std::cout << "FPS: " << frames / elapsed << '\n';
            frames = 0;
            last = now;
        }
    }
}



int main() {
    high_level_test();
    return 0;
}