# README #

The K15 Engine 3D is a 3D Game Engine which is currently in heavy development.
It is the successor to my 2D Game Engine the [K15 Engine](https://bitbucket.org/FelixK15/k15-engine).

The engine is in development since late 2012 and has been refactored several times since then as this is a project I work on in my spare time to test various techniques related with game and engine design.

### Features ###

![platforms.png](https://bitbucket.org/repo/Kne54k/images/720827910-platforms.png)

Currently the engine supports the following features:

* Runs on Windows, Linux and Android (Ouya, iOS and Mac planned)
* Plugin based architecture to keep the engine core as light as possible
* Component based game object system
* Easy to use code reflection system (WIP)
* Serialization through code reflection
* Data driven through serialization
* Custom RTTI system
* Custom memory management system
* Allows for multithreaded applications through threadworker/threadpool system
* Comes with several tools e.g. for [enum name creation](https://bitbucket.org/FelixK15/enumparser) or automatic code generation
* Light core, doesn't depend on 'big' libraries like SDL or Qt for multiplatform features
* CMake compatible

## Content ###

This repository holds the following projects:

* K15 Engine Core (/engine/)
* K15 Math Library (/math/ - Basically a wrapper around glm)
* OpenGL <= 3.3. Renderer (/render_opengl/ - The ES renderer is obsolete and now part of the <= 3.3. renderer)
* Dependencies (/dependencies/ - listed below)
* Various Tools (/tools/)
* Test Gameplay plugins (/plugins/)

Only the engine core, math library and renderer will be part of the projects that will be generated via CMake.

### Dependencies ###

Currently the engine depends on the following libraries:

* enet          - network
* glew          - OpenGL Extension Wrangler Library used by the OpenGL renderer
* jsoncpp       - reading/writing json files for serialization (maybe will get replaced with cereal)
* libtiff       - loading tiff files
* tinyobjloader - loading obj files
* minizip/zlib  - read zip files
* tinythread    - multiplatform thread support in case C++11 is not available

There are more libraries in the /dependencies/ folder but they are either obsolete or will be implemented in the future.

### Planned ###

As the engine is in ongoing development there are still some major features missing I definitely want to implement for 1.0 release. The following features are currently only existing on my ToDo List:

* Physics using Bullet (via plugin to keep the core light)
* Network communication via enet
* Sophisticated debugging tools (memory/performance)
* Support for Mac, iOS and Ouya.
* DirectX 11 renderer
* Game Engine WYSIWYG Editor for Windows, Linux and Mac(probably not for 1.0)

### Work In Progress ###

At the moment I'm working on the implementation of the OpenGL renderer for versions <= 3.3.
For 1.0 only the OpenGL renderer will be released albeit for OpenGL >= 3.3 and <= 4.0 (this will probably end up in 2 separate renderers).