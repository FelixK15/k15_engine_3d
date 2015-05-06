# README #

The K15 Engine 3D is a 3D Game Engine which is currently in heavy development.
It is the successor to my 2D Game Engine the [K15 Engine](https://bitbucket.org/FelixK15/k15-engine).

### Features ###

![platforms.png](https://bitbucket.org/repo/Kne54k/images/720827910-platforms.png)

Currently the engine supports the following features:

* Runs on Windows, Linux and Android (Ouya, iOS and Mac planned)
* Multithreaded OpenGL renderer
* Custom resource (asset) compiler
* Data driven through serialization
* Allows for multithreaded applications through threadworker/threadpool system
* Comes with several tools e.g. for [enum name creation](https://bitbucket.org/FelixK15/enumparser) or automatic code generation
* Light core, doesn't depend on 'big' libraries like SDL or Qt for multiplatform features
* CMake build files

* (Deprecated) Custom RTTI system
* (Deprecated) Custom memory management system

## Content ###

This repository holds the following projects:

* K15 Engine Core (/core/...)
* K15 Math Library (/core/math/ - Basically a wrapper around glm)
* K15 Multithreaded Renderer (/core/renderer/)
* Dependencies (/dependencies/ - listed below)
* Various Tools (/tools/)

Only the engine core, math library and renderer will be part of the projects that will be generated via CMake.

### Dependencies ###

Currently the engine depends on the following libraries:

* assimp		- load various mesh formats
* enet          - network
* jsoncpp       - reading/writing json files for serialization (maybe will get replaced with cereal)
* minizip/zlib  - read zip files
* luajit		- super fast lua vm
* squish		- dxt texture compression

There are more libraries in the /dependencies/ folder but they are either deprecated and not longer used or will be implemented in the future.

### Planned ###

As the engine is in ongoing development there are still some major features missing I definitely want to implement for 1.0 release. The following features are currently only existing on my ToDo List:

* Physics using Bullet (via plugin to keep the core light)
* Network communication via enet
* Sophisticated debugging tools (memory/performance)
* Support for Mac, iOS and Ouya.
* DirectX 11 renderer
* Game Engine WYSIWYG Editor for Windows, Linux and Mac(probably not for 1.0)