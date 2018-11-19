/**
@mainpage OSRE - An Open-Source-Render-Experiment

@section intro Introduction

OSRE is a shortcut for Open Source Render Engine, an engine to do some experiments.

@section arch The base Architecture
The engine code is separated into several sub-folders called modules. Each module has
its own source folder containing the cpp-files with the private header files. 
<br>
The public interface to a module are located in the folder:

include/<ModuleName>

All public interfaces shall be used to access the API of the modules, the privates headers
shall not be visible at all.

@section common The Common Module

@section debug The Debugger-Module

@section props The Property-Module

@section threads The Threading-Module

@section assets The Assets-Module

@section render The Render-Module

@section scene The Scene-Module

@section app The App-Module

*/
