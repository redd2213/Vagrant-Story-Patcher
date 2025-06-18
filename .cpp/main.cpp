#define SDL_MAIN_HANDLED
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>



int main() {

	// Initialize SDL2 and OpenGL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("VSPatcher", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // VSync

	// Initialize SDL2 mixer for audio playback
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << "SDL_mixer did not start " << Mix_GetError() << std::endl; // Error in case the mixer fails to initialize
    }

	// Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic(); // Set ImGui style to classic for a more traditional look, Dark colors tend to switch to a more gray-ish color palette

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

	// Load music file for audio playback
    Mix_Music* bgm = Mix_LoadMUS("VSPatcher.mp3");
    Mix_VolumeMusic(3);
    if (!bgm) {
		std::cerr << "Failed to load mp3 file" << Mix_GetError() << std::endl; // Error in case the music file fails to load
    }
    else {
        Mix_PlayMusic(bgm, -1); // -1 means loop indefinitely
    }



    // App icon
    SDL_Surface* icon = SDL_LoadBMP("VSPatcher.bmp");
    if (icon) {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }
    else {
		std::cerr << "Failed to load bmp file " << SDL_GetError() << std::endl; // Error in case the bmp icon file fails to load
    }

    // Main loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Main window creation
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize); 
        ImGui::Begin("VSPatcher", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoBringToFrontOnFocus); // Set window size to a fixed size and pinned the grey usual ImGui window behind it

		ImGui::SetWindowSize(ImVec2(640, 480)); // Using 640x480 as the window size, which is a common resolution for older tools
        
        // ASCII art logo for "Vagrant Story"
        ImGui::Dummy(ImVec2(0.0f, 16.0f));
		ImGui::SetWindowFontScale(0.65f); // Scaling the text to the desired size for it to fit the window

        ImVec2 oldSpacing = ImGui::GetStyle().ItemSpacing;// ASCII logo had a gap between the two blocks were split, and I figured out what triggered it was the ImGui widget spacing
		ImGui::GetStyle().ItemSpacing = ImVec2(0, 0);// Set the ImGui widget spacing to 0 so it removes the gap

        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
            "                                                                                                .\n"
            "                                                                                               ..\n"
            "                                                                                              .--\n"
            "                                                                                              .--\n"
            "                                                                                              _--\n"
            "                                                                                              _--\n"
            "                                                                                              _--\n"
            "                .      ...    .....                                                    ..______--_______.     __---_.                     _--------_                  .-_.        ._.              \n"
            "       _-----------_  ._----------_.   _--____----_---------_  _----------------.  _-----------------------_---.   ._-----------------__---__..__-----_  ._---_----_._------. ._-----__ .         \n"
            "          _----.          _----.     _--_      _--._--_. _---_    ._---_    _---_     ._--_..._--....._..  _--_      .--_   _--_  _--__--_        ._---_  .--_   _---_ _---.     --_               \n"
            "           .---_          _-_--_    _--_           _--_.  _---    _-_---.   .----_     _--    _--.         _--_             _--_     _--.           _---_ .---    _---  ._--.  .--.             \n"
            "            _--_         .-_._--.  .--_            _--_.  _---    _-._--_   .--__-_    _--    _--.        .----_            _--_     --_      ..     .---_.---    _---    _--__--.              \n"
            "             _--.        _-_ _--_  _--_            .--_  _---_   .-_ ._--_  .--_ _-_   _--    _--.        ._-----_          .--_    _--_     ._-_     _---._--   _---_     _---_               \n"
            "             .---     __ -_.  _--__---_     ._____ _---_----_ ._ _-_  _---_----_  _-.  _-_    _--.          ._-----_.       .--_    _--_    _----_    .---_---__----_       ---_              ");

        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
            "              _--_    _---_   _-------_      __---..-------_  _---_.  ._---__--_   _-. _-_    _--.             _------_.    _--_    _--_      __.     .--_._-------.        _--_              \n"
            "              ._--     .---___----__--_.      _---._--_._--_    _--___-----..--_    _-__-_    _--                __-----_   _--_     ---_             _--..---._---_        _--.             \n"
            "               _--_    _-_._-_..---_---_.     _---._--_  ---_  .--._--_ _--_.--_     _---_    _--        ..        _----.   _--_     .---_           .--_ .---  _---.       _--_             \n"
            "                _-_.  ._-       _--_.---_.    _---._--_  .---  _-_      .---_--_      _--_    _--        __         .--_    _--_      ._---_.      ._--_  ._--   .---.      _--_             \n"
            "                .--_.__--___. ._----_.__---____--__----_  _--__--_...  ._-------___   ._-_    _--        _-.        .--_  _-----__.     __----____---_   ._---_.  ._--.   ._----_.           \n"
            "                 _--_.__                  ...              _--_......                  .-_    _--        _--.       _-_.                   .__--__.                 _-_.   .                 \n"
            "                  _-___.                                    .-_.                        ._    _--        ___--_   ._-_                                               ._-.                      \n"
            "                  _--__                                      ._-.                             _--              ....                                                    ._-_                    \n"
            "                  ._--_                                        .__                            _--.                                                                                               \n"
            "                   _-_.                                          ..                           ._-                                                                                                \n"
            "                    __                                                                        .__                                                                                                \n"
            "                                                                                               __                                                                                                \n"
            "                                                                                               _.                                                                                                \n"
        );// Text needed to be split in order to not exceed the 1024 character limit for the ImGui::TextColored synthax
		ImGui::GetStyle().ItemSpacing = oldSpacing;// Reset the ImGui widget spacing to the original value after the ASCII art logo
		
        ImGui::Dummy(ImVec2(0.0f, 16.0f));// Dummy = Spacing 

		ImGui::SetWindowFontScale(1.0f);
		ImGui::Text("                         Vagrant Story Universal Mod/Hack ISO Patcher - Version 1.0");
        ImGui::Text("                                              Created by redd2213");

        ImGui::Dummy(ImVec2(0.0f, 32.0f));

		ImGui::Separator();
        ImGui::End();

        // Necessary Rendering
        ImGui::Render();
        glViewport(0, 0, 640, 400);
        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

	// Cleanup after running the application
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();

    return 0;
}
