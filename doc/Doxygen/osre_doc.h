/**
@mainpage OSRE - An Open-Source-Render-Experiment

@section intro Introduction

OSRE is a shortcut for Open Source Render Engine, an engine to do some experiments.

@section arch The base Architecture
The engine code is separated into several sub-folders called modules. Each module has
its own source folder containing the cpp-files with the private header files. 
<br>
The public interface to a module are located in the folder:

include/ModuleName

All public interfaces shall be used to access the API of the modules, the privates headers
shall not be visible at all.

@section common The Common Module
The common-module provide a couple of widely-used services and classes. The base-object for all 
derived objects can be found here.

@section debug The Debugger-Module
The DEbugging-module provided some nice assert-implementations, which will show you a typical 
assertion-behavior plus some nice log for the OSRE-environment.

@section props The Property-Module

@section threads The Threading-Module
The provide a multi-threaded render-environment the OSRE provides the concept of tasks. A task is 
just an active object, running in its own thread and has a async-event-queue to enqueue events onto it.

@section assets The Assets-Module

@section render The Render-Module

@section scene The Scene-Module

@section app The App-Module

*/
