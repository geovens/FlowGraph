
#pragma once
#include <string>
#include "tchar.h"

extern "C" __declspec(dllexport) void FlowSetWidthHeight(const wchar_t* graphname, int width, int height);

extern "C" __declspec(dllexport) void FlowSetBoundry(const wchar_t* graphname, double minvalue, double maxvalue);

extern"C" __declspec(dllexport) void FlowSetSpeed(const wchar_t* graphname, int speed);

extern "C" __declspec(dllexport) void FlowSetViewMode(const wchar_t* graphname, int viewmode);

extern "C" __declspec(dllexport) void FlowCreateGraph(const wchar_t* graphname, const wchar_t* mode = L"normal", int param = 1);

extern "C" __declspec(dllexport) void FlowDuplicateGraph(const wchar_t* srcname, const wchar_t* dstname);

extern "C" __declspec(dllexport) void FlowHideGraph(const wchar_t* graphname);

extern "C" __declspec(dllexport) void FlowMoveGraph(const wchar_t* graphname, int x, int y);

extern "C" __declspec(dllexport) void FlowSaveGraph(const wchar_t* graphname = L"TheFirstGraph");

extern "C" __declspec(dllexport) void FlowCreateCurve(const wchar_t* graphname, const wchar_t* curvename, const wchar_t* mode = L"normal");

//extern __declspec(dllexport) void FlowCreateCurve(std::wstring curvename);

//extern __declspec(dllexport) void FlowValue(std::wstring graphname, std::wstring curvename, double value);

extern "C" __declspec(dllexport) void FlowValue(const wchar_t* curvename, double value, const wchar_t* newgraphname = L"Debug");

extern "C" __declspec(dllexport) void FlowValueIncrease(const wchar_t* curvename, double value, const wchar_t* newgraphname = L"Debug");

extern "C" __declspec(dllexport) void FlowStart();

extern "C" __declspec(dllexport) void FlowPause(bool pause);

extern "C" __declspec(dllexport) void FlowStop();

extern "C" __declspec(dllexport) void FlowTest();