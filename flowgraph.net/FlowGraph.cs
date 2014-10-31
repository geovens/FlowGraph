using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace FlowGraphNet
{
	public class FlowGraph
	{
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowSetWidthHeight(string graphname, int width, int height);
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowSetBoundry(string graphname, double minvalue, double maxvalue);
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowSetSpeed(string graphname, int speed);
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowSetViewMode(string graphname, int viewmode);
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowCreateGraph(string graphname, string mode = "normal", int param = 1);
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowDuplicateGraph(string srcname, string dstname);
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowHideGraph(string graphname);
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowMoveGraph(string graphname, int x, int y);
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowSaveGraph(string graphname = "TheFirstGraph");
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowCreateCurve(string graphname, string curvename, string mode = "normal");
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowCreateCurve(string curvename);
		[DllImport("flowgraph.dll", CharSet=CharSet.Unicode)]
		private static extern void FlowValue(string curvename, double value, string newgraphname = "Debug");
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowValueIncrease(string curvename, double value, string newgraphname = "Debug");
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowStart();
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowPause(bool pause);
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowStop();
		[DllImport("flowgraph.dll", CharSet = CharSet.Unicode)]
		private static extern void FlowTest();


		public static void SetWidthHeight(string graphname, int width, int height)
		{
			FlowSetWidthHeight(graphname, width, height);
		}
		public static void SetBoundry(string graphname, double minvalue, double maxvalue)
		{
			FlowSetBoundry(graphname, minvalue, maxvalue);
		}
		public static void SetSpeed(string graphname, int speed)
		{
			FlowSetSpeed(graphname, speed);
		}
		public static void SetViewMode(string graphname, int viewmode)
		{
			FlowSetViewMode(graphname, viewmode);
		}
		public static void CreateGraph(string graphname, string mode = "normal", int param = 1)
		{
			FlowCreateGraph(graphname, mode, param);
		}
		public static void DuplicateGraph(string srcname, string dstname)
		{
			FlowDuplicateGraph(srcname, dstname);
		}
		public static void HideGraph(string graphname)
		{
			FlowHideGraph(graphname);
		}
		public static void MoveGraph(string graphname, int x, int y)
		{
			FlowMoveGraph(graphname, x, y);
		}
		public static void SaveGraph(string graphname = "TheFirstGraph")
		{
			FlowSaveGraph(graphname);
		}
		public static void CreateCurve(string graphname, string curvename, string mode = "normal")
		{
			FlowCreateCurve(graphname, curvename, mode);
		}
		public static void CreateCurve(string curvename)
		{
			FlowCreateCurve(curvename);
		}
		public static void Value(string curvename, double value, string newgraphname = "Debug")
		{

			FlowValue(curvename, value, newgraphname);
		}
		public static void ValueIncrease(string curvename, double value, string newgraphname = "Debug")
		{
			FlowValueIncrease(curvename, value, newgraphname);
		}
		public static void Start()
		{
			FlowStart();
		}
		public static void Pause(bool pause)
		{
			FlowPause(pause);
		}
		public static void Stop()
		{
			FlowStop();
		}
		public static void Test()
		{
			FlowTest();
		}

	}
}
