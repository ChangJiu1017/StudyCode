using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.PropertyGridInternal;
using System.Runtime.InteropServices;
using System.IO;
using System.IO.Ports;
using System.Reflection;
using System.Threading;

namespace _200直喷泵检测
{
    public partial class FormRecord : Form
    {
        [DllImport("user32.dll")]
        public static extern bool ReleaseCapture();
        [DllImport("user32.dll")]
        public static extern bool SendMessage(IntPtr hwnd, int wMsg, int wParam, int lParam);
        public const int WM_SYSCOMMAND = 0x0112;
        public const int SC_MOVE = 0xF010;
        public const int HTCAPTION = 0x0002;
        Form_Main parentForm;
        DataTable dt = new DataTable();
        DataTable dtsv = new DataTable();

        private void panel_Main_Paint(object sender, PaintEventArgs e)
        {
            //ReleaseCapture();
           // SendMessage(this.Handle, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
            //panel_title.Focus();
        }
        public FormRecord(Form_Main _parentForm)
        {
            InitializeComponent();
            parentForm = _parentForm;
        }
        /// <summary>
        /// 点击进入记录文件界面
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void FormRecord_Load(object sender, EventArgs e)
        {
            this.WindowState = FormWindowState.Normal;
            panel_Main.Hide();
            //SearchConditionInit();
            this.Cursor = Cursors.WaitCursor;

            StartSearchRecord();
            this.Cursor = Cursors.Default;
            panel_Main.Show();
            //加载的时候将年份添加到下拉框中
            //获得当前的年份
            int year = DateTime.Now.Year;

            for (int i = year; i >= 2010; i--) 
            {
                cboYears.Items.Add(i + "年");
            }
        }
        /// <summary>
        /// 当年份发生改变的时候加载月份
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cboYears_SelectedIndexChanged(object sender, EventArgs e)
        {
            //添加之前清空之前的内容
            cboMonth.Items.Clear();
            for(int i=1;i<=12;i++)
            {
                cboMonth.Items.Add(i + "月");
            }


        }
        /// <summary>
        /// 当月份发生改变的时候 加载天
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cboMonth_SelectedIndexChanged(object sender, EventArgs e)
        {
            cboDays.Items.Clear();
            int day = 0;//定义一个变量 来储存天数
            //获得月份
            string strMonth = cboMonth.SelectedItem.ToString().Split(new char[] { '月' }, StringSplitOptions.RemoveEmptyEntries)[0];
            string strYears = cboYears.SelectedItem.ToString().Split(new char[] { '年' }, StringSplitOptions.RemoveEmptyEntries)[0];
            int year = Convert.ToInt32(strYears);
            int month = Convert.ToInt32(strMonth);
            switch(month)
            {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12: day = 31;
                    break;
               case 2:
                  if ((year%400==0)||(year % 4==0 && year% 100 !=0 ))
                    {
                        day = 29;
                    }
                    else
                    {
                        day = 28;
                    }
                    break;
                default:day = 30;
                    break;                                                              
            }
            //MessageBox.Show(cboMonth.SelectedIndex.ToString());
            for (int i = 1; i <= day; i++)
            {
                cboDays.Items.Add(i + "日");
            }
        }
        private void DispdataGridViewData(DataView dispDv)
        {
            try
            {
                dataGridView_Record.DataSource = dispDv;

                for (int i = 0; i < dataGridView_Record.Columns.Count; i++)
                {
                    dataGridView_Record.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;

                }
                dataGridView_Record.Columns[0].Width = 30000;
                //dataGridView_Record.Columns[4].Width = 100;
                dataGridView_Record.Columns[6].Width = 30000;
                //dataGridView_Record.Columns[8].Width = 150;
                //dataGridView_Record.Columns[9].Width = 150;
                //dataGridView_Record.Columns[10].Width = 150;

                for (int i = 0; i < dataGridView_Record.Rows.Count; i++)
                {
                    if ((string)(dataGridView_Record.Rows[i].Cells[3].Value) == "故障")
                    {
                        dataGridView_Record.Rows[i].DefaultCellStyle.ForeColor = Color.Red;
                    }
                }
            }
            catch
            {

            }
        }

        private void button_Found_Click(object sender, EventArgs e)
        {
            this.Cursor = Cursors.WaitCursor;
            StartSearchRecord();
            this.Cursor = Cursors.Default;
        }
        private void StartSearchRecord()
        {
            string pth = "2022年1月4日无人机200型直喷泵测试记录数据.CSV";
            
            DataTable mydt = OpenCSV(pth);
            DataTable dtsv = new DataTable();
            dataGridView_Record.DataSource = mydt;
            dataGridView_Record.Columns[1].Width = 115;
            dataGridView_Record.Columns[0].Width = 115;
            dataGridView_Record.Columns[2].Width = 115;
            dataGridView_Record.Columns[3].Width = 115;
            dataGridView_Record.Columns[4].Width = 115;
            dataGridView_Record.Columns[5].Width = 110;
            dataGridView_Record.Columns[6].Width = 110;
            dataGridView_Record.Columns[7].Width = 110;
            dataGridView_Record.Columns[8].Width = 110;
            dataGridView_Record.Columns[9].Width = 110;
        }

       
        /// <summary>
        /// 导出文件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_daochu_Click(object sender, EventArgs e)
        {
            saveFileDialog_RecordCSV.Filter = "csv文件(*.csv)|*.csv|txt文件(*.txt)|*.txt";
            saveFileDialog_RecordCSV.FilterIndex = 1;
            saveFileDialog_RecordCSV.RestoreDirectory = true;
            if (saveFileDialog_RecordCSV.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                SaveCSV(dtsv, saveFileDialog_RecordCSV.FileName);
            }
        }
        private bool SaveCSV(DataTable savedt, string filePath)
        {
            try
            {
                FileInfo fi = new FileInfo(filePath);
                if (!fi.Directory.Exists)
                {
                    fi.Directory.Create();
                }
                FileStream fs = new FileStream(filePath, System.IO.FileMode.Create, System.IO.FileAccess.Write);
                StreamWriter sw = new StreamWriter(fs, System.Text.Encoding.GetEncoding("GB2312"));
                string data = "";
                for (int i = 0; i < savedt.Columns.Count; i++)
                {
                    data += savedt.Columns[i].ColumnName.ToString();
                    if (i < savedt.Columns.Count - 1)
                    {
                        data += ",";
                    }
                }
                sw.WriteLine(data);
                for (int i = 0; i < savedt.Rows.Count; i++)
                {
                    data = "";
                    for (int j = 0; j < savedt.Columns.Count; j++)
                    {
                        string str = savedt.Rows[i][j].ToString();
                        //str = string.Format();
                        data += str;
                        if (j < savedt.Columns.Count - 1)
                        {
                            data += ",";
                        }
                    }
                    sw.WriteLine(data);
                }
                sw.Close();
                fs.Close();
                return true;
            }
            catch
            {
                return false;
            }
        }
        public DataTable OpenCSV(string filePath)
        {
            DataTable tempdt = new DataTable();
            FileStream fs = new FileStream(filePath, System.IO.FileMode.Open, System.IO.FileAccess.Read);
            StreamReader sr = new StreamReader(fs, System.Text.Encoding.GetEncoding("UTF-8"));

            string strLine = " ";
            string[] aryLine = null;
            string[] tableHead = null;

            int columnCount = 0;
            bool IsFirst = true;
            while ((strLine = sr.ReadLine()) != null)
            {
                if (IsFirst == true)
                {
                    tableHead = strLine.Split(',');
                    IsFirst = false;
                    columnCount = tableHead.Length;
                    //创建列
                    for (int i = 0; i < columnCount; i++)
                    {
                        tableHead[i] = tableHead[i];
                        DataColumn dc = new DataColumn(tableHead[i]);
                        tempdt.Columns.Add(dc);
                    }
                }
                else
                {
                    aryLine = strLine.Split(',');
                    DataRow dr = tempdt.NewRow();
                    for (int j = 0; j < columnCount; j++)
                    {
                        dr[j] = aryLine[j];
                    }
                    tempdt.Rows.Add(dr);
                }
            }
            if (aryLine != null && aryLine.Length > 0)
            {
                tempdt.DefaultView.Sort = tableHead[3] + " " + "DESC";
                tempdt.DefaultView.Sort = "[ColumnName] ASC, [ColumnName] DESC";
            }
            sr.Close();
            fs.Close();
            return tempdt;
        }

        private void button_empty_Click(object sender, EventArgs e)
        {

        }

        private void dataGridView_Record_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

       
    }
}
