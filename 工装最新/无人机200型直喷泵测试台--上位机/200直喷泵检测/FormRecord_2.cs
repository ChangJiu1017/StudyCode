using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
namespace _200直喷泵检测
{
    public partial class FormRecord_2 : Form
    {

        DataTable dt = new DataTable();
        public FormRecord_2(Form_Main _parentForm)
        {
            InitializeComponent();
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
                tempdt.DefaultView.Sort = tableHead[1] + " " + "DESC";
            }
            sr.Close();
            fs.Close();
            return tempdt;
        }
        
        private void StartSearchRecord()
        {
            bool readCorrect = true;
            //string mypath = "..\\Debug\\无人机200直喷泵记录数据\\Save\\2022年1月4日无人机200型直喷泵测试记录数据.CSV";
            string mypath= @"..\\Debug\\无人机200直喷泵记录数据\\Save" + "\\" +
                                 DateTime.Now.Year.ToString() + "年" + DateTime.Now.Month.ToString() + "月" + "无人机200型直喷泵测试记录数据" + ".CSV";
            try
            {
                dt = OpenCSV(mypath);
            }
            catch
            {
                dt.Clear();
                readCorrect = false;
            }
            System.Data.DataTable tempdt = dt;
            DataView dv = dt.DefaultView;

            if (readCorrect)
            {
                //编号有输入
                if (!textBox1.Text.Equals(""))
                {
                    dv.RowFilter = string.Format("产品编号 = '{0}'", textBox1.Text);
                    tempdt = dv.ToTable();
                    dv = tempdt.DefaultView;
                }
            }
            DispdataGridViewData(dv);
        }

        private void DispdataGridViewData(DataView dispDv)
        {
            try
            {
                dataGridView1.DataSource = dispDv;

                for (int i = 0; i < dataGridView1.Columns.Count; i++)
                {
                    dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;

                }


                for (int i = 0; i < dataGridView1.Rows.Count; i++)
                {
                    if ((string)(dataGridView1.Rows[i].Cells[3].Value) == "故障")
                    {
                        dataGridView1.Rows[i].DefaultCellStyle.ForeColor = Color.Red;
                    }
                }
            }
            catch
            {

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

        private void button2_Click_1(object sender, EventArgs e)
        {
            //string pth = "..\\Debug\\无人机200直喷泵记录数据\\Save\\2022年1月4日无人机200型直喷泵测试记录数据.CSV";
            string pth = @"..\\Debug\\无人机200直喷泵记录数据\\Save" + "\\" +
                                 DateTime.Now.Year.ToString() + "年" + DateTime.Now.Month.ToString() + "月"  + "无人机200型直喷泵测试记录数据" + ".CSV";

            DataTable mydt = OpenCSV(pth);

            saveFileDialog_RecordCSV.Filter = "csv文件(*.csv)|*.csv|txt文件(*.txt)|*.txt";
            saveFileDialog_RecordCSV.FilterIndex = 1;
            saveFileDialog_RecordCSV.RestoreDirectory = true;
            if (saveFileDialog_RecordCSV.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                SaveCSV(mydt, saveFileDialog_RecordCSV.FileName);
            }
        }

        private void FormRecord_2_Load(object sender, EventArgs e)
        {
           // string pth = "..\\Debug\\无人机200直喷泵记录数据\\Save\\2022年1月4日无人机200型直喷泵测试记录数据.CSV";
            string pth = @"..\\Debug\\无人机200直喷泵记录数据\\Save" + "\\" +
                                  DateTime.Now.Year.ToString() + "年" + DateTime.Now.Month.ToString() + "月"  + "无人机200型直喷泵测试记录数据" + ".CSV";
            DataTable mydt = OpenCSV(pth);
            DataTable dtsv = new DataTable();
            dataGridView1.DataSource = mydt;
            dataGridView1.Columns[1].Width = 115;
            dataGridView1.Columns[0].Width = 115;
            dataGridView1.Columns[2].Width = 115;
            dataGridView1.Columns[3].Width = 115;
            dataGridView1.Columns[4].Width = 115;
            dataGridView1.Columns[5].Width = 110;
            dataGridView1.Columns[6].Width = 110;
            dataGridView1.Columns[7].Width = 110;
            dataGridView1.Columns[8].Width = 110;
            dataGridView1.Columns[9].Width = 110;
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            this.Cursor = Cursors.WaitCursor;
            StartSearchRecord();
            dataGridView1.ClearSelection();
            this.Cursor = Cursors.Default;
        }

        private void button3_Click_1(object sender, EventArgs e)
        {
            textBox1.Text = "";
            this.Cursor = Cursors.WaitCursor;
            StartSearchRecord();
            dataGridView1.ClearSelection();
            this.Cursor = Cursors.Default;
            //string pth = "..\\Debug\\无人机200直喷泵记录数据\\Save\\2022年1月4日无人机200型直喷泵测试记录数据.CSV";
            string pth = @"..\\Debug\\无人机200直喷泵记录数据\\Save" + "\\" +
                                 DateTime.Now.Year.ToString() + "年" + DateTime.Now.Month.ToString() + "月" + "无人机200型直喷泵测试记录数据" + ".CSV";

            DataTable mydt = OpenCSV(pth);
            dataGridView1.DataSource = mydt;
        }
    }
}
