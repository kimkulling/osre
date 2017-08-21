# How to build OSRE on Windows

You need to add one ENV-var called SDL_PATH showing to the place where your SDL2-installation is located.

Doing this in the console:
```
set SDL_PATH=<Your_SDL_installation>
```
Or do this globally: [Introducing a global ENV-variable](https://stackoverflow.com/questions/3803581/setting-a-system-environment-variable-from-a-windows-batch-file)

Now you can call CMake as usual. A solution for your visual project will be generated.
