// dots.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "flowgraph.h"
#include "graph.h"

DWORD WINAPI ThreadShow(LPVOID lpParameter)
{
	cv::Mat Black = cv::Mat::zeros(300, 300, CV_8UC4);
	while (!exitthread)
	{
		for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
		{
			Graph* thegraph = &(*it);

			if (thegraph->Hide)
				continue;

			char chars[100];
			wcstombs_s(NULL, chars, thegraph->Name.c_str(), 100);
			HWND wind = FindWindow(NULL, thegraph->Name.c_str());
			if (wind == NULL)
			{
				if (thegraph->HaveBeenOpened)
				{
					FlowHideGraph(thegraph->Name.c_str());
					break;
				}
				else
				{
					cv::namedWindow(chars, cv::WINDOW_OPENGL);
					cv::resizeWindow(chars, it->Width, it->Height + 41);
					cv::moveWindow(chars, 100, NextPosition);
					NextPosition += it->Height + 41 + 40;
					if (NextPosition > 1000)
						NextPosition = 1000;
					thegraph->HaveBeenOpened = true;
				}
			}
			if (thegraph->ViewMode == 1)
			{
				if (wind != NULL)
				{
					RECT rect;
					GetClientRect(wind, &rect);
					if (rect.right > 10 && rect.bottom > 50)
					{
						if (thegraph->Height != rect.bottom - 42)
							thegraph->Height = rect.bottom - 42;
						if (thegraph->Width != rect.right - 1)
							thegraph->Width = rect.right - 1;
					}
				}
			}
			else if (thegraph->ViewMode == 2)
			{
				if (wind != NULL)
				{
					RECT rect;
					GetClientRect(wind, &rect);
					if (rect.right != thegraph->Dot.cols || rect.bottom != thegraph->Dot.rows + 1)
						cv::resizeWindow(chars, thegraph->Dot.cols, thegraph->Dot.rows + 1);
				}
			}

			if (thegraph->ViewMode == 1)
			{
				DrawViewMode1(thegraph);
			}
			else if (thegraph->ViewMode == 2)
			{
				DrawViewMode2(thegraph);
			}

			cv::imshow(chars, thegraph->Dot);
		}

		//do
		{
			int k = cv::waitKey(10);
			if (k == ' ')
			{
				FlowPause(!Paused);
			}
			else if (k == '*')
			{
				for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
				{
					it->Hide = false;
				}
			}
			else if (k == '\t' || k == 'f' || k == 7929856 || k == 'b' || k == '+' || k == 'g')
			{
				wchar_t foretitle[100];
				HWND hwnd = GetForegroundWindow();
				GetWindowText(hwnd, (LPWSTR)foretitle, 100);
				for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
				{
					if (it->Name == foretitle)
					{
						if (k == '\t')
						{
							it->ViewMode++;
							if (it->ViewMode > 2)
								it->ViewMode = 1;
							if (it->ViewMode == 1)
							{
								char chars[100];
								wcstombs_s(NULL, chars, foretitle, 100);
								cv::resizeWindow(chars, it->Width, it->Height + 41);
							}
						}
						else if (k == 'f')
						{
							it->ValueFormat++;
							if (it->ValueFormat > 3)
								it->ValueFormat = 0;
						}
						else if (k == 7929856)
						{
							FlowSaveGraph(it->Name.c_str());
						}
						else if (k == 'b')
						{
							if (it->ViewMode == 1)
							{
								it->AutoUpBottom = 1 - it->AutoUpBottom;
							}
						}
						else if (k == '+')
						{
							it->DuplicatedCount++;
							wchar_t copychar[3];
							_itow_s(it->DuplicatedCount, copychar, 10);
							FlowDuplicateGraph(it->Name.c_str(), (it->Name + L" copy " + copychar).c_str());
							break;
						}
						else if (k == 'g')
						{
							it->ShowGrid = !it->ShowGrid;
							PlotMesh(&(*it), it->ViewMode);
						}
					}
				}
			}
		} 
		//while (Paused && !exitthread);
	}
	cv::destroyAllWindows();
	return 0;
}

