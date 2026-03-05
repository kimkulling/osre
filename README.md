<p align="center">
  <img src="https://github.com/kimkulling/osre/blob/master/assets/Icons/osre_logo2.png" />
</p>

# OSRE - Open Source Render Engine

An experimental render engine to explore render engine architecture and modern graphics techniques.

## Build Status
[![Build status](https://github.com/kimkulling/osre/actions/workflows/cmake.yml/badge.svg)](https://github.com/kimkulling/osre/actions/workflows/cmake.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=kimkulling_osre&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=kimkulling_osre)
[![Documentation Status](https://readthedocs.org/projects/osre-doc/badge/?version=latest)](https://osre-doc.readthedocs.io/en/latest/?badge=latest)

## Features

**Platform:**
- CMake-based build system
- Multithreaded rendering
- Windows & Linux support
- OpenGL API

**2D Rendering:**
- Basic shapes
- Font rendering

**3D Rendering:**
- Multipass forward renderer
- Instancing support
- Asset loading via Assimp (40+ formats)
- Basic lighting
- Debug renderer

## Samples
- [3D Hello World](samples/00_HelloWorld)
- [Animation](samples/05_Animation)
- [Model Loading](samples/01_ModelLoading)
- [2D Demo](samples/02_Demo2D)
- [Terrain Rendering](samples/04_terrain)

## Documentation
- [Build Instructions](https://github.com/kimkulling/osre/wiki/Build)
- [OSRE Docs](https://osre-doc.readthedocs.io/en/latest/)

## Community
- [Discord Server](https://discord.gg/kqJQW5dQ)
- [GitHub Discussions](https://github.com/kimkulling/osre/discussions)
- [Reddit Community](https://www.reddit.com/r/osre/)

## Supported Compilers
- **Windows:** Visual Studio 2019/2022/2026
- **Linux:** GCC, Clang

## OSRE-Ed

The engine includes an experimental 3D editor called OSRE-Ed with basic scene editing capabilities.

![Project Activity](https://repobeats.axiom.co/api/embed/71f422c76a23d1da904af0e5c23de54df6c0d0b2.svg)
