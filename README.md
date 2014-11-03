# FlowGraph

FlowGraph is a visual debug tool that show variable values against time. I use FlowGraph in my other projects to debug, and benefit a lot from the convenience it brings.

![screenshot](https://raw.githubusercontent.com/geovens/FlowGraph/master/t165.gif)

To use FlowGraph in your C++ project, include flowgraph.h, link flowgraph.lib and use flowgraph.dll. See flowgraph.h for APIs. The most important API is FlowValue(std::wstring variablename, double value), which could do most of the works even if used solo.

To use FlowGraph in your C#/VB project, add a reference of flowgraph.net.dll in your project and copy flowgraph.dll beside your excutables. All APIs are static functions in 'FlowGraph' class under 'FlowGraphNet' namespace. See 'sample_csharp' project as an example on how to use.

FlowGraph uses OpenCV 3.0.0. To build FlowGraph, you need to link OpenCV 3.0.0 library files built with OpenGL flag enabled, which are not included in this repository. 
A modified (unoffical) version of OpenCV highgui module binary is provided in 'lib/opencv2' folder. It is recommanded to link this modified version instead of the offical version, but this is optional and up to you. The aim of the modification is to disable vsync in OpenGL so that drawing could be faster.
