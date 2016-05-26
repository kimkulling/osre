# OSRE - An Open Source Render Experience
Another open-source render experience. feel free to play around with it!

# Supported platforms:
- Windows: OpenGL-renderbackend
- Linux ( [![Build Status](https://travis-ci.org/kimkulling/osre.png)](https://travis-ci.org/kimkulling/osre) ): OpenGL-renderbackend

# How to start
## What you need to install
- A Linux or Windows-PC with a more or less modern GPU
- cmake to genreate the build environment for your platform ( see www.cmake.org )
- SDL2.0 ( see https://www.libsdl.org/download-2.0.php )

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
Generate a build environment of your choise:
> cmake CMakeLists.txt

for linux:
> make

for Windows: Use the generated Visual studio project files.

## Get involved
If you want to contribute just use the github project page or reach us under IRC:
server freenode, channel #osre.

## What is cooking
If you want to see what is currently under development or what is planned next you can take a look into our planning board: 
[OSRE Planning Board](https://trello.com/b/AJEAv98P/osre)

## Reference doc
Check our doxygen reference doc: http://kimkulling.github.io/osre/doc/html/index.html

# Samples
## Sample 1: Hello world
This example will show you how to create a windo and add a simple stage to it.
### [01-helloworld](https://github.com/kimkulling/osre/blob/master/samples/00_HelloWorld/)
