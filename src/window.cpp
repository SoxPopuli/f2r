#include "window.hpp"

#include <glbinding/gl32core/gl.h>
#include <glbinding/glbinding.h>

#include <spdlog/spdlog.h>

#include <SDL.h>
#include <stdexcept>
#include <array>
#include <utility>

using namespace gl;

void detail::SDLDeleter::operator()(SDL_Window *ptr) { SDL_DestroyWindow(ptr); }

void set_gl_attributes()
{
    struct GLAttributes { SDL_GLattr a; int v; };
    constexpr auto attributes = std::to_array<GLAttributes>({
            { SDL_GL_RED_SIZE, 8 },
            { SDL_GL_BLUE_SIZE, 8 },
            { SDL_GL_GREEN_SIZE, 8 },
            { SDL_GL_ALPHA_SIZE, 8 },
            { SDL_GL_DOUBLEBUFFER, 1 },
            { SDL_GL_CONTEXT_MAJOR_VERSION, 3 },
            { SDL_GL_CONTEXT_MINOR_VERSION, 2 },
            { SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE }
        });
    for( const auto& attr: attributes )
        SDL_GL_SetAttribute(attr.a, attr.v);
}

Window::Window()
{
    constexpr auto DEFAULT_WINDOW_POS = SDL_WINDOWPOS_CENTERED;
    constexpr auto WINDOW_WIDTH = 800;
    constexpr auto WINDOW_HEIGHT = 600;
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        throw std::runtime_error("SDL init failed");
    
    set_gl_attributes();

    window.reset(
            SDL_CreateWindow("f2r", 
                DEFAULT_WINDOW_POS, DEFAULT_WINDOW_POS,
                WINDOW_WIDTH, WINDOW_HEIGHT,
                SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
            ));
    if(!window) throw std::runtime_error("Window creation failed");

    context = SDL_GL_CreateContext(window.get());
    SDL_GL_MakeCurrent(window.get(), context);

    SDL_GL_SetSwapInterval(1);
    glbinding::initialize([](const char* c) { 
        auto addr = SDL_GL_GetProcAddress(c);
        return reinterpret_cast<glbinding::ProcAddress>(addr);
    });
    gl::glClearColor(0.8f, 0.0f, 0.8f, 1.0f);


    auto print_gl_string = [](auto str, auto name) {
        spdlog::info(str, glGetString(name));
    };

    print_gl_string("Vendor: {}", GL_VENDOR);
    print_gl_string("Renderer: {}", GL_RENDERER);
    print_gl_string("GL Version: {}", GL_VERSION);
    print_gl_string("GLSL Version: {}", GL_SHADING_LANGUAGE_VERSION);

    int context_flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
}

Window::~Window()
{

}

bool Window::event_loop(SDL_Event& ev)
{
    while(SDL_PollEvent(&ev))
    {
        switch(ev.type)
        {
            case SDL_QUIT: return false;
            default: break;
        }
    }

    return true;
}

void Window::loop()
{
    SDL_Event ev;
    while(event_loop(ev))
    {
        gl::glClear(gl::ClearBufferMask::GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window.get());
    }
}
