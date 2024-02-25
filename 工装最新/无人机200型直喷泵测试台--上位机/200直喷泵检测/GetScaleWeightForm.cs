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
using System.Runtime.InteropServices; //DllImport命名空间
using System.IO;
using System.IO.Ports;
using System.Reflection;
using System.Threading;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Data.OleDb;
//using Microsoft.Office.Interop.Excel;
using System.Collections.Specialized;
using System.Collections;
using System.Windows.Forms.DataVisualization.Charting;

namespace _200直喷泵检测
{
    public partial class GetScaleWeightForm : Form
    {

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_OpenDevice(UInt32 DeviceType, UInt32 DeviceInd, UInt32 Reserved);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_CloseDevice(UInt32 DeviceType, UInt32 DeviceInd);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_InitCAN(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_INIT_CONFIG pInitConfig);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ReadBoardInfo(UInt32 DeviceType, UInt32 DeviceInd, ref VCI_BOARD_INFO pInfo);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_GetReceiveNum(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ClearBuffer(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_StartCAN(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ResetCAN(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_Transmit(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_CAN_OBJ pSend, UInt32 Len);

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_Receive(UInt32 DeviceType, UInt32 DeviceInd, UInt32 CANInd, ref VCI_CAN_OBJ pReceive, UInt32 Len, Int32 WaitTime);

        /*------------其他函数描述---------------------------------*/

        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_ConnectDevice(UInt32 DevType, UInt32 DevIndex);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_UsbDeviceReset(UInt32 DevType, UInt32 DevIndex, UInt32 Reserved);
        [DllImport("controlcan.dll")]
        static extern UInt32 VCI_FindUsbDevice(ref VCI_BOARD_INFO1 pInfo);

        //CAN设置
        static UInt32 m_devtype = 4;//USBCAN2
        UInt32 m_bOpen = 0;   //
        UInt32 m_devind = 0;  //设备索引
        UInt32 m_canind = 0;  //can通道索引
        VCI_CAN_OBJ[] m_recobj = new VCI_CAN_OBJ[2000];
        UInt32[] m_arrdevtype = new UInt32[20];

        // CAN定时器
        System.Timers.Timer CAN_time = new System.Timers.Timer(500);

        public string ScaleWeight
        {
            get { return textBox1.Text; }
        }
        public GetScaleWeightForm()
        {
            InitializeComponent();
        }

        //获取父窗体ListView控件()
        
        private void GetScaleWeightForm_Load(object sender, EventArgs e)
        {
            this.MaximizeBox = false;

            textBox1.Focus();
            label3.Hide();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (textBox1.Text.Length > 0)
            {
                label3.Hide();
            }
        }

        private void textBox1_Click(object sender, EventArgs e)
        {
            label3.Hide();
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {

        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (((int)e.KeyChar < 48 || (int)e.KeyChar > 57)
                && (int)e.KeyChar != 8 && (int)e.KeyChar != 13 && (int)e.KeyChar != 46)
                e.Handled = true;
            //小数点
            if ((int)e.KeyChar == 46)
            {
                if (textBox1.Text.Length <= 0)
                    e.Handled = true;
                else
                {
                    float f;
                    float oldf;
                    bool b1 = false, b2 = false;
                    b1 = float.TryParse(textBox1.Text, out oldf);
                    b2 = float.TryParse(textBox1.Text + e.KeyChar.ToString(), out f);
                    if (b2 == false)
                    {
                        if (b1 == true)
                            e.Handled = true;
                        else
                            e.Handled = false;
                    }
                }
            }
            else if ((int)e.KeyChar == 13)
            {
                if (textBox1.Text.Length <= 0)
                {
                    label3.Show();
                }
                else
                {
                    this.DialogResult = DialogResult.OK;
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text.Length <= 0)
            {
                label3.Show();
            }
            else
            {
                this.DialogResult = DialogResult.OK;
            }
        }

        private void GetScaleWeightForm_KeyPress(object sender, KeyPressEventArgs e)
        {
            char key = e.KeyChar;
        }


        unsafe public bool Can_Send(string str, string ID)
        {
            VCI_CAN_OBJ sendobj = new VCI_CAN_OBJ();
            sendobj.ExternFlag = 0;  //帧类型
            sendobj.RemoteFlag = 0;//帧格式
            sendobj.ID = System.Convert.ToUInt32("0x" + ID, 16);   //ID
            int len = (str.Length + 1) / 3; //数据长度
            sendobj.DataLen = System.Convert.ToByte(len);
            String strdata = str;
            int i = -1;
            if (i++ < len - 1)
                sendobj.Data[0] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[1] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[2] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[3] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[4] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[5] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[6] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (i++ < len - 1)
                sendobj.Data[7] = System.Convert.ToByte("0x" + strdata.Substring(i * 3, 2), 16);
            if (VCI_Transmit(4, 0, 0, ref sendobj, 1) == 0)
            {
                MessageBox.Show("发送失败", "错误",
                        MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                return false;
            }
            return true;
        }
        public void Stop_trick() //按下停止后界面显示
        {

            Can_Send(MYSettings.Default.stop, MYSettings.Default.send_ID);

        }

        private void button2_Click(object sender, EventArgs e)
        {
            if(DialogResult.OK==MessageBox.Show("你确定要退出吗？","关闭提示",MessageBoxButtons.OKCancel,MessageBoxIcon.Question))
            {
                Stop_trick();
                System.Windows.Forms.Application.Exit();
                Environment.Exit(0);
            }
            else
            {
              
            }
        }

        private void GetScaleWeightForm_FormClosed(object sender, FormClosedEventArgs e)
        {

        }
    }
}
