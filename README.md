# Saurobyte

Saurobyte is a 3D game engine currently under development. The goal of this engine is to
make it as easy to use as possible while maintaining a healthy amount of features. Using
modern features such as OpenGL 4.3 and C++11, inevitably making it less cross-platform.

**Please note:** Saurobyte is under heavy development, it is **NOT** in a condition to be used by anyone other than me (for development).

## Features
**Please note:** Saurobyte is under development so features may be added or removed

* Cross-platform support (Intended feature, but not tested thoroughly)
* Based on Entity/Component design
* Lua scripting (Parts of the engine is exposed to Lua)
* Config files in Lua (loading/saving)
* R*-trees for spatial partitioning
* 3D audio using OpenAL
* Global messaging system using the subscriber pattern

## Dependencies
Saurobyte depends on a few other libraries as well, which are dynamically linked to make
updating the engine more flexible. These dependencies are included in the source files on
Windows and MacOSX. Other operating systems and platforms are required to have the development versions of the dependencies installed on their system.

* OpenGL 4.3
* OpenAL Soft
* GLEW
* Sndfile
* Lua5.2
* SDL2
* SDL2_image

The engine also uses many C++11 features.
