using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _200直喷泵检测
{
    public partial class TestFileSetForm : Form
    {
        string[] keyVal = new string[4] { "TestTXT", "TestExcel", "VerifyTXT", "MessJetTXT" };
        Form_Main parentForm;
        public TestFileSetForm(Form_Main _parentForm)
        {
            InitializeComponent();

            this.parentForm = _parentForm;
        }

        private void TestFileSetForm_Load(object sender, EventArgs e)
        {
            this.MaximizeBox = false;

            checkedListBox1.Hide();
            label3.Hide();
            if (!this.parentForm.ini.ExistINIFile())
            {
                this.parentForm.creatConfigFile();
            }

            for (int i = 0; i < this.parentForm.recordFilePartName.Length; i++)
            {
                this.checkedListBox1.Items.Add(this.parentForm.recordFilePartName[i]);
            }

            for (int i = 0; i < this.checkedListBox1.Items.Count; i++)
            {
                if (checkedListBox1.Items[i].Equals("数据表格(excel)"))
                    this.checkedListBox1.SetItemChecked(i, false);
                else
                    this.checkedListBox1.SetItemChecked(i, true);

                if (this.checkedListBox1.GetItemChecked(i))
                {
                    this.parentForm.ini.WriteString("RECORD", keyVal[i], "Y");
                }
                else
                {
                    this.parentForm.ini.WriteString("RECORD", keyVal[i], "N");
                }
            }

            string path = this.parentForm.ini.ReadString("FILEPATH", "RecordPath", " ");
            comboBox1.Items.Add(path);
            comboBox1.Text = path;
            comboBox1.SelectionStart = comboBox1.Text.Length;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog();
            System.Windows.Forms.DialogResult result = folderBrowserDialog.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                this.comboBox1.Text = folderBrowserDialog.SelectedPath;
                if (this.comboBox1.Text[comboBox1.Text.Length - 1] != '\\')
                    this.comboBox1.Text += @"\";
                this.comboBox1.Items.Insert(0, this.comboBox1.Text);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (!this.parentForm.ini.ExistINIFile())
            {
                this.parentForm.creatConfigFile();
            }

            for (int i = 0; i < this.checkedListBox1.Items.Count; i++)
            {
                if (this.checkedListBox1.GetItemChecked(i))
                {
                    this.parentForm.ini.WriteString("RECORD", keyVal[i], "Y");
                }
                else
                {
                    this.parentForm.ini.WriteString("RECORD", keyVal[i], "N");
                }
            }
            this.parentForm.ini.WriteString("FILEPATH", "RecordPath", this.comboBox1.Text);
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }


    }
}
