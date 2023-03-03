# roguelike-game

## Initial setup
- Download the repo
- Check that the C++ integration works (C++ build was tested on MacOS 'arm64')):
    - Install python 3.x and latest version of pip
    - Install scons building manager: `pip install scons`
    - `cd client/godot-cpp` 
    - `scons platform=<platform> generate_bindings=yes bits=64` - will build the godot-cpp source files for your platform
    - `cd client/godot` 
    - `scons platform=<platform> arch=<arch>` - specify arch for macos(arm64, x86_64). (Need to check the tutorials to get the feeling of the structure for C++ integration)
    - Now when open a project in Godot you should see floating godot logo 
- After that you can delete everything inside `client/godot/game` folder and start working on your project (we can attach C++ integration when needed)


## Helpful tutorial:
- https://docs.godotengine.org/en/stable/tutorials/scripting/gdnative/gdnative_cpp_example.html - (godot docs) C++ intergration