void DrawViewMode1(Graph* thegraph)
{
	if (thegraph->Width != thegraph->Dot.cols || thegraph->Height + 1 + 40 != thegraph->Dot.rows)
	{
		thegraph->Dot = cv::Mat::zeros(thegraph->Height + 1 + 40, thegraph->Width, CV_8UC4);
		thegraph->Back = cv::Mat::zeros(thegraph->Height + 1 + 40, thegraph->Width, CV_8UC4);
		PlotMesh(thegraph, 1);
	}
	if (thegraph->NeedPlotMesh)
	{
		PlotMesh(thegraph, 1);
	}
	if (thegraph->AutoUpBottom == 1)
	{
		CalGraphMaxMin(thegraph);
		if (thegraph->MaxVisibleValue != thegraph->BottomBoundary || thegraph->MinVisibleValue != thegraph->TopBoundary)
		{
			if (thegraph->MinVisibleValue < thegraph->MaxVisibleValue)
			{
				thegraph->BottomBoundary = thegraph->MinVisibleValue;
				thegraph->TopBoundary = thegraph->MaxVisibleValue;
			}
		}
	}
	else if (thegraph->AutoUpBottom == 0)
	{
		thegraph->BottomBoundary = thegraph->UserSetBottomBoundary;
		thegraph->TopBoundary = thegraph->UserSetTopBoundary;
	}

	memcpy(thegraph->Dot.data, thegraph->Back.data, thegraph->Dot.cols * thegraph->Dot.rows * 4);

	// 写上下界数值
	char minytext[20];
	char maxytext[20];
	if (thegraph->ValueFormat == 0)
	{
		sprintf_s(minytext, "%d", (int)thegraph->BottomBoundary);
		sprintf_s(maxytext, "%d", (int)thegraph->TopBoundary);
	}
	else if (thegraph->ValueFormat == 1)
	{
		sprintf_s(minytext, "%.1lf", thegraph->BottomBoundary);
		sprintf_s(maxytext, "%.1lf", thegraph->TopBoundary);
	}
	else if (thegraph->ValueFormat == 2)
	{
		sprintf_s(minytext, "%.2lf", thegraph->BottomBoundary);
		sprintf_s(maxytext, "%.2lf", thegraph->TopBoundary);
	}
	else
	{
		sprintf_s(minytext, "%.1e", thegraph->BottomBoundary);
		sprintf_s(maxytext, "%.1e", thegraph->TopBoundary);
	}
	cv::putText(thegraph->Dot, minytext, cv::Point(thegraph->Dot.cols - 10 * (int)strlen(minytext) - 20, thegraph->Back.rows - 6), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(120, 120, 120, 0));
	cv::putText(thegraph->Dot, maxytext, cv::Point(thegraph->Dot.cols - 10 * (int)strlen(maxytext) - 20, 15), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(120, 120, 120, 0));

	// 画曲线
	int c = 0;
	for (std::list<Curve>::iterator it = thegraph->Curves.begin(); it != thegraph->Curves.end(); it++)
	{
		c++;
		Curve* thecurve = &(*it);

		int pa, pb;
		pa = *thegraph->PShow;
		for (int i = thegraph->Width - 1; i >= 0; i--)
		{
			pa--;
			if (pa < 0) pa += 1920;
			pb = pa - 1;
			if (i == 0) pb = pa;
			if (pb < 0) pb += 1920;
			double abmin = std::min(thecurve->ValuesShow[pa], thecurve->ValuesShow[pb]);
			double abmax = std::max(thecurve->ValuesShow[pa], thecurve->ValuesShow[pb]);
			if (false)
			{
				if (abmin < thegraph->BottomBoundary)
					abmin = thegraph->BottomBoundary;
				if (abmin > thegraph->TopBoundary)
					abmin = thegraph->TopBoundary;
				if (abmax < thegraph->BottomBoundary)
					abmax = thegraph->BottomBoundary;
				if (abmax > thegraph->TopBoundary)
					abmax = thegraph->TopBoundary;
			}
			int hmin = (int)((abmin - thegraph->BottomBoundary) / (thegraph->TopBoundary - thegraph->BottomBoundary) * thegraph->Height);
			int hmax = (int)((abmax - thegraph->BottomBoundary) / (thegraph->TopBoundary - thegraph->BottomBoundary) * thegraph->Height);
			int tH = thegraph->Height;
			for (int h = hmin; h <= hmax; h++)
			if (h >= 0 && h <= tH)
			{
				//thegraph->Dot.at<cv::Vec4b>(tH + 20 - h, i) = Color.at<cv::Vec4b>(c, 0);
				cv::Vec4b* pixel = &thegraph->Dot.at<cv::Vec4b>(tH + 20 - h, i);
				if ((*pixel)[3] == 0)
					*pixel = Color.at<cv::Vec4b>(c, 0);
				else
					*pixel = *pixel / 2 + Color.at<cv::Vec4b>(c, 0) / 2;
				(*pixel)[3]++;
			}
		}
	}
}

