#define SDL_MAIN_HANDLED
#define _CRT_SECURE_NO_WARNINGS
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
#include <thread> 
#include "Patcher.h"
#include <windows.h>
#include <commdlg.h>

std::string OpenFileDialog(const char* filter) {
    OPENFILENAMEA ofn;
    char fileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "";

    if (GetOpenFileNameA(&ofn))
        return std::string(fileName);
    return "";
}

int main() {
    // Initialize variables
    char iso_path[1024] = "";
    char patch_path[1024] = "";
    bool create_backup = true;
    bool mute_music = false;
    const int MUSIC_VOL = 10;

    // State for the patching process
    PatcherState patchState;
    std::thread patchThread;

    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Setup Window
    SDL_Window* window = SDL_CreateWindow("VSPatcher",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_OPENGL);

    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // Enable VSync

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer did not start: " << Mix_GetError() << std::endl;
    }

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");


	// Load music file for audio playback
    Mix_Music* bgm = Mix_LoadMUS("VSPatcher.mp3");
    Mix_VolumeMusic(10); // volume (0-128)
    if (!bgm) {
        std::cerr << "Failed to load mp3: " << Mix_GetError() << std::endl;
    }
    else {
        Mix_PlayMusic(bgm, -1);
    }

    // App icon
    SDL_Surface* icon = SDL_LoadBMP("VSPatcher.bmp");
    if (icon) {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }
    else {
        std::cerr << "Failed to load icon: " << SDL_GetError() << std::endl;
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

        // Main window setup
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        ImGui::Begin("VSPatcher", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoBringToFrontOnFocus);

		// Mute Music Checkbox
        float muteTextWidth = ImGui::CalcTextSize("Mute Music").x + 40.0f;
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - muteTextWidth);

        if (ImGui::Checkbox("Mute Music", &mute_music)) {
            if (mute_music) {
                Mix_VolumeMusic(0);
            }
            else {
                Mix_VolumeMusic(MUSIC_VOL);
            }
        }

		// Logo Display
        ImGui::Dummy(ImVec2(0.0f, 16.0f));
        ImGui::SetWindowFontScale(0.65f);

        ImVec2 oldSpacing = ImGui::GetStyle().ItemSpacing;
        ImGui::GetStyle().ItemSpacing = ImVec2(0, 0);

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

        );
        ImGui::GetStyle().ItemSpacing = oldSpacing;
        ImGui::Dummy(ImVec2(0.0f, 16.0f));

        ImGui::SetWindowFontScale(1.0f);
        ImGui::Text("                          Vagrant Story Universal Mod/Hack ISO Patcher - Version 1.0");

        // Links
        const char* modListTxt = "[Mod List Link]";
        const char* discordTxt = "[Discord Server Invite]";
        float spacingLinks = 16.0f;
        float totalWidthLinks = ImGui::CalcTextSize(modListTxt).x + spacingLinks + ImGui::CalcTextSize(discordTxt).x;
        float winWidth = ImGui::GetWindowSize().x;

        ImGui::SetCursorPosX((winWidth - totalWidthLinks) * 0.5f);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
        ImGui::TextUnformatted(modListTxt);
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            SDL_OpenURL("https://github.com/redd2213/Vagrant-Story-Patcher/blob/0c6769b48e3333dafc62bca4b869a1271e010263/README.md");
        }
        ImGui::PopStyleColor();

        ImGui::SameLine(0, spacingLinks);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.6f, 1.0f, 1.0f));
        ImGui::TextUnformatted(discordTxt);
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            SDL_OpenURL("https://discord.gg/9kAhmDMStD");
        }
        ImGui::PopStyleColor();

        ImGui::Text("                                              Created by redd2213");
        ImGui::Dummy(ImVec2(0.0f, 16.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 8.0f));

        // Styling Buttons
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 1.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 1.0f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.9f, 0.8f));

        float windowWidth = ImGui::GetWindowSize().x;
        float inputWidth = 400.0f;
        float buttonWidth = ImGui::CalcTextSize("Browse ISO").x + 20.0f;
        float spacing = ImGui::GetStyle().ItemSpacing.x;
        float totalWidth = inputWidth + spacing + buttonWidth;

        // ISO Input
        ImGui::Dummy(ImVec2(0.0f, 16.0f));
        ImGui::SetCursorPosX((windowWidth - totalWidth) * 0.5f);
        ImGui::Text("Select .bin file's Path:");
        ImGui::SetCursorPosX((windowWidth - totalWidth) * 0.5f);
        ImGui::PushItemWidth(inputWidth);
        ImGui::InputText("##iso_path", iso_path, IM_ARRAYSIZE(iso_path));
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if (ImGui::Button("Browse ISO")) {
            std::string path = OpenFileDialog("Disk Image Files\0*.iso;*.bin;*.img\0All Files\0*.*\0");
            if (!path.empty()) {
                strncpy(iso_path, path.c_str(), sizeof(iso_path) - 1);
            }
        }

        // Patch Input
        ImGui::Dummy(ImVec2(0.0f, 24.0f));
        ImGui::SetCursorPosX((windowWidth - totalWidth) * 0.5f);
        ImGui::Text("Select the Patch file's Path:");
        ImGui::SetCursorPosX((windowWidth - totalWidth) * 0.5f);
        ImGui::PushItemWidth(inputWidth);
        ImGui::InputText("##patch_path", patch_path, IM_ARRAYSIZE(patch_path));
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if (ImGui::Button("Browse Patch")) {
            std::string path = OpenFileDialog("PPF Patch Files\0*.ppf\0All Files\0*.*\0");
            if (!path.empty()) {
                strncpy(patch_path, path.c_str(), sizeof(patch_path) - 1);
            }
        }

        // Checkbox
        ImGui::Dummy(ImVec2(0.0f, 24.0f));
        float checkboxWidth = ImGui::CalcTextSize("Create a Backup of The ISO Before Patching").x + 30.0f;
        ImGui::SetCursorPosX((windowWidth - checkboxWidth) * 0.5f);
        ImGui::Checkbox("Create a Backup of The .bin Before Patching", &create_backup);



            if (ImGui::Button("Apply Patch", ImVec2(applyBtnWidth, 0))) {
                if (std::filesystem::exists(iso_path) && std::filesystem::exists(patch_path)) {
                    // Start Patching
                    if (patchThread.joinable()) patchThread.join();
                    patchThread = std::thread(RunPatchLogic, std::string(iso_path), std::string(patch_path), create_backup, &patchState);
                }
                else {
                    patchState.status = "Error: File not found!";
                }
            }

			//Status Message
            if (patchState.status != "Idle") {
                ImGui::Dummy(ImVec2(0.0f, 5.0f));
                float textW = ImGui::CalcTextSize(patchState.status.c_str()).x;
                ImGui::SetCursorPosX((windowWidth - textW) * 0.5f);

                if (patchState.wasSuccessful) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                else ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));

                ImGui::Text("%s", patchState.status.c_str());
                ImGui::PopStyleColor();
            }
        }

        ImGui::PopStyleColor(3);

        ImGui::End();

        // Render
        ImGui::Render();
        glViewport(0, 0, 800, 600);
        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    if (patchThread.joinable()) {
        patchThread.join();
    }

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