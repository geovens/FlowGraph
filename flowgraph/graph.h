
#pragma once
#include "tchar.h"
#include "time.h"
#include "windows.h"
#include "opencv2/opencv.hpp"
#include <list>

struct TypeGraph;

typedef struct TypeCurve
{
	std::wstring Name;
	TypeGraph* Graph;
	double Values[1920];
	double ValuesPause[1920];
	double* ValuesShow;
	std::wstring Mode;
	double MaxVisibleValue;
	double MinVisibleValue;
	double IncomeValue;
	double IncomeValuePause;
	double* IncomeValueShow;
	double CounterValue;  // 仅当计数冲击曲线模式"bump"时使用

	TypeCurve()
	{
		Name = L"default curve";
		for (int v = 0; v < 1920; v++)
			Values[v] = 0;
		ValuesShow = Values;
		IncomeValue = 0;
		IncomeValueShow = &IncomeValue;
	}
} Curve;

typedef struct TypeGraph
{
	std::wstring Name;
	bool HaveBeenOpened;  // 是否已打开过opencv显示窗口。为了区分人工关闭
	std::wstring Mode;  // "normal"为普通，"nontime"为非时间线性而是数据驱动
	int ViewMode;  // 1为流图，2为表格
	bool ShowGrid;
	bool Hide;
	int Width;
	int Height;
	int Speed;
	double TopBoundary;  // 计算得到或人工设置的图上界值，实际起作用
	double BottomBoundary;  // 计算得到或人工设置的图下界值，实际起作用
	double UserSetTopBoundary;  // 人工设置的图上界值
	double UserSetBottomBoundary;  // 人工设置的图下界值
	double MaxVisibleValue;  // 计算填写的时间可视范围内最大值
	double MinVisibleValue;  // 计算填写的时间可视范围内最大值
	int P;
	int PPause;
	int* PShow;
	int LastUpdateTime;
	std::list<Curve> Curves;
	cv::Mat Dot;
	cv::Mat Back;  //画文字，背景
	//int UpdateFinished;
	double SampleInterval;  // 仅当点驱动时模式"nontime"时使用，采样间隔
	double SampleCount;   // 仅当点驱动时模式"nontime"时使用
	double SampleLength;  // 仅当点驱动时模式"nontime"时使用，每个采样点画几个像素长
	int ValueFormat;  // 数字格式，仅当表格观察模式时使用
	int AutoUpBottom;  // 是否自动上下界，仅当流图观察模式时使用
	int SaveCount;  // 记录以保存成图片的数量
	bool NeedPlotMesh;  // 一个通知重画画布的消息
	int DuplicatedCount;  // 该图被复制过几次

	TypeGraph()
	{
		Name = L"default graph";
		HaveBeenOpened = false;
		Mode = L"normal";
		Hide = false;
		ViewMode = 1;
		ShowGrid = true;
		Width = 600;
		Height = 100;
		Speed = 200;
		UserSetBottomBoundary = 0;
		UserSetTopBoundary = 10;

		SampleInterval = 1;
		SampleCount = 0;
		SampleLength = 1;
		SaveCount = 0;
		NeedPlotMesh = false;
		ValueFormat = 0;
		AutoUpBottom = 1;
		LastUpdateTime;
		//Dot = cv::Mat::zeros(Height + 1, Width, CV_8UC3);
		P = 0;
		PShow = &P;
		//UpdateFinished = 1;
		DuplicatedCount = 0;
	}
} Graph;

void PlotMesh(Graph* graph, int viewmode);
void CalCurveMaxMin(Curve* curve);
void CalGraphMaxMin(Graph* curve);
void DrawViewMode1(Graph* thegraph);
void DrawViewMode2(Graph* thegraph);

std::list<Graph> Graphs;
bool exitthread;
//bool RunPaused;  //是否暂停接收新数值
bool Paused;  // 是否暂停显示
bool Running = false;
int NextPosition;  // 为了让窗口自动从上到下排列，记录下一个窗口的位置
cv::Mat Color;