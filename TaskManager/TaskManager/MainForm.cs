using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

namespace TaskManager
{
    public partial class MainForm : Form
    {
        readonly int ramFactor = 1024;
        readonly string suffix = "kB";
        Dictionary<int, Process> d_processes;
        CommandLine cmd;
        public MainForm()
        {
            InitializeComponent();
            cmd = new CommandLine();

            SetColumns();
            statusStrip1.Items.Add("");
            LoadProcesses();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            AddNewProcesses();
            RemoveOldProcesses();
            UpdateExistingProcesses();
            statusStrip1.Items[0].Text = ($"количество  процессов: {listViewProcesses.Items.Count}");
        }
        void SetColumns()
        {
            listViewProcesses.Columns.Add("PID");
            listViewProcesses.Columns.Add("Name");
            listViewProcesses.Columns.Add("Working set");
            listViewProcesses.Columns.Add("Peak working set");
        }
        void LoadProcesses()
        {
            d_processes = Process.GetProcesses().ToDictionary(item => item.Id, item => item);
            foreach (KeyValuePair<int, Process> i in d_processes)
            {
                AddProcessToListView(i.Value);
            }
        }
        void AddNewProcesses()
        {
            Dictionary<int, Process> d_processes = Process.GetProcesses().ToDictionary(item => item.Id, item => item);
            foreach (KeyValuePair<int, Process> i in d_processes)
            {
                if (!this.d_processes.ContainsKey(i.Key))
                {

                    AddProcessToListView(i.Value);
                }
            }
        }
        void RemoveOldProcesses()
        {
            this.d_processes = Process.GetProcesses().ToDictionary(item => item.Id, item => item);
            for (int i = 0; i < listViewProcesses.Items.Count; i++)
            {
                if (!d_processes.ContainsKey(Convert.ToInt32(listViewProcesses.Items[i].Text)))
                    listViewProcesses.Items.RemoveAt(i);
            }
        }
        void UpdateExistingProcesses()
        {
            for (int i = 0; i < listViewProcesses.Items.Count; i++)
            {
                int id = Convert.ToInt32(listViewProcesses.Items[i].Text);
                listViewProcesses.Items[i].SubItems[2].Text = $"{d_processes[id].WorkingSet64 / ramFactor} {suffix}";
                listViewProcesses.Items[i].SubItems[3].Text = $"{d_processes[id].PeakWorkingSet64 / ramFactor} {suffix}";
            }
        }
        void AddProcessToListView(Process process)
        {

            ListViewItem item = new ListViewItem();
            item.Text = process.Id.ToString();
            item.SubItems.Add(process.ProcessName);
            item.SubItems.Add($"{process.WorkingSet64 / ramFactor} {suffix}");
            item.SubItems.Add($"{process.PeakWorkingSet64 / ramFactor} {suffix}");
            listViewProcesses.Items.Add(item);
        }
        void RemoveProcessFromListView(int pid)
        {
            listViewProcesses.Items.RemoveByKey(pid.ToString());
        }

        private void runToolStripMenuItem_Click(object sender, EventArgs e)
        {
            cmd.ShowDialog();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            StreamWriter sw = new StreamWriter("ProgramList.txt");

            for (int i = 0; i < cmd.ComboBoxFileName.Items.Count; i++)
            {
                sw.WriteLine(cmd.ComboBoxFileName.Items[i]);
            }

            sw.Close();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {

        }

        private void MainForm_Load_1(object sender, EventArgs e)
        {

        }

        private void MainForm_Load_2(object sender, EventArgs e)
        {

        }
    }
}
