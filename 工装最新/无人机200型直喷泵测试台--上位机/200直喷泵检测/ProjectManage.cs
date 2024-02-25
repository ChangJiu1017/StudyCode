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

    public partial class ProjectManageForm : Form
    {        
        /// <summary>
        /// ini文档读写类
        /// </summary>
        //public class IniFile
        //{
        //    private string fileName;

        //    [DllImport("kernel32")]
        //    private static extern int GetPrivateProfileInt(string lpAppName, string lpKeyName, int nDefault, string lpFileName);

        //    [DllImport("kernel32")]
        //    private static extern int GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault,
        //        StringBuilder lpReturnedString, int nSize, string lpFileName);

        //    [DllImport("kernel32")]
        //    private static extern bool WritePrivateProfileString(string lpAppName, string lpKeyName, string lpString, string lpFileName);

        //};

        public class FileFoldDeleter
        {
        }

        [DllImport("user32.dll")]
        public static extern bool ReleaseCapture();
        [DllImport("user32.dll")]
        public static extern bool SendMessage(IntPtr hwnd, int wMsg, int wParam, int lParam);
        public const int WM_SYSCOMMAND = 0x0112;
        public const int SC_MOVE = 0xF010;
        public const int HTCAPTION = 0x0002;
        Form_Main parentForm;

        public ProjectManageForm(Form_Main _parentForm)
        {
            InitializeComponent();
            parentForm = _parentForm;
        }

        private void dataGridView_Param_DispStyleSet()
        {
            dataGridView_Manage.CurrentCell = null;
            dataGridView_Manage.Columns[0].Width = 100;
            dataGridView_Manage.Columns[1].Width = 550;
            dataGridView_Manage.Columns[0].Resizable = DataGridViewTriState.False;
            dataGridView_Manage.Columns[1].Resizable = DataGridViewTriState.False;
            //dataGridView_Manage.Columns[0].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            //dataGridView_Manage.Columns[1].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            dataGridView_Manage.Columns[0].SortMode = DataGridViewColumnSortMode.NotSortable;
            dataGridView_Manage.Columns[1].SortMode = DataGridViewColumnSortMode.NotSortable;
            for (UInt16 i = 0; i < dataGridView_Manage.RowCount; i++)
            {
                if (i % 2 == 0)
                {
                    dataGridView_Manage.Rows[i].DefaultCellStyle.BackColor = Color.White;
                }
                else
                {
                    dataGridView_Manage.Rows[i].DefaultCellStyle.BackColor = Color.FromArgb(212, 238, 255);
                }
            }
            dataGridView_Manage.ClearSelection();
        }

        private void AddProjectToList()
        {
            string ProjectName;
            string text;
            string ProjConfig;
            DataTable dt = new DataTable();
            //DataRow dr = new DataRow();
            string fname = System.Windows.Forms.Application.StartupPath + @"\DCUProject.proj";

            if (File.Exists(fname))
            {
                System.IO.StreamReader file = new StreamReader(fname);
                dt.Columns.Add("项目");
                dt.Columns.Add("参数文件");
                dataGridView_Manage.DataSource = dt;
                while (true)
                {
                    text = file.ReadLine();
                    if (text != null)
                    {
                        if (text.Equals("")) continue;
                        if (text.Substring(0, 1).Equals(":"))//头格式正确
                        {
                            ProjectName = text.Replace(":", "");
                            text = file.ReadLine();
                            ProjConfig = System.Windows.Forms.Application.StartupPath + text.Replace("|", "");
                            if (!File.Exists(ProjConfig))
                            {
                                //MessageBox.Show("不正确的工程配置文件路径", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                            }
                            else
                            {
                                DataRow dr = dt.NewRow();
                                dr[0] = ProjectName;
                                dr[1] = ProjConfig;
                                dt.Rows.Add(dr);
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                file.Close();
                dataGridView_Param_DispStyleSet();
            }

        }
        //添加文件按钮
        private void button_add_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.OpenFileDialog openFileDialog = new OpenFileDialog();
            //openFileDialog.InitialDirectory = path;
            openFileDialog.Filter = "Excel(*.xls,*.xlsx)|*.xls|All|*.*";//"File|*.xls|File|*.xlsx|All|*.*";
            System.Windows.Forms.DialogResult result = openFileDialog.ShowDialog();

            if (result == System.Windows.Forms.DialogResult.OK)
            {
                textBox_File.Text = openFileDialog.FileName;
                if (label_tipsFile.Visible)
                {
                    label_tipsFile.Visible = false;
                }
            }
        }

        private void textBox_projectName_Leave(object sender, EventArgs e)
        {
            if (!textBox_projectName.Text.Equals(""))
            {
                if (label_tipsName.Visible)
                    label_tipsName.Visible = false;
            }
        }

        private void CreatProjectManageFile(string FilePath)
        {
            string fname = System.Windows.Forms.Application.StartupPath + @"\DCUProject.proj";
            string lineWriteStr;
            FileStream myFs;
            StreamWriter mySw;
            if (!File.Exists(fname))
            {
                lineWriteStr = ":" + textBox_projectName.Text + "\r\n" + "|" + FilePath;
                myFs = new FileStream(fname, FileMode.Create);
                mySw = new StreamWriter(myFs);
            }
            else
            {
                lineWriteStr = "\r\n:" + textBox_projectName.Text + "\r\n" + "|" + FilePath;
                myFs = new FileStream(fname, FileMode.Append, FileAccess.Write);
                mySw = new StreamWriter(myFs);
            }

            try
            {
                mySw.Write(lineWriteStr);
            }
            catch (SystemException ex)
            {
                MessageBox.Show(ex.Message, "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
            mySw.Close();
            myFs.Close();
        }

        //创建按钮
        private void button_OK_Click(object sender, EventArgs e)
        {
            bool creat = true;
            label_tipsName.Visible = false;
            label_tipsFile.Visible = false;
            label_projectExist.Visible = false;
            if (textBox_projectName.Text.Equals(""))
            {
                label_tipsName.Visible = true;
                creat = false;
            }
            if (textBox_File.Text.Equals(""))
            {
                label_tipsFile.Visible = true;
                creat = false;
            }

            if (creat)
            {
                button_OK.Enabled = false;
                string path = System.Windows.Forms.Application.StartupPath + @"\Project\" + textBox_projectName.Text + @"\";
                string relativePath = @"\Project\" + textBox_projectName.Text + @"\";
                if (!Directory.Exists(path))//如果可以建
                {
                    Directory.CreateDirectory(path);
                    System.IO.File.Copy(textBox_File.Text, (path + Path.GetFileName(textBox_File.Text)));
                    //CreatProjectManageFile(path + Path.GetFileName(textBox_File.Text));
                    CreatProjectManageFile(relativePath + Path.GetFileName(textBox_File.Text));
                    //   parentForm.PrjectConfigFile = path + Path.GetFileName(textBox_File.Text);
                    MYSettings.Default.ProjectName = textBox_projectName.Text;
                    MYSettings.Default.Save();
                    // if (parentForm.ProjectParam.ReadParamFile(parentForm.PrjectConfigFile))
                    {
                        this.DialogResult = System.Windows.Forms.DialogResult.OK;
                    }
                    // else
                    {
                        //      this.DialogResult = System.Windows.Forms.DialogResult.No;
                    }
                    this.Close();
                }
                else
                {
                    label_projectExist.Visible = true;
                }

            }
        }

        private void dataGridView_Manage_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right && e.RowIndex > -1 && e.ColumnIndex > -1)
            {
                dataGridView_Manage.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
                dataGridView_Manage.ClearSelection();
                dataGridView_Manage.Rows[e.RowIndex].Selected = true;
                dataGridView_Manage.CurrentCell = dataGridView_Manage.Rows[e.RowIndex].Cells[e.ColumnIndex];
                contextMenuStrip_Option.Show(MousePosition.X, MousePosition.Y);
                //dataGridView_Manage.SelectionMode = DataGridViewSelectionMode.CellSelect;
            }
            else
            {
                dataGridView_Manage.SelectionMode = DataGridViewSelectionMode.CellSelect;
            }
        }

        private void dataGridView_Manage_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void button_SetTotalUse_Click(object sender, EventArgs e)
        {
            MYSettings.Default.TotalUseMax = textBox_TotalUseMax.Text;
            MYSettings.Default.Save();
            if (MYSettings.Default.TotalUseMax == textBox_TotalUseMax.Text)
            {
                button_SetTotalUse.Enabled = false;
            }

        }

        private void button_ClearUse_Click(object sender, EventArgs e)
        {
            MYSettings.Default.TotalUse = "0";
            MYSettings.Default.Save();
            if (Convert.ToUInt32(MYSettings.Default.TotalUse) == 0)
            {
                textBox_TotalUse.Text = MYSettings.Default.TotalUse;
            }
        }

        private void textBox_TotalUse_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (((int)e.KeyChar < 48 || (int)e.KeyChar > 57)
                && (int)e.KeyChar != 8)
            {
                e.Handled = true;
            }
        }

        private void textBox_TotalUseMax_TextChanged(object sender, EventArgs e)
        {
            button_SetTotalUse.Enabled = true;
        }

        private void textBox_TotalUseMax_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (((int)e.KeyChar < 48 || (int)e.KeyChar > 57)
                && (int)e.KeyChar != 8)
            {
                e.Handled = true;
            }
        }

        private void tabControl_Manage_Click(object sender, EventArgs e)
        {
            if (tabControl_Manage.SelectedIndex == 1)
            {
                // if (Settings.Default.Admin_Checked)
                {
                    dataGridView_Manage.Show();
                    AddProjectToList();
                }
                //    else
                {
                    //        dataGridView_Manage.Hide();
                }
            }
        }

        private void ProjectManageForm_Load(object sender, EventArgs e)
        {
            //if (tabControl_Manage.SelectedIndex == 1)
            //{
            //    //  if (Settings.Default.Admin_Checked)
            //    {
            //        dataGridView_Manage.Visible = true;
            //        //AddProjectToList();
            //    }
            //    //  else
            //    {
            //        //     dataGridView_Manage.Visible = false;
            //    }
            //}
            //textBox_TotalUseMax.Text = MYSettings.Default.TotalUseMax;//parentForm.Config.Param.TotalUseMax;
            //textBox_TotalUse.Text = MYSettings.Default.TotalUse;//parentForm.Config.Param.TotalUse;
        }

        private void ProjectManageForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Close();
        }


        private bool DeleteProjectFile(string Project)
        {
            //bool ret = false;
            List<string> text;
            string ProjectName;
            string fname = System.Windows.Forms.Application.StartupPath + @"\DCUProject.proj";
            //System.IO.StreamReader file = new StreamReader(fname);
            File.SetAttributes(fname, FileAttributes.Normal);
            if (File.Exists(fname))
            {
                text = new List<string>(File.ReadAllLines(fname));
                for (UInt16 i = 0; i < text.Count; i++)
                {
                    if (text.ElementAt(i).Equals(""))
                    {
                        text.RemoveAt(i);
                    }
                    ProjectName = text.ElementAt(i);
                    ProjectName = ProjectName.Replace(":", "");
                    if (ProjectName.Equals(Project))
                    {
                        text.RemoveAt(i);
                        text.RemoveAt(i);
                    }
                    Thread.Sleep(1);
                }
                File.WriteAllLines(fname, text.ToArray());
                File.SetAttributes(fname, FileAttributes.Hidden);
            }

            string folderName = System.Windows.Forms.Application.StartupPath + @"\Project\" + Project;
            string[] files = Directory.GetFiles(folderName);

            foreach (var subfile in files)
            {
                File.Delete(subfile);
            }

            string[] folders = Directory.GetDirectories(System.Windows.Forms.Application.StartupPath + @"\Project\");

            foreach (var subfolder in folders)
            {
                if (subfolder.Equals(folderName))
                {
                    Directory.Delete(subfolder);
                }
            }

            return true;
        }
        private void 删除ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string Project = dataGridView_Manage.Rows[dataGridView_Manage.CurrentRow.Index].Cells[0].Value.ToString();
            DialogResult dr = MessageBox.Show("删除项目将删除参数文件，确认删除 " + Project + " 项目吗？", "提示", MessageBoxButtons.OKCancel, MessageBoxIcon.Question);
            if (dr == System.Windows.Forms.DialogResult.OK)
            {
                if (DeleteProjectFile(Project))
                {
                    this.dataGridView_Manage.Rows.RemoveAt(dataGridView_Manage.CurrentCell.RowIndex);
                    //parentForm.DeleteOption = true;
                }
            }
        }

        private void contextMenuStrip_Option_Opening(object sender, CancelEventArgs e)
        {

        }
    }
}
