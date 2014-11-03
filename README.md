# FlowGraph

FlowGraph is a visual variable debug tool that show variable values against time with graphs. I use FlowGraph to do debug things in other projects and benefit a lot from it.

![screenshot](https://raw.githubusercontent.com/geovens/FlowGraph/master/t165.gif)

To use FlowGraph in your C++ project, include flowgraph.h, link flowgraph.lib and use flowgraph.dll. See flowgraph.h for APIs. The most important API is FlowValue(), play around with it to start.

To use FlowGraph in your C#/VB project, add a reference of flowgraph.net.dll in your project and copy flowgraph.dll beside your excutables. All APIs are static functions in FlowGraph class under namespace FlowGraphNet. See sample_csharp project as an example on how to use.

FlowGraph uses OpenCV 3.0.0 library (with OpenGL). To build FlowGraph, you need to link OpenCV 3.0.0 (with OpenGL) static library files which are not included in this repository. 
FlowGraph uses a modified highgui module. The binary file of the modified highgui module is provided in this directory. The aim of the modification is to disable vsync in OpenGL so the drawing could be faster. The modified highgui is optional.
