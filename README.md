<p align="center">
  <img src="https://github.com/kimkulling/osre/blob/master/assets/Icons/osre_logo2.png" />
</p>

# OSRE - Just another Open-Source-Render-Engine
This is just another open-source render engine / game engine made by Kim Kulling. This is a playground project for me to work on my side projects.
Don't expect too much, I will not invest too much time into it. But If you are interested in playing around with 3D stuff feel free to discuss 
these things on Reggit or in the Discussion Forum.

# Build Status
[![Build status](https://github.com/kimkulling/osre/actions/workflows/cmake.yml/badge.svg)](https://github.com/kimkulling/osre/actions/workflows/cmake.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=kimkulling_osre&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=kimkulling_osre)
[![Documentation Status](https://readthedocs.org/projects/osre-doc/badge/?version=latest)](https://osre-doc.readthedocs.io/en/latest/?badge=latest)

# Supported Features: #

- Platform Features:
  - CMake-based build-environment
  - Multithreaded Render-Environment
  - Support for Windows and Linux
  - Supported Render APIs
    - OpenGL-API
- 2D-Rendering
  - Basic shapes
- 3D-Rendering
  - Multipass Forward Renderer
  - Font rendering
  - Instancing
  - Asset loading via assimp
    - More than 40 3D-formats supported thanks to Assimp
  - Basic lighting
  - Debug renderer

# Sample showcases
  - [How to get started with 3D](samples/00_HelloWorld)
  - [Model rendering](samples/01_ModelLoading)
  - [How to get started with 2D](samples/02_Demo2D)
  - [Terrain rendering](samples/04_terrain)

# Build instructions
- See [Build instruction in the Wiki](https://github.com/kimkulling/osre/wiki/Build)

# Documentation
- Please check [OSRE-Docs](https://osre-doc.readthedocs.io/en/latest/).

# Project activities #
![Alt](https://repobeats.axiom.co/api/embed/71f422c76a23d1da904af0e5c23de54df6c0d0b2.svg "Repobeats analytics image")

# Join out community
We have a discord server at [Discord](https://discord.gg/kqJQW5dQ)


# Questions
- Take a look into our [OSRE-Forum](https://github.com/kimkulling/osre/discussions)
- Please check our [Reddit-Community](https://www.reddit.com/r/osre/)

# Supported compilers
- Windows:
  - Visual Studio 2019
  - Visual Studio 2022
- Linux:
  - GCC
  - Clang

# OSRE-Ed
![OSRE-Ed](src/Editor/)

The engine provdes an 3D-Editor called OSRE-Ed. It is still experimental:
