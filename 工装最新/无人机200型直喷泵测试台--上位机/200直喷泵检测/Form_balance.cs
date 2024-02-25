using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace _200直喷泵检测
{
    public partial class Form_balance : Form
    {
        Form_Main parentForm;
        public Form_balance(Form_Main _parentForm)
        {
            this.parentForm = _parentForm;
            InitializeComponent();
        }


        private void saveData()
        {
            if (!this.parentForm.ini.ExistINIFile())
            {
                this.parentForm.creatConfigFile();
            }
            //  this.parentForm.ini.WriteString("COM", "ECUCom", ECUComBox.Text);
            this.parentForm.ini.WriteString("COM", "ScaleCom", scaleComBox.Text);
            //  this.parentForm.ini.WriteString("BAUD", "ECUComBaud", ECUBaudBox.Text);
            this.parentForm.ini.WriteString("BAUD", "ScaleComBaud", scaleBaudBox.Text);
        }

        private void Form_balance_Load(object sender, EventArgs e)
        {
            this.MaximizeBox = false;

            if (!this.parentForm.ini.ExistINIFile())
            {
                this.parentForm.creatConfigFile();
            }
            string AutoGetWeightStatus = parentForm.ini.ReadString("GETWEIGHT", "AutoReadScale", "N");
            // ECUComBox.Text = parentForm.ini.ReadString("COM", "ECUCom", "");
            scaleComBox.Text = parentForm.ini.ReadString("COM", "ScaleCom", "");
            //   ECUBaudBox.Text = parentForm.ini.ReadString("BAUD", "ECUComBaud", "");
            scaleBaudBox.Text = parentForm.ini.ReadString("BAUD", "ScaleComBaud", "");

            if (ComReadError(scaleComBox.Text))
            {
                scaleComBox.Text = "COM2";
            }


            if (BaudReadError(scaleBaudBox.Text))
            {
                scaleBaudBox.Text = "600";
            }

            if (AutoGetWeightStatus == "Y")
            {
                scaleGroupBox.Enabled = true;
            }
            else
            {
                scaleGroupBox.Enabled = false;
            }
            searchSerialPort();
        }

        private bool ComReadError(string str)
        {
            bool ret = false;
            if (str.Length > 0)
                if (str[0] != 'C' && str[1] != 'O' && str[2] != 'M') ret = true;
            return ret;
        }

        private bool BaudReadError(string str)
        {
            int i;
            bool error = false;
            for (i = 0; i < str.Length; i++)
            {
                if (str[i] > '9' || str[i] < '0')
                {
                    error = true;
                    break;
                }
            }
            return error;
        }

        private void searchSerialPort()
        {
            string[] str = SerialPort.GetPortNames();

            if (str == null)
            {
                //MessageBox.Show("本机没有串口", "Error");
                return;
            }
            string str1 = null;
            foreach (string s in System.IO.Ports.SerialPort.GetPortNames())
            {
                str1 += s;
                str1 += " ";
            }
            if (str1 == null) str1 = "串口未连接";
            ComDispTextBox.Text = str1;

        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            saveData();
            this.Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }

    }
}
