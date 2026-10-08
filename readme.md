#ODIN5
Odin5 is a game engine built in C++ that packages windowing, rendering, and audio, providing manipulatable entities to harness these faculties.

#Why?
Computers have always interested me. So did games. So why not make my own games? Then I realized I liked working at the bare metal, with nobody trying to tie me down in their abstractions; as such, I began working on my own custom game engines. This is iteration 3, which takes what I learned from the former engines to iteravely improve upon what and how I write code.

#What works now
Windowing and graphics both have interfaces set up, with proper compartmentialization structurally. Windowing works properly, graphics are work in progress. Current only GLFW and Vulkan are supported as backends.

#Design
I take full advantage of the explicit type-safety C++ provides. Using templates, constexpr, and compile time constraints, I can constrain a type to behave a certain way and hold certain data; as a result, I can create flexible interfaces to create consistent APIs no matter the backend. I also wrap integers as a unique type in the case that I do not wish to mix up conotations, meaning an ID for, say, a mesh cannot be confused with an ID for any other resource.

#Build system
Using Clang, C++23, Vulkan, OpenGL, GLFW, glm. vcpkg is the package manager. CMake + Ninja as the build system on top. Debug and release preset modes (.bat files provided, {mode}-build.bat and {mode}-run.bat). Shaders have a unique folder, and use slang; compile with the provided .bat file as well (shaders\build.bat).
Written and tested on a system using Windows 11, 7800x3d, and 7900xt.

#Yet to be implemented
Audio backend, certain backends, ECS, entity components, major rendering components
