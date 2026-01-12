# VSPatcher
An open-source, universal patch utility for Vagrant Story. This project was developed using C++17 and the following libraries and tools:
- SDL2 – Used for window management, event handling.
- SDL_mixer – For playing background music.
- OpenGL – For rendering graphics.
- Dear ImGui – Immediate mode GUI for rendering the patcher UI.
- Visual Studio – Used as the development environment.

Beyond its function as a patching utility, this project aims to serve as a "hub" for the Vagrant Story modding community. 

My goal is to aggregate and promote the incredible work done by other developers, to ensure they remain accessible to new players and veterans alike. 

You can find a curated list of these community projects at the bottom of this document.

# Description:
This tool allows you to easily apply PPF patches to your Vagrant Story 
(or other PS1) game files. It handles the patching process automatically 
and includes a safety backup feature.

# Key Features:
- Full source code available on this repository.
- Automatic backup creation (safeguards your original file)
- Simple graphical interface & music player for our nostalgic track :)
- Works with standard .bin/.cue rips and .iso files.

# Instructions:
1. Extract all files in this ZIP to a folder.
2. Run "VSPatcher.exe".
3. Click "Browse ISO" and select your game file (.bin, .iso, .img).
4. Click "Browse Patch" and select your patch file (.ppf).
5. Ensure "Create a Backup" is checked if you want to keep your original file.
6. Click "Apply Patch".

# Troubleshooting:
- If the app doesn't open, ensure all DLL files are in the same folder as the .exe.
- If music doesn't play, check your system volume mixer.

# Credits:
- Developed by redd2213
- UI powered by Dear ImGui
- Audio/Video backend by SDL2
- Patch logic inspired by PPF-O-MATIC

# Disclaimer:
This software is provided "as is", without warranty of any kind. 
Always keep a backup of your game files before patching.

# Vagrant Story Mods & Tools List

Here is a curated list of mods and tools for Vagrant Story:

- [VS Zenith - A comprehensive mod](https://vszenith.wordpress.com/) - by vagrantstory2000
- [VSTrack - Real-time tracker](https://github.com/tomasz-rozanski/vstrack) - by Tomasz Różański
- [Vagrant Story Lua Script](https://github.com/notdodgeball/vagrant-story-lua-script) - by notdodgeball
- [VSViewer](https://github.com/MercurialForge/VSViewer) - by MercurialForge
- [VSTools](https://github.com/morris/vstools) - by Morris
- [Vagrant Story Unity Parser](https://github.com/korobetski/Vagrant-Story-Unity-Parser) - by korobetski
- [VSStatsEditor](https://github.com/jaythreee/VSStatsEditor) - by jaythreee
- [GodHands](https://github.com/collinsmichael/GodHands/) - by collinsmichael
- [Vagrant Story Randomizer](https://github.com/ChaoticBrave/VagrantStoryRandomizer) - by ChaoticBrave
- [Vagrant Story Tools & Forum Discussion](https://ffhacktics.com/smf/index.php?topic=8494.0) - by Valendian
