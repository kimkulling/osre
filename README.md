# OSRE - Another Open Source Render Engine

Just another open-source renderer. So feel free to play around with it!

Some features:
- Multithreaded rendering + instanced-rendering
- Platform abstraction-layer
- OS-independent task-support to make multi-threaded rendering more easy
- Model-import provided by Asset-Importer-Lib ( see https://github.com/assimp/assimp )
- Bitmap-Font-Rendering
- Simple scenegraph-module
- Simple UI-module providing Panels Buttons and some more stuff
- CMake base build environment

Discussions: [![Join the chat at https://gitter.im/kimkulling/osre](https://badges.gitter.im/kimkulling/osre.svg)](https://gitter.im/kimkulling/osre?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

# Supported platforms:
- Windows ( [![Build status](https://ci.appveyor.com/api/projects/status/q2ivokgqcit9k5w1/branch/master?svg=true)](https://ci.appveyor.com/project/kimkulling/osre/branch/master) ): 
  + OpenGL-Renderbackend
  + Vulkan-Renderbackend
- Linux ( [![Build Status](https://travis-ci.org/kimkulling/osre.png)](https://travis-ci.org/kimkulling/osre) ): 
  + OpenGL-Renderbackend

# Static code analysis
- Coverity Status: [![Coverity Scan Build Status](https://scan.coverity.com/projects/13242/badge.svg)](https://scan.coverity.com/projects/kimkulling-osre)

# Getting started
## What you need to install it?
- A Linux or Windows-PC with a more or less modern GPU
- cmake to genreate the build environment for your platform ( see www.cmake.org )
- SDL2.0 ( see https://www.libsdl.org/download-2.0.php )
- The rest comes out of the box!

## Build the engine from the source
Clone the repo via git from our project space on github.
```
> cd projects
> git clone https://github.com/kimkulling/osre.git
```
Now update all used submodule, which are usep by OSRE:
```
> git submodule init
> git submodule update -r
```
Generate a build environment of your choise ( you can choose your own build environment by selecting it
via -G )
```
> cmake CMakeLists.txt [-G <makefile generator>]
```
[Special build instructions for Windows](doc/Build_Windows.md)

## Run the tests
Navigate to bin\debug or bin\release and run our test suite for rendering:
```
> bin\debug\osre_rendertest
```

### For Linux:
```
> make -j4
```

### For Windows: 
Use the generated Visual studio project files.

## Featurelist
- OpenGL4.x-Renderer:
   + Multithreaded render-concept.
   + Primitive rendering
   + Texture mapping
   + Instanced rendering
   + Font rendering

- Vulkan-Renderer:
   + Experimental

## Get involved
If you want to contribute just use the github project page or reach us via Gitter:
[![Join the chat at https://gitter.im/kimkulling/osre](https://badges.gitter.im/kimkulling/osre.svg)](https://gitter.im/kimkulling/osre?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

## What is cooking?
- Working on Vulkan support
- Working on editor + MeshViewer

## Reference doc
Check our doxygen reference doc: http://kimkulling.github.io/osre/doc/html/index.html

# Samples
## Sample 1: Hello world
This example will show you how to create a window and add a simple stage to it.

[00-helloworld](https://github.com/kimkulling/osre/blob/master/samples/00_HelloWorld/)


## Sample 2: Model Loader
This example will show you how to create a window, load a model using assimp and render it into a new created stage.

[01-ModelLoading](https://github.com/kimkulling/osre/blob/master/samples/01_ModelLoading/)


## Sample 3: UI-Demo
This example will show you how to create a simple dialog and add your own callbacks.

[02-UIDemo](https://github.com/kimkulling/osre/tree/master/samples/02_UIDemo)
  