void DrawViewMode2(Graph* thegraph)
{
	if (500 != thegraph->Dot.cols || thegraph->Curves.size() * 25 + 40 != thegraph->Dot.rows)
	{
		thegraph->Dot = cv::Mat::zeros((int)thegraph->Curves.size() * 25 + 40, 500, CV_8UC4);
		thegraph->Back = cv::Mat::zeros((int)thegraph->Curves.size() * 25 + 40, 500, CV_8UC4);
		PlotMesh(thegraph, 2);
	}
	if (thegraph->NeedPlotMesh)
	{
		PlotMesh(thegraph, 2);
	}

	CalGraphMaxMin(thegraph);
	memcpy(thegraph->Dot.data, thegraph->Back.data, thegraph->Dot.cols * thegraph->Dot.rows * 4);
	int c = 0;
	for (std::list<Curve>::iterator it = thegraph->Curves.begin(); it != thegraph->Curves.end(); it++)
	{
		c++;
		Curve* thecurve = &(*it);
		cv::Scalar color = cv::Scalar(Color.at<cv::Vec4b>(c, 0)[0], Color.at<cv::Vec4b>(c, 0)[1], Color.at<cv::Vec4b>(c, 0)[2], 0);
		char valuetext[20];
		char maxtext[20];
		char mintext[20];
		if (thegraph->ValueFormat == 0)
		{
			sprintf_s(valuetext, "%d", (int)*thecurve->IncomeValueShow);
			sprintf_s(maxtext, "%d", (int)thecurve->MaxVisibleValue);
			sprintf_s(mintext, "%d", (int)thecurve->MinVisibleValue);
		}
		else if (thegraph->ValueFormat == 1)
		{
			sprintf_s(valuetext, "%.1lf", *thecurve->IncomeValueShow);
			sprintf_s(maxtext, "%.1lf", thecurve->MaxVisibleValue);
			sprintf_s(mintext, "%.1lf", thecurve->MinVisibleValue);
		}
		else if (thegraph->ValueFormat == 2)
		{
			sprintf_s(valuetext, "%.2lf", *thecurve->IncomeValueShow);
			sprintf_s(maxtext, "%.2lf", thecurve->MaxVisibleValue);
			sprintf_s(mintext, "%.2lf", thecurve->MinVisibleValue);
		}
		else
		{
			sprintf_s(valuetext, "%.1e", *thecurve->IncomeValueShow);
			sprintf_s(maxtext, "%.1e", thecurve->MaxVisibleValue);
			sprintf_s(mintext, "%.1e", thecurve->MinVisibleValue);
		}
		cv::putText(thegraph->Dot, valuetext, cv::Point(210, c * 25 - 7 + 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, color);
		cv::putText(thegraph->Dot, mintext, cv::Point(310, c * 25 - 7 + 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, color);
		cv::putText(thegraph->Dot, maxtext, cv::Point(410, c * 25 - 7 + 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, color);
	}
}

void DefineColor()
{
	Color = cv::Mat::zeros(11, 1, CV_8UC4);
	Color.at<cv::Vec4b>(0, 0)[0] = 0;
	Color.at<cv::Vec4b>(0, 0)[1] = 0;
	Color.at<cv::Vec4b>(0, 0)[2] = 0;
	//红
	Color.at<cv::Vec4b>(1, 0)[0] = 50;
	Color.at<cv::Vec4b>(1, 0)[1] = 50;
	Color.at<cv::Vec4b>(1, 0)[2] = 255;
	//草绿
	Color.at<cv::Vec4b>(2, 0)[0] = 20;
	Color.at<cv::Vec4b>(2, 0)[1] = 220;
	Color.at<cv::Vec4b>(2, 0)[2] = 120;
	//篮
	Color.at<cv::Vec4b>(3, 0)[0] = 255;
	Color.at<cv::Vec4b>(3, 0)[1] = 150;
	Color.at<cv::Vec4b>(3, 0)[2] = 40;
	//黄
	Color.at<cv::Vec4b>(4, 0)[0] = 0;
	Color.at<cv::Vec4b>(4, 0)[1] = 220;
	Color.at<cv::Vec4b>(4, 0)[2] = 250;
	//粉
	Color.at<cv::Vec4b>(5, 0)[0] = 200;
	Color.at<cv::Vec4b>(5, 0)[1] = 120;
	Color.at<cv::Vec4b>(5, 0)[2] = 255;
	//青
	Color.at<cv::Vec4b>(6, 0)[0] = 200;
	Color.at<cv::Vec4b>(6, 0)[1] = 200;
	Color.at<cv::Vec4b>(6, 0)[2] = 20;
	//深蓝
	Color.at<cv::Vec4b>(7, 0)[0] = 255;
	Color.at<cv::Vec4b>(7, 0)[1] = 80;
	Color.at<cv::Vec4b>(7, 0)[2] = 0;
	//橙
	Color.at<cv::Vec4b>(8, 0)[0] = 20;
	Color.at<cv::Vec4b>(8, 0)[1] = 140;
	Color.at<cv::Vec4b>(8, 0)[2] = 220;
	//绿
	Color.at<cv::Vec4b>(9, 0)[0] = 0;
	Color.at<cv::Vec4b>(9, 0)[1] = 140;
	Color.at<cv::Vec4b>(9, 0)[2] = 20;
	//紫
	Color.at<cv::Vec4b>(10, 0)[0] = 250;
	Color.at<cv::Vec4b>(10, 0)[1] = 0;
	Color.at<cv::Vec4b>(10, 0)[2] = 250;
}

void PlotMesh(Graph* graph, int viewmode)
{
	for (int i = 0; i < 20; i++)
	for (int j = graph->Dot.cols - 1; j >= 0; j--)
		graph->Back.at<cv::Vec4b>(i, j) = cv::Vec4b(15, 15, 15);
	for (int i = graph->Dot.rows - 20; i < graph->Dot.rows; i++)
	for (int j = graph->Dot.cols - 1; j >= 0; j--)
		graph->Back.at<cv::Vec4b>(i, j) = cv::Vec4b(15, 15, 15);
	for (int i = 20; i < graph->Dot.rows - 20; i++)
	for (int j = graph->Dot.cols - 1; j >= 0; j--)
		graph->Back.at<cv::Vec4b>(i, j) = cv::Vec4b(0, 0, 0, 0);

	if (viewmode == 1)
	{
		if (graph->ShowGrid)
		{
			for (int j = graph->Width - 1; j >= 0; j -= graph->Speed)
				cv::line(graph->Back, cv::Point(j, 20), cv::Point(j, graph->Height + 20), cv::Scalar(60, 60, 60, 0));
			cv::line(graph->Back, cv::Point(0, graph->Height / 2 + 20), cv::Point(graph->Width - 1, graph->Height / 2 + 20), cv::Scalar(80, 80, 80, 0));
		}
		cv::line(graph->Back, cv::Point(0, 20), cv::Point(graph->Width - 1, 20), cv::Scalar(80, 80, 80, 0));
		cv::line(graph->Back, cv::Point(0, graph->Height + 20), cv::Point(graph->Width - 1, graph->Height + 20), cv::Scalar(80, 80, 80, 0));
			
		int c = 0;
		int l = 20;
		for (std::list<Curve>::iterator it = graph->Curves.begin(); it != graph->Curves.end(); it++)
		{
			c++;
			Curve* thecurve = &(*it);
			cv::Scalar color = cv::Scalar(Color.at<cv::Vec4b>(c, 0)[0] / 1.2, Color.at<cv::Vec4b>(c, 0)[1] / 1.2, Color.at<cv::Vec4b>(c, 0)[2] / 1.2, 0);
			std::string name;
			char text[100];
			wcstombs_s(NULL, text, thecurve->Name.c_str(), 100);
			name = text;
			if (thecurve->Mode != L"normal")
			{
				wcstombs_s(NULL, text, thecurve->Mode.c_str(), 100);
				name += "(" + std::string(text) + ")";
			}
			cv::putText(graph->Back, name, cv::Point(l, 15), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, color);
			l += cv::getTextSize(name, cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, 1, NULL).width + 10;
			//l += name.length() * 14 + 15;
		}
	}
	else if (viewmode == 2)
	{
		int curvecount = (int)graph->Curves.size();
		cv::line(graph->Back, cv::Point(200, 20), cv::Point(200, curvecount * 25 + 20), cv::Scalar(60, 60, 60, 0));
		cv::line(graph->Back, cv::Point(300, 20), cv::Point(300, curvecount * 25 + 20), cv::Scalar(60, 60, 60, 0));
		cv::line(graph->Back, cv::Point(400, 20), cv::Point(400, curvecount * 25 + 20), cv::Scalar(60, 60, 60, 0));
		cv::putText(graph->Back, "value", cv::Point(205, 14), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(120, 120, 120, 0));
		cv::putText(graph->Back, "min", cv::Point(305, 14), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(120, 120, 120, 0));
		cv::putText(graph->Back, "max", cv::Point(405, 14), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(120, 120, 120, 0));
		for (int i = 20; i <= curvecount * 25 + 20; i += 25)
			cv::line(graph->Back, cv::Point(0, i), cv::Point(graph->Back.cols - 1, i), cv::Scalar(80, 80, 80, 0));

		int c = 0;
		for (std::list<Curve>::iterator it = graph->Curves.begin(); it != graph->Curves.end(); it++)
		{
			c++;
			Curve* thecurve = &(*it);
			cv::Scalar color = cv::Scalar(Color.at<cv::Vec4b>(c, 0)[0], Color.at<cv::Vec4b>(c, 0)[1], Color.at<cv::Vec4b>(c, 0)[2], 0);
			char text[100];
			wcstombs_s(NULL, text, thecurve->Name.c_str(), 100);
			cv::putText(graph->Back, text, cv::Point(10, c * 25 - 7 + 20), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, color);
		}
	}
}

DWORD WINAPI ThreadUpdate(LPVOID lpParameter)
{
	while (!exitthread)
	{
		for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
		{
			Graph* thegraph = &(*it);
			// 对于nontime模式的图，由FlowValue处理向Values[]中写数
			if (thegraph->Mode == L"nontime")
				continue;

			int nowtime = GetTickCount();
			while (nowtime - thegraph->LastUpdateTime > 1000.0 / thegraph->Speed)
			{
				thegraph->LastUpdateTime += (int)(1000.0 / thegraph->Speed);

				for (std::list<Curve>::iterator it = thegraph->Curves.begin(); it != thegraph->Curves.end(); it++)
				{
					Curve* thecurve = &(*it);
					/*int endp = thegraph->P - thegraph->Width;
					int endpb = endp - 1;
					if (endp < 0) 
						endp += 1920;
					if (endpb < 0)
						endpb += 1920;
					if (thecurve->Values[endpb] != thecurve->Values[endp] && (thecurve->Values[endpb] == thecurve->MaxVisibleValue || thecurve->Values[endpb] == thecurve->MinVisibleValue))
						CalCurveMaxMin(thecurve);*/

					if (thecurve->Mode == L"bump")
					{
						double increase = thecurve->IncomeValue - thecurve->CounterValue;
						if (abs(increase) > 0)
						{
							thecurve->Values[thegraph->P] += abs(increase);
							thecurve->CounterValue += increase;
						}
						else
						{
							int lastp = thegraph->P - 1;
							if (lastp < 0)
								lastp = 1919;
							thecurve->Values[thegraph->P] = thecurve->Values[lastp] * 0.8;
						}
					}
					else
						thecurve->Values[thegraph->P] = thecurve->IncomeValue;

				/*	if (thecurve->IncomeValue > thecurve->MaxVisibleValue || thecurve->IncomeValue < thecurve->MinVisibleValue)
						CalCurveMaxMin(thecurve);*/
				}

				thegraph->P++;
				if (thegraph->P >= 1920)
					thegraph->P = 0;
			}
		}
		Sleep(1);
	}
	return 0;
}

void CalCurveMaxMin(Curve* curve)
{
	int pa = *curve->Graph->PShow;
	double min = 9e50;
	double max = -9e50;
	for (int i = 0; i < curve->Graph->Width; i++)
	{
		pa--;
		if (pa < 0)
			pa += 1920;
		if (curve->ValuesShow[pa] > max)
			max = curve->ValuesShow[pa];
		if (curve->ValuesShow[pa] < min)
			min = curve->ValuesShow[pa];
	}
	curve->MaxVisibleValue = max;
	curve->MinVisibleValue = min;
}

void CalGraphMaxMin(Graph* graph)
{
	double min = 9e50;
	double max = -9e50;
	for (std::list<Curve>::iterator it = graph->Curves.begin(); it != graph->Curves.end(); it++)
	{
		Curve* curve = &(*it);
		CalCurveMaxMin(curve);
		if (curve->MaxVisibleValue > max)
			max = curve->MaxVisibleValue;
		if (curve->MinVisibleValue < min)
			min = curve->MinVisibleValue;
	}
	graph->MaxVisibleValue = max;
	graph->MinVisibleValue = min;
}

void nontimeGraphSample(Graph* thegraph)
{
	for (std::list<Curve>::iterator it = thegraph->Curves.begin(); it != thegraph->Curves.end(); it++)
	{
		Curve* thecurve = &(*it);
		if (thecurve->Mode == L"bump")
		{
			double increase = thecurve->IncomeValue - thecurve->CounterValue;
			if (abs(increase) > 0)
			{
				thecurve->Values[thegraph->P] += abs(increase);
				thecurve->CounterValue += increase;
			}
			else
			{
				int lastp = thegraph->P - 1;
				if (lastp < 0)
					lastp = 1919;
				thecurve->Values[thegraph->P] = thecurve->Values[lastp] * 0.8;
			}
		}
		else
			thecurve->Values[thegraph->P] = thecurve->IncomeValue;

		//CalCurveMaxMin(thecurve);
	}

	thegraph->P++;
	if (thegraph->P >= 1920)
		thegraph->P = 0;
}

extern "C" __declspec(dllexport) void FlowSetWidthHeight(const wchar_t* graphname, int width, int height)
{
	if (width > 2 && width <= 1920 && height > 2 && height <= 1080)
	{
		for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
		{
			if ((*it).Name == graphname)
			{
				(*it).Width = width;
				(*it).Height = height;
				if (it->ViewMode == 1)
				{
					HWND wind = FindWindow(NULL, graphname);
					if (wind != NULL)
					{
						char chars[100];
						wcstombs_s(NULL, chars, graphname, 100);
						cv::resizeWindow(chars, width, height + 1);
					}
				}
			}
		}
	}
}

extern "C" __declspec(dllexport) void FlowSetBoundry(const wchar_t* graphname, double bottomvalue, double topvalue)
{
	if (bottomvalue < topvalue)
	{
		for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
		{
			if ((*it).Name == graphname)
			{
				(*it).UserSetBottomBoundary = bottomvalue;
				(*it).UserSetTopBoundary = topvalue;
				(*it).AutoUpBottom = 0;
			}
		}
	}
}

extern "C" __declspec(dllexport) void FlowSetSpeed(const wchar_t* graphname, int speed)
{
	if (speed >= 1 && speed <= 4000)
	{
		for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
		{
			if ((*it).Name == graphname)
			{
				(*it).Speed = speed;
			}
		}
	}
}

extern "C" __declspec(dllexport) void FlowSetViewMode(const wchar_t* graphname, int viewmode)
{
	if (viewmode >= 1 && viewmode <= 2)
	{
		for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
		{
			if ((*it).Name == graphname)
			{
				(*it).ViewMode = viewmode;
			}
		}
	}
}

extern "C" __declspec(dllexport) void FlowCreateGraph(const wchar_t* graphname, const wchar_t* mode, int param)
{
	Graph* thegraph = NULL;
	for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
	{
		if ((*it).Name == graphname)
		{
			thegraph = &(*it);
			thegraph->Mode = mode;
			if (mode == L"nontime")
				thegraph->SampleInterval = param;
		}
	}
	if (thegraph == NULL)
	{
		Graph* agraph = new Graph();
		agraph->Name = graphname;
		agraph->Mode = mode;
		if (mode == L"nontime")
			agraph->SampleInterval = param;
		agraph->LastUpdateTime = GetTickCount();
		Graphs.push_back(*agraph);
		if (!Paused)
			Graphs.back().PShow = &Graphs.back().P;
		else
			Graphs.back().PShow = &Graphs.back().PPause;
		thegraph = &Graphs.back();
	}
}

extern "C" __declspec(dllexport) void FlowDuplicateGraph(const wchar_t* srcname, const wchar_t* dstname)
{
	Graph* thegraph = NULL;
	for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
	{
		if ((*it).Name == dstname)
		{
			it->Hide = false;
			return;
		}
	}
	for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
	{
		if ((*it).Name == srcname)
		{
			thegraph = &(*it);
			Graph* agraph = new Graph();
			*agraph = *thegraph;
			agraph->Name = dstname;
			agraph->HaveBeenOpened = false;
			Graphs.push_back(*agraph);
			break;
		}
	}
}

extern "C" __declspec(dllexport) void FlowHideGraph(const wchar_t* graphname)
{
	Graph* thegraph = NULL;
	for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
	{
		if ((*it).Name == graphname)
		{
			thegraph = &(*it);
			thegraph->Hide = true;
			thegraph->HaveBeenOpened = false;
			break;
		}
	}
}

extern "C" __declspec(dllexport) void FlowMoveGraph(const wchar_t* graphname, int x, int y)
{
	Graph* thegraph = NULL;
	for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
	{
		if ((*it).Name == graphname)
		{
			thegraph = &(*it);
			char chars[100];
			wcstombs_s(NULL, chars, thegraph->Name.c_str(), 100);
			cv::moveWindow(chars, x, y);
			break;
		}
	}
}

extern "C" __declspec(dllexport) void FlowSaveGraph(const wchar_t* graphname)
{
	Graph* thegraph = NULL;
	for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
	{
		if ((*it).Name == graphname || graphname == L"TheFirstGraph")
		{
			thegraph = &(*it);
			thegraph->SaveCount++;
			char namechar[10];
			wcstombs_s(NULL, namechar, thegraph->Name.c_str(), 100);
			char savechar[10];
			_itoa_s(thegraph->SaveCount, savechar, 10);
			char chars[100] = "";
			strcat_s(chars, namechar);
			strcat_s(chars, savechar);
			strcat_s(chars, ".bmp");
			if (it->ViewMode == 1)
				DrawViewMode1(thegraph);
			else if (it->ViewMode == 2)
				DrawViewMode2(thegraph);
			cv::imwrite(chars, thegraph->Dot);
			break;
		}
	}
}

extern "C" __declspec(dllexport) void FlowCreateCurve(const wchar_t* graphname, const wchar_t* curvename, const wchar_t* mode)
{
	Graph* thegraph = NULL;
	for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
	{
		if ((*it).Name == graphname)
		{
			thegraph = &(*it);
		}
	}
	if (thegraph == NULL)
	{
		Graph* agraph = new Graph();
		agraph->Name = graphname;
		agraph->Mode = L"nontime";
		agraph->LastUpdateTime = GetTickCount();
		Graphs.push_back(*agraph);
		thegraph = &Graphs.back();
	}
	
	Curve* thecurve = NULL;
	for (std::list<Curve>::iterator it = thegraph->Curves.begin(); it != thegraph->Curves.end(); it++)
	{
		if ((*it).Name == curvename)
		{
			thecurve = &(*it);
			thecurve->Graph = thegraph;
			thecurve->Mode = mode;
		}
	}
	if (thecurve == NULL && thegraph->Curves.size() < 10)
	{
		Curve* acurve = new Curve();
		acurve->Name = curvename;
		acurve->Graph = thegraph;
		acurve->Mode = mode;
		thegraph->Curves.push_back(*acurve);
		if (!Paused)
		{
			thegraph->Curves.back().ValuesShow = thegraph->Curves.back().Values;
			thegraph->Curves.back().IncomeValueShow = &thegraph->Curves.back().IncomeValue;
		}
		else
		{
			thegraph->Curves.back().ValuesShow = thegraph->Curves.back().ValuesPause;
			thegraph->Curves.back().IncomeValueShow = &thegraph->Curves.back().IncomeValuePause;
		}
		thegraph->NeedPlotMesh = true;
		thecurve = acurve;
	}

}

//extern __declspec(dllexport) void FlowCreateCurve(std::wstring curvename)
//{
//	FlowCreateCurve(L"default curve", curvename);
//}

//extern __declspec(dllexport) void FlowValue(std::wstring graphname, std::wstring curvename, double value)
//{
//	for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
//	{
//		if ((*it).Name == graphname)
//		{
//			Graph* thegraph = &(*it);
//			for (std::list<Curve>::iterator it = thegraph->Curves.begin(); it != thegraph->Curves.end(); it++)
//			{
//				if ((*it).Name == curvename)
//				{
//					Curve* thecurve = &(*it);
//					thecurve->IncomeValue = value;
//					if (thegraph->Mode == L"nontime")
//					{
//						if (thegraph->SampleCount == 0)
//							nontimeGraphSample(thegraph);
//						thegraph->SampleCount++;
//						if (thegraph->SampleCount == thegraph->SampleInterval)
//							thegraph->SampleCount = 0;
//					}
//				}
//			}
//		}
//	}
//}

extern "C" __declspec(dllexport) void FlowValue(const wchar_t* curvename, double value, const wchar_t* newgraphname)
{
	if (!Running)
		FlowStart();
	//if (RunPaused)
	//	return;
	
	Graph* thegraph;
	Curve* thecurve = NULL;
	for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
	{
		thegraph = &(*it);
		for (std::list<Curve>::iterator it = thegraph->Curves.begin(); it != thegraph->Curves.end(); it++)
		{
			if ((*it).Name == curvename)
			{
				thecurve = &(*it);
				thecurve->IncomeValue = value;
				if (thegraph->Mode == L"nontime")
				{
					if (thegraph->SampleCount == 0)
					{
						for (int l = 0; l < thegraph->SampleLength; l++)
							nontimeGraphSample(thegraph);
					}
					thegraph->SampleCount++;
					if (thegraph->SampleCount == thegraph->SampleInterval)
						thegraph->SampleCount = 0;
				}
			}
		}
	}

	thegraph = NULL;
	for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
	{
		if (it->Name == newgraphname)
		{
			thegraph = &(*it);
			break;
		}
	}

	if (thegraph == NULL && thecurve != NULL)
	{
		if (newgraphname != L"Debug")
		{	
			FlowCreateGraph(newgraphname, L"nontime", 1);
			FlowCreateCurve(newgraphname, curvename);
			FlowValue(curvename, value);
		}
	}
	else if (thegraph != NULL && thecurve == NULL && thegraph->Curves.size() < 10)
	{
		FlowCreateCurve(newgraphname, curvename);
		FlowValue(curvename, value);
	}
	else if (thegraph == NULL && thecurve == NULL)
	{	
		FlowCreateGraph(newgraphname, L"nontime", 1);
		FlowCreateCurve(newgraphname, curvename);
		FlowValue(curvename, value);
	}
}

extern "C" __declspec(dllexport) void FlowValueIncrease(const wchar_t* curvename, double value, const wchar_t* newgraphname)
{
	Curve* thecurve = NULL;
	for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
	{
		Graph* thegraph = &(*it);
		for (std::list<Curve>::iterator it = thegraph->Curves.begin(); it != thegraph->Curves.end(); it++)
		{
			if ((*it).Name == curvename)
			{
				thecurve = &(*it);
				/*thecurve->IncomeValue += value;
				if (thegraph->Mode == L"nontime")
				{
					if (thegraph->SampleCount == 0)
						nontimeGraphSample(thegraph);
					thegraph->SampleCount++;
					if (thegraph->SampleCount == thegraph->SampleInterval)
						thegraph->SampleCount = 0;
				}*/
			}
		}
	}
	if (thecurve == NULL)
	{
		FlowCreateGraph(newgraphname, L"normal");
		FlowCreateCurve(newgraphname, curvename, L"bump");
		FlowValue(curvename, value, newgraphname);
	}
	else
	{
		FlowValue(curvename, thecurve->IncomeValue + value, newgraphname);
	}
}

extern "C" __declspec(dllexport) void FlowStart()
{
	if (Running)
	{
		return;
		//FlowStop();
	}
	Running = true;
	timeBeginPeriod(1);
	exitthread = false;
	Paused = false;
	NextPosition = 100;
	DefineColor();
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, ThreadUpdate, NULL, 0, NULL);
	CloseHandle(hThread);
	hThread = CreateThread(NULL, 0, ThreadShow, NULL, 0, NULL);
	CloseHandle(hThread);
}

extern "C" __declspec(dllexport) void FlowPause(bool pause)
{
	for (std::list<Graph>::iterator it = Graphs.begin(); it != Graphs.end(); it++)
	{
		Graph* thegraph = &(*it);
		for (std::list<Curve>::iterator it = thegraph->Curves.begin(); it != thegraph->Curves.end(); it++)
		{
			Curve* thecurve = &(*it);
			if (pause == true)
			{
				memcpy(thecurve->ValuesPause, thecurve->Values, 1920 * sizeof(double));
				thecurve->ValuesShow = thecurve->ValuesPause;
				thecurve->IncomeValuePause = thecurve->IncomeValue;
				thecurve->IncomeValueShow = &thecurve->IncomeValuePause;
				thegraph->PPause = thegraph->P;
				thegraph->PShow = &thegraph->PPause;
			}
			else
			{
				thecurve->ValuesShow = thecurve->Values;
				thecurve->IncomeValueShow = &thecurve->IncomeValue;
				thegraph->PShow = &thegraph->P;
			}
		}
	}
	Paused = pause;
}

extern "C" __declspec(dllexport) void FlowStop()
{
	exitthread = true;
	Sleep(300);
	Graphs.clear();
	Running = false;
}

extern "C" __declspec(dllexport) void FlowTest()
{
	FlowSetBoundry(L"Debug", 0, 1000);
	FlowStart();
	for (int i = 0; i < 32; i++)
	for (int j = 0; j < 32; j++)
	{
		int x = abs(i - j);
		FlowValue(L"i", i);
		FlowValue(L"j", j);
		FlowValue(L"x", x);
	}

	for (int i = 0; i < 3000; i++)
	{
		FlowValueIncrease(L"t", rand() % 10, L"T");
		Sleep(100);
		if (i > 30)
			FlowValueIncrease(L"d", rand() % 10, L"T");
		Sleep(100);
	}
	return;
	FlowStop();
}

int _tmain(int argc, _TCHAR* argv[])
{
	FlowTest();
	return 0;
}
