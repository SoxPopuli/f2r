#pragma once
#include <memory>

struct SDL_Window;
union SDL_Event;
typedef void* SDL_GLContext;

namespace detail
{
    struct SDLDeleter {
        void operator()(SDL_Window* ptr);
    };
}

class Window
{
    template<class T>
    using SDLPtr = std::unique_ptr<T, detail::SDLDeleter>;

    SDLPtr<SDL_Window> window;
    SDL_GLContext context;

    bool event_loop(SDL_Event& ev);
public:
    Window();
    ~Window();

    void loop();

    SDL_Window& get_window() const { return *window; }
};
