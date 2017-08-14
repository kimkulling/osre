# OSRE - An Open Source Render Experience

Another open-source render experience. feel free to play around with it!
Discuss with us: [![Join the chat at https://gitter.im/kimkulling/osre](https://badges.gitter.im/kimkulling/osre.svg)](https://gitter.im/kimkulling/osre?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

# Supported platforms:
- Windows: 
  + OpenGL-Renderbackend
  + Vulkan-Renderbackend
- Linux ( [![Build Status](https://travis-ci.org/kimkulling/osre.png)](https://travis-ci.org/kimkulling/osre) ): 
  + OpenGL-renderbackend

# Static code analysis
- Coverity Status: [![Coverity Scan Build Status](https://scan.coverity.com/projects/13242/badge.svg)](https://scan.coverity.com/projects/kimkulling-osre)

# How to start
## What you need to install it
- A Linux or Windows-PC with a more or less modern GPU
- cmake to genreate the build environment for your platform ( see www.cmake.org )
- SDL2.0 ( see https://www.libsdl.org/download-2.0.php )
- The rest comes out of the box!

## How to build it
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
[Special build instructions for Windows](docs/Build_Windows.md)

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
   + Multithreaded renderer
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
- If you want to see what is currently under development or what is planned next you can take a look into our planning board: 
[OSRE Planning Board](https://trello.com/b/AJEAv98P/osre)

## Reference doc
Check our doxygen reference doc: http://kimkulling.github.io/osre/doc/html/index.html

# Samples
## Sample 1: Hello world
This example will show you how to create a windo and add a simple stage to it.
### [01-helloworld](https://github.com/kimkulling/osre/blob/master/samples/00_HelloWorld/)

# Running the render-test-suite
After building the engine you can find the render-tests in the binary-folder. To run the tests just run:
- for Windows:
```
> .\osre-rendertest.exe
```
- for Linux:
```
> ./osre-rendertest
```

You can terminate the current test-fixure by pressing any key...
The render-test will show you basics like rendering primitives, animating a render-object,
usage of material rendering, using our debug font render-engine, use the instancing or updating 
render-buffer.
  
