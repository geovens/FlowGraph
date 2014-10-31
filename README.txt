# FlowGraph

FlowGraph uses OpenCV 3.0.0 library (with OpenGL). To build FlowGraph, you need to link OpenCV 3.0.0 (with OpenGL) static library files. 
FlowGraph uses a modified highgui module. The binary file of the modified highgui module is provided in this directory. The aim of the modification is to disable vsync in OpenGL so the drawing would be faster.
