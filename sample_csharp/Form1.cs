using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using FlowGraphNet;

namespace sample_csharp
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		private void button1_Click(object sender, EventArgs e)
		{
			for (int i = 0; i < 32; i++)
				for (int j = 0; j < 32; j++)
				{
					int x = Math.Abs(i - j);
					FlowGraph.Value("i", i);
					FlowGraph.Value("j", j);
					FlowGraph.Value("x", x);
				}

			timer1.Enabled = true;
		}

		private void timer1_Tick(object sender, EventArgs e)
		{
			FlowGraph.ValueIncrease("t", 1, "T");
		}
	}
}
