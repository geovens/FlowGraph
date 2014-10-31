
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
	double CounterValue;  // ���������������ģʽ"bump"ʱʹ��

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
	bool HaveBeenOpened;  // �Ƿ��Ѵ򿪹�opencv��ʾ���ڡ�Ϊ�������˹��ر�
	std::wstring Mode;  // "normal"Ϊ��ͨ��"nontime"Ϊ��ʱ�����Զ�����������
	int ViewMode;  // 1Ϊ��ͼ��2Ϊ���
	bool ShowGrid;
	bool Hide;
	int Width;
	int Height;
	int Speed;
	double TopBoundary;  // ����õ����˹����õ�ͼ�Ͻ�ֵ��ʵ��������
	double BottomBoundary;  // ����õ����˹����õ�ͼ�½�ֵ��ʵ��������
	double UserSetTopBoundary;  // �˹����õ�ͼ�Ͻ�ֵ
	double UserSetBottomBoundary;  // �˹����õ�ͼ�½�ֵ
	double MaxVisibleValue;  // ������д��ʱ����ӷ�Χ�����ֵ
	double MinVisibleValue;  // ������д��ʱ����ӷ�Χ�����ֵ
	int P;
	int PPause;
	int* PShow;
	int LastUpdateTime;
	std::list<Curve> Curves;
	cv::Mat Dot;
	cv::Mat Back;  //�����֣�����
	//int UpdateFinished;
	double SampleInterval;  // ����������ʱģʽ"nontime"ʱʹ�ã��������
	double SampleCount;   // ����������ʱģʽ"nontime"ʱʹ��
	double SampleLength;  // ����������ʱģʽ"nontime"ʱʹ�ã�ÿ�������㻭�������س�
	int ValueFormat;  // ���ָ�ʽ���������۲�ģʽʱʹ��
	int AutoUpBottom;  // �Ƿ��Զ����½磬������ͼ�۲�ģʽʱʹ��
	int SaveCount;  // ��¼�Ա����ͼƬ������
	bool NeedPlotMesh;  // һ��֪ͨ�ػ���������Ϣ
	int DuplicatedCount;  // ��ͼ�����ƹ�����

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
//bool RunPaused;  //�Ƿ���ͣ��������ֵ
bool Paused;  // �Ƿ���ͣ��ʾ
bool Running = false;
int NextPosition;  // Ϊ���ô����Զ����ϵ������У���¼��һ�����ڵ�λ��
cv::Mat Color;