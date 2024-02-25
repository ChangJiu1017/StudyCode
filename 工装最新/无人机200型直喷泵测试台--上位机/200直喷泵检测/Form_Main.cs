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
using Microsoft.Office.Interop.Excel;
using System.Collections.Specialized;
using System.Collections;
using System.Windows.Forms.DataVisualization.Charting;


//1.ZLGCAN系列接口卡信息的数据类型。
public struct VCI_BOARD_INFO
{
    public UInt16 hw_Version;
    public UInt16 fw_Version;
    public UInt16 dr_Version;
    public UInt16 in_Version;
    public UInt16 irq_Num;
    public byte can_Num;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)]
    public byte[] str_Serial_Num;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 40)]
    public byte[] str_hw_Type;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
    public byte[] Reserved;
}

/////////////////////////////////////////////////////
//2.定义CAN信息帧的数据类型。
unsafe public struct VCI_CAN_OBJ  //使用不安全代码
{
    public uint ID;
    public uint TimeStamp;        //时间标识
    public byte TimeFlag;         //是否使用时间标识
    public byte SendType;         //发送标志。保留，未用
    public byte RemoteFlag;       //是否是远程帧
    public byte ExternFlag;       //是否是扩展帧
    public byte DataLen;          //数据长度
    public fixed byte Data[8];    //数据
    public fixed byte Reserved[3];//保留位

}

//3.定义初始化CAN的数据类型
public struct VCI_INIT_CONFIG
{
    public UInt32 AccCode;
    public UInt32 AccMask;
    public UInt32 Reserved;
    public byte Filter;     //0或1接收所有帧。2标准帧滤波，3是扩展帧滤波。
    public byte Timing0;    //波特率参数，具体配置，请查看二次开发库函数说明书。
    public byte Timing1;
    public byte Mode;       //模式，0表示正常模式，1表示只听模式,2自测模式
}

/*------------其他数据结构描述---------------------------------*/
//4.USB-CAN总线适配器板卡信息的数据类型1，该类型为VCI_FindUsbDevice函数的返回参数。
public struct VCI_BOARD_INFO1
{
    public UInt16 hw_Version;
    public UInt16 fw_Version;
    public UInt16 dr_Version;
    public UInt16 in_Version;
    public UInt16 irq_Num;
    public byte can_Num;
    public byte Reserved;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
    public byte[] str_Serial_Num;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
    public byte[] str_hw_Type;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
    public byte[] str_Usb_Serial;
}

/*------------数据结构描述完成---------------------------------*/

public struct CHGDESIPANDPORT
{
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
    public byte[] szpwd;
    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)]
    public byte[] szdesip;
    public Int32 desport;

    public void Init()
    {
        szpwd = new byte[10];
        szdesip = new byte[20];
    }
}


namespace _200直喷泵检测
{
    public partial class Form_Main : System.Windows.Forms.Form
    {

        #region   const int VCI_PCI5121 = 1;
        const int VCI_PCI9810 = 2;
        const int VCI_USBCAN1 = 3;
        const int VCI_USBCAN2 = 4; //设备型号 TQS传感器 本实验所有设备类型；
        const int VCI_USBCAN2A = 4;
        const int VCI_PCI9820 = 5;
        const int VCI_CAN232 = 6;
        const int VCI_PCI5110 = 7;
        const int VCI_CANLITE = 8;
        const int VCI_ISA9620 = 9;
        const int VCI_ISA5420 = 10;
        const int VCI_PC104CAN = 11;
        const int VCI_CANETUDP = 12;
        const int VCI_CANETE = 12;
        const int VCI_DNP9810 = 13;
        const int VCI_PCI9840 = 14;
        const int VCI_PC104CAN2 = 15;
        const int VCI_PCI9820I = 16;
        const int VCI_CANETTCP = 17;
        const int VCI_PEC9920 = 18;
        const int VCI_PCI5010U = 19;
        const int VCI_USBCAN_E_U = 20;
        const int VCI_USBCAN_2E_U = 21;
        const int VCI_PCI5020U = 22;
        const int VCI_EG2OT_CAN = 23;
        #endregion
   
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


        public bool Get_T = false;
        public bool GetStandFormCloseEnable = false;
        //private bool GetStandardT_AD = false;
        //private bool GetStandardTPCB_ADing = false;
        public bool Auto_Test = false;

        public Int32 receive_T1Get;
        public Int32 receive_PGet;
        byte T_Bad = 0;
        byte P_Bad = 0;
        public bool Flag_T_Bad=false;
        public bool Flag_P_Bad=false;
        UInt16 ii = 0;
        public const int G1_SUCCESS = 0;
        public const int G1_FAIL = 1;

        bool IsComReceiving = false;    //是否正在进行数据接收

        //  从Adjust4.20正式版复制过来的

        //   /////////////////////////////////////////////
        public const int TICK_TIME = 50;//50毫秒
        //数据模式类型
        public const int REMARK_TYPE = 0;
        public const int KESEI_TYPE = 1;
        public const int COS_TYPE = 2;
        public const int BTDC_TYPE = 3;//又名IGN
        public const int BACK_TYPE = 4;
        public const int T1_TYPE = 5;

        //测试步骤
        public const int TEST_START = 0;
        public const int TEST_20 = 99;//2019.1.3
        public const int TEST_T1MSCAN = 1;
        public const int TEST_550 = 2;
        public const int TEST_540 = 3;
        public const int TEST_541 = 4;
        public const int TEST_53 = 5;//2017.6.26
        public const int TEST_FINISH = 20;
        public const int TEST_ABORT = 23;

        public const int VERIFY_START = 0;
        public const int VERIFY_53_30 = 1;
        public const int VERIFY_50_60 = 2;
        public const int VERIFY_30 = 5;
        public const int VERIFY_20 = 6;
        public const int VERIFY_FINISH = 20;
        public const int VERIFY_FAIL = 21;
        public const int VERIFY_ABORT = 23;

        public const int FILLING_START = 0;
        public const int FILLING = 1;
        public const int FILLING_FINISH = 20;
        public const int CANCEL_FILLING = 23;

        public const int SUCCESS = 0;
        public const int FAIL = 250;
       
        private bool ScaleSerialPortIsOpen = false;        //电子秤串口开启标志
       
       

        private int ScaleTimeTick = 0;                     //时间滴答
    
        Queue controlRecQueue = new Queue();               //控制器接收队列
        public bool adjustEnable = false;                  //表定使能
        public bool writeEnable = false;                   //写EEPROM使能
        public bool verifyEnable = false;                  //检验
        public bool fillEnable = false;                    //注水
        private int adjustStep = TEST_START;               //标定步骤
        private int verifyStep = VERIFY_START;             //校验步骤
      
        //private bool ScaleSerialPortIsOpen = false;        //电子秤串口开启标志
        private static object accessComObj = new object(); //写串口互斥
        byte[] scaleBuff = new byte[14];                   //电子秤数据缓冲区
        byte[] data = new byte[2];                         //数据发送
        //byte[] controlBuff = new byte[20];                 //控制器数据缓冲区
        private bool scaleComReceive = false;              //电子秤接收数据标志
        //private bool scaleDataReceive = false;             //电子秤真正接收数据标志
        private bool mIsTryToClosePort = false;            //是否尝试关闭串口
        SerialPort scaleSerialPort = new SerialPort();
        static string fName = System.Windows.Forms.Application.StartupPath + @"\config.ini";
        public IniFile ini = new IniFile(fName);

        //private bool addDataToStructFlag = false;          //是否存入数据结构
        private bool addDataToScaleStructFlag = false;
        //private bool mIsAddingDataToStruct = false;        //是否正在存入数据结构
        private bool mIsAddingDataToScaleStruct = false;     //是否正在存入电子秤数据
        public bool T3adjust = true;                         //判断T3
        public bool T1adjust = true;                         //判断T1
        public bool Openserial = false;                      //串口打开标志
        
        byte[] controlBuff = new byte[20];                   //控制器数据缓冲区

        byte[] HUIWEI=new byte[30];
        byte[] ZUOGONG = new byte[30];
        byte[] PINLV = new byte[30];

        #region//采样电压数组初始化
        //string str_volt = "";//保存电压数据的字符串
        //string str_elect = ""; //保存电流数据的字符串
        //string str_pro = "";   //保存压力数据的字符串
        //string str_tem = "";   //保存温度数据的字符串

        string weight_sum = "";
        double weight_average = 0;
        double weight_all = 0;
        double[] weight= new double[24];     //泵流量采样值 
        double[] electric_save = new double[30000];//电流采样值 存入该数组中进行保存
        double[] voltage_save = new double[30000];//电压采样值 存入该数组中进行保存

        double volt_average = 0;
        double elect_average = 0;
        string volt_all = "";
        string electric_all = "";


        double volt_average_1 = 0;
        double volt_average_2 = 0;
        double volt_average_3 = 0;
        double volt_average_4 = 0;
        double volt_average_5 = 0;
        double volt_average_6 = 0;
        double volt_average_7 = 0;
        double volt_average_8 = 0;
        double volt_average_9 = 0;
        double volt_average_10 = 0;
        double volt_average_11 = 0;
        double volt_average_12 = 0;
        double volt_average_13 = 0;
        double volt_average_14 = 0;
        double volt_average_15 = 0;
        double volt_average_16 = 0;
        double volt_average_17 = 0;
        double volt_average_18 = 0;
        double volt_average_19 = 0;
        double volt_average_20 = 0;
        double volt_average_21 = 0;
        double volt_average_22 = 0;
        double volt_average_23 = 0;
        double volt_average_24 = 0;


        double elect_average_1 = 0;
        double elect_average_2 = 0;
        double elect_average_3 = 0;
        double elect_average_4 = 0;
        double elect_average_5 = 0; 
        double elect_average_6 = 0;
        double elect_average_7 = 0;
        double elect_average_8 = 0;
        double elect_average_9 = 0;
        double elect_average_10 = 0;
        double elect_average_11 = 0;
        double elect_average_12 = 0;
        double elect_average_13 = 0;
        double elect_average_14 = 0;
        double elect_average_15 = 0;
        double elect_average_16 = 0;
        double elect_average_17 = 0;
        double elect_average_18 = 0;
        double elect_average_19 = 0;
        double elect_average_20 = 0;
        double elect_average_21 = 0;
        double elect_average_22 = 0;
        double elect_average_23 = 0;
        double elect_average_24 = 0;



        double volt_sum = 0;
        double elect_sum = 0;
        double volt_sum_1 = 0;
        double volt_sum_2 = 0;
        double volt_sum_3 = 0;
        double volt_sum_4 = 0;
        double volt_sum_5 = 0;
        double volt_sum_6 = 0;
        double volt_sum_7 = 0;
        double volt_sum_8 = 0;
        double volt_sum_9 = 0;
        double volt_sum_10 = 0;
        double volt_sum_11 = 0;
        double volt_sum_12 = 0;
        double volt_sum_13 = 0;
        double volt_sum_14 = 0;
        double volt_sum_15 = 0;
        double volt_sum_16 = 0;
        double volt_sum_17 = 0;
        double volt_sum_18 = 0;
        double volt_sum_19 = 0;
        double volt_sum_20 = 0;
        double volt_sum_21 = 0;
        double volt_sum_22 = 0;
        double volt_sum_23 = 0;
        double volt_sum_24 = 0;

        double elect_sum_1 = 0;
        double elect_sum_2 = 0;
        double elect_sum_3 = 0;
        double elect_sum_4 = 0;
        double elect_sum_5 = 0;
        double elect_sum_6 = 0;
        double elect_sum_7 = 0;
        double elect_sum_8 = 0; 
        double elect_sum_9 = 0;
        double elect_sum_10 = 0;
        double elect_sum_11 = 0;
        double elect_sum_12 = 0;
        double elect_sum_13 = 0;
        double elect_sum_14 = 0;
        double elect_sum_15 = 0;
        double elect_sum_16 = 0;
        double elect_sum_17 = 0;
        double elect_sum_18 = 0;
        double elect_sum_19 = 0;
        double elect_sum_20 = 0;
        double elect_sum_21 = 0;
        double elect_sum_22 = 0;
        double elect_sum_23 = 0;
        double elect_sum_24 = 0;




        double T_sum = 0;
        double P_sum = 0;

        double[] T_save = new double[30000];
        double[] P_save = new double[30000];

        double Temp_record = 0;

        double T_average = 0;
        double P_average = 0;
        int  weight_n = 0;

        UInt32 overrun_V=0;
        UInt32 overrun_I=0;
        UInt32 overrun_P = 0;
        UInt32 overrun_T = 0;

        UInt32 Flag_time = 0;  //喷射周期记录
        //UInt32 Frequ = 0;
        //UInt32 Spray_time = 0;
        //UInt32 T1 = 0;
        //UInt32 T2 = 0;
        //UInt32 T3 = 0;
        //UInt32 T = 0;
        //string  count_time;
        //串口重量保存变量

        UInt32 G1_1 = 0;
        UInt32 G1_2 = 0;
        UInt32 G1_3 = 0;
        UInt32 G1_4 = 0;
        UInt32 G1_5 = 0;
        UInt32 G1_6 = 0;
        UInt32 G1_7 = 0;
        UInt32 G1_8 = 0;
        UInt32 G1_9 = 0;
        UInt32 G1_10 = 0;
        UInt32 G1_11 = 0;
        UInt32 G1_12 = 0;
        UInt32 G1_13 = 0;
        UInt32 G1_14 = 0;
        UInt32 G1_15 = 0;
        UInt32 G1_16 = 0;
        UInt32 G1_17 = 0;
        UInt32 G1_18 = 0;
        UInt32 G1_19 = 0;
        UInt32 G1_20 = 0;
        UInt32 G1_21 = 0;
        UInt32 G1_22 = 0;
        UInt32 G1_23 = 0;
        //UInt32 G1_24 = 0;
        //bool T_One = false; //温度判断标志位
        //bool P_One = false; //压力判断标志位             

        Int32 progress_kk = 0;
        Int32 count_num  = 0;
        #endregion

        #region//重量高低限位与使能初始化
        private string ProjectName = "";
        public string PrjectCoifigFile;
        public bool DeleteOption = false;
        private byte ManageCoifig = 0;
        public UInt16 CANBaudrate;

        public double V_Up;
        public double V_Low;
        public bool Place_V;

        public UInt16 T_Up;
        public UInt16 T_Low;
        public bool Place_T;

        public double P_Up;
        public double P_Low;
        public bool Place_P;

        public UInt16 I_Up;
        public UInt16 I_Low;
        public bool Place_I;


        public UInt16 Place0_Low;
        public UInt16 Place0_Up;
        public bool Place0;


        public UInt16 Place1_Low;
        public UInt16 Place1_Up;
        public UInt16 Place1_HuiWei;
        public UInt16 Place1_ZuoGong;
        public UInt16 Place1_PinLv;
        public bool Place1;

        public UInt16 Place2_Low;
        public UInt16 Place2_Up;
        public UInt16 Place2_HuiWei;
        public UInt16 Place2_ZuoGong;
        public UInt16 Place2_PinLv;
        public bool Place2;
        public UInt16 Place3_Low;
        public UInt16 Place3_Up;
        public UInt16 Place3_HuiWei;
        public UInt16 Place3_ZuoGong;
        public UInt16 Place3_PinLv;
        public bool Place3;
        public UInt16 Place4_Low;
        public UInt16 Place4_Up;
        public UInt16 Place4_HuiWei;
        public UInt16 Place4_ZuoGong;
        public UInt16 Place4_PinLv;
        public bool Place4;
        public UInt16 Place5_Low;
        public UInt16 Place5_Up;
        public UInt16 Place5_HuiWei;
        public UInt16 Place5_ZuoGong;
        public UInt16 Place5_PinLv;
        public bool Place5;
        public UInt16 Place6_Low;
        public UInt16 Place6_Up;
        public UInt16 Place6_HuiWei;
        public UInt16 Place6_ZuoGong;
        public UInt16 Place6_PinLv;
        public bool Place6;
        public UInt16 Place7_Low;
        public UInt16 Place7_Up;
        public UInt16 Place7_HuiWei;
        public UInt16 Place7_ZuoGong;
        public UInt16 Place7_PinLv;
        public bool Place7;
        public UInt16 Place8_Low;
        public UInt16 Place8_Up;
        public UInt16 Place8_HuiWei;
        public UInt16 Place8_ZuoGong;
        public UInt16 Place8_PinLv;
        public bool Place8;
        public UInt16 Place9_Low;
        public UInt16 Place9_Up;
        public UInt16 Place9_HuiWei;
        public UInt16 Place9_ZuoGong;
        public UInt16 Place9_PinLv;
        public bool Place9;
        public UInt16 Place10_Low;
        public UInt16 Place10_Up;
        public UInt16 Place10_HuiWei;
        public UInt16 Place10_ZuoGong;
        public UInt16 Place10_PinLv;
        public bool Place10;
        public UInt16 Place11_Low;
        public UInt16 Place11_Up;
        public UInt16 Place11_HuiWei;
        public UInt16 Place11_ZuoGong;
        public UInt16 Place11_PinLv;
        public bool Place11;
        public UInt16 Place12_Low;
        public UInt16 Place12_Up;
        public UInt16 Place12_HuiWei;
        public UInt16 Place12_ZuoGong;
        public UInt16 Place12_PinLv;
        public bool Place12;
        public UInt16 Place13_Low;
        public UInt16 Place13_Up;
        public UInt16 Place13_HuiWei;
        public UInt16 Place13_ZuoGong;
        public UInt16 Place13_PinLv;
        public bool Place13;
        public UInt16 Place14_Low;
        public UInt16 Place14_Up;
        public UInt16 Place14_HuiWei;
        public UInt16 Place14_ZuoGong;
        public UInt16 Place14_PinLv;
        public bool Place14;
        public UInt16 Place15_Low;
        public UInt16 Place15_Up;
        public UInt16 Place15_HuiWei;
        public UInt16 Place15_ZuoGong;
        public UInt16 Place15_PinLv;
        public bool Place15;
        public UInt16 Place16_Low;
        public UInt16 Place16_Up;
        public UInt16 Place16_HuiWei;
        public UInt16 Place16_ZuoGong;
        public UInt16 Place16_PinLv;
        public bool Place16;
        public UInt16 Place17_Low;
        public UInt16 Place17_Up;
        public UInt16 Place17_HuiWei;
        public UInt16 Place17_ZuoGong;
        public UInt16 Place17_PinLv;
        public bool Place17;
        public UInt16 Place18_Low;
        public UInt16 Place18_Up;
        public UInt16 Place18_HuiWei;
        public UInt16 Place18_ZuoGong;
        public UInt16 Place18_PinLv;
        public bool Place18;
        public UInt16 Place19_Low;
        public UInt16 Place19_Up;
        public UInt16 Place19_HuiWei;
        public UInt16 Place19_ZuoGong;
        public UInt16 Place19_PinLv;
        public bool Place19;
        public UInt16 Place20_Low;
        public UInt16 Place20_Up;
        public UInt16 Place20_HuiWei;
        public UInt16 Place20_ZuoGong;
        public UInt16 Place20_PinLv;
        public bool Place20;
        public UInt16 Place21_Low;
        public UInt16 Place21_Up;
        public UInt16 Place21_HuiWei;
        public UInt16 Place21_ZuoGong;
        public UInt16 Place21_PinLv;
        public bool Place21;
        public UInt16 Place22_Low;
        public UInt16 Place22_Up;
        public UInt16 Place22_HuiWei;
        public UInt16 Place22_ZuoGong;
        public UInt16 Place22_PinLv;
        public bool Place22;
        public UInt16 Place23_Low;
        public UInt16 Place23_Up;
        public UInt16 Place23_HuiWei;
        public UInt16 Place23_ZuoGong;
        public UInt16 Place23_PinLv;
        public bool Place23;
        public UInt16 Place24_Low;
        public UInt16 Place24_Up;
        public UInt16 Place24_HuiWei;
        public UInt16 Place24_ZuoGong;
        public UInt16 Place24_PinLv;
        public bool Place24;
        #endregion

        public Form_Main()
        {
            InitializeComponent();         
        }
   
        public void Plice_False() 
        {
            Place_T=false;
            Place_I = false;
            Place_V = false;
            Place_P = false;
            Place0 = false;
            Place1 = false;
            Place2 = false;
            Place3 = false;
            Place4 = false;
            Place5 = false;
            Place6 = false;
            Place7 = false;
            Place8 = false;
            Place9 = false;
            Place10 = false;
            Place11 = false;
            Place12 = false;
            Place13 = false;
            Place14 = false;
            Place15 = false;
            Place16 = false;
            Place17 = false;
            Place18 = false;
            Place19 = false;
            Place20 = false;
            Place21 = false;
            Place22 = false;
            Place23 = false;
        }
   
        private void Form_Main_Load(object sender, EventArgs e)//进入主窗口
        {
            SNComboBox.Focus();
            //ReadParamFile();
            textBox_DianYa.Visible = false;
            textBox_DianLiu.Visible = false;
            textBox_YaLi.Visible = false;
            textBox_WenDu.Visible = false;
            label15.Visible = false;
            label16.Visible = false;
            label17.Visible = false;
            label18.Visible = false;
            label19.Visible = false;
            if (Config.Param.AutoReadScale == "Y")
            {
                自动读秤ToolStripMenuItem.Checked = true;
            }
            else
            {
                自动读秤ToolStripMenuItem.Checked = false;
            }
            Plice_False();   //初始化重量采样变量
            ProjectName = MYSettings.Default.ProjectName;//Coifig.Param.ProjectName;
            //添加项目列表
            AddProject2Select();
            //CAN定时器设置
            CAN_time.Elapsed += new System.Timers.ElapsedEventHandler(Receive_CAN);//到达时间执行事件
            CAN_time.AutoReset = true;                                             //一直执行
            CAN_time.Enabled = false;
            CAN_time.SynchronizingObject = this;
            SerialPort scaleSerialPort = new SerialPort(); //串口资源配置 
            Auto_Test = false;//全速测试·
        }

        #region //无效按键
        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
        #endregion

        public  bool  Writefile(string Path,string writefile)
        {           
            File.AppendAllText(Path, writefile);
            return true;
        }

        /// <summary>
        /// 记录上位机采集到的数据，并存入文件夹
        /// </summary>
        /// <param name="mystr"></param>
      
        public void writedata(string mystr)
        {
            //string fafa = mystr;      
            string fname;
            fname = @"..\\Debug\\无人机200直喷泵记录数据\\Save" + "\\" + 
                                  DateTime.Now.Year.ToString() + "年" + DateTime.Now.Month.ToString() + "月"  + "无人机200型直喷泵测试记录数据" + ".CSV";
            
            //File.AppendAllText(fname, mystr);
            if (!File.Exists(fname))
            {
                FileStream myFs = new FileStream(fname, FileMode.OpenOrCreate,FileAccess.Write);
                StreamWriter mySw = new StreamWriter(myFs, System.Text.Encoding.GetEncoding("UTF-8")); //设置数据的编码格式
                try
                {
                    mySw.WriteLine("项目名称,产品编号,测试时间,流量品质,温度||压力,电流||电压,阶段电流采样平均值(mA),阶段电压采样平均值(V),电流不合格次数,电压不合格次数,温度采样值,压力采样值,重量采样平均值,重量-1,重量-2,重量-3,重量-4,重量-5,重量-6,重量-7,重量-8,重量-9,重量-10,重量-11,重量-12,重量-13,重量-14,重量-15,重量-16,重量-17,重量-18,重量-19,重量-20,重量-21,重量-22,重量-23");//表头
                    mySw.Write(mystr);//写数据
                }
                catch (Exception ex)
                {

                    MessageBox.Show(ex.Message, "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
                mySw.Close();
                myFs.Close();
            }
            else
            {
                FileStream myFs = new FileStream(fname, FileMode.Append, FileAccess.Write);
                StreamWriter mySw = new StreamWriter(myFs, System.Text.Encoding.GetEncoding("UTF-8"));
                try
                {
                    mySw.Write(mystr);
                }
                catch (SystemException ex)
                {
                    MessageBox.Show(ex.Message, "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
                mySw.Close();
                myFs.Close();
            }

        }

        public void LOND(System.Timers.Timer time)  //打开设备》设置参数》设备:USBCAN2 通道：0 索引：0 波特率250kbps;
        {
            if (m_bOpen == 0)
            {
                MessageBox.Show("请连接CAN,再打开！");
                CAN_time.Enabled = false;
                //timer_CAN2.Enabled = false;
                VCI_CloseDevice(m_devtype, m_devind);
                return;
            }
            else
            {
                m_devtype = 4;   //can类型
                m_devind = 0;    //CAN索引
                m_canind = 0;    //CAN通道
                if (VCI_OpenDevice(m_devtype, m_devind, 0) == 0) return;
                m_bOpen = 1;
                VCI_INIT_CONFIG coifig = new VCI_INIT_CONFIG();
                coifig.AccCode = 0;
                coifig.AccMask = 4294967295;
                coifig.Timing0 = 0x01;
                coifig.Timing1 = 0x1C;
                coifig.Filter = 1;
                coifig.Mode = 0;
                VCI_InitCAN(m_devtype, m_devind, m_canind, ref coifig);
                if (Convert.ToBoolean(VCI_StartCAN(m_devtype, m_devind, m_canind)))
                {
                    time.Enabled = true;
                }
            }
        }

        public string[] recordFilePartName = new string[4] { "标定记录(txt)", "数据表格(excel)", "校验记录(txt)", "计量喷嘴测试(txt)" };

        /// <summary>
        /// 点击记录按键
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_record_Click(object sender, EventArgs e)
        {
            FormRecord_2 Record = new FormRecord_2(this);
            Record.StartPosition = FormStartPosition.CenterParent;//调出用于记录的窗体 居中显示
            Record.FormClosed += delegate (object obj, FormClosedEventArgs arg)
            {
               
            };
            //this.BackColor = Color.Brown;
            Record.ShowDialog();
        }

        /// <summary>
        /// EXCEL导入方法
        /// </summary>
        /// <param name="excelpath"></param>
        /// <returns></returns>
        /// <exception cref="Exception"></exception>
        private System.Data.DataTable ReadFromExcel(string excelpath)
        
        {
            string sExt = System.IO.Path.GetExtension(excelpath);
            string sConn = null;
            if (sExt == ".xlsx")
            {
                sConn = "Provider=Microsoft.ACE.OLEDB.12.0;" + "Data Source=" + excelpath + ";" + "Extended Properties='Excel 12.0;HDR=YES'";
            }
            else if (sExt == ".xls")
            {
                sConn = "Provider=Microsoft.Jet.OLEDB.4.0;" + "Data Source=" + excelpath + ";" + "Extended Properties=Excel 8.0";
            }
            else
            {
                throw new Exception("文件格式有误");
            }
            OleDbConnection oledbConn = new OleDbConnection(sConn);
            oledbConn.Open();
            OleDbDataAdapter command = new OleDbDataAdapter("SELECT * FROM [Sheet1$]", oledbConn);
            DataSet ds = new DataSet();
            command.Fill(ds);
            oledbConn.Close();
            return ds.Tables[0];
        } //EXCEL导入

        private void AddProject2Select()
        {
            string text;
            string ProjectName;
            uint ProjectCount = 0;
            string ProjPath;
            string fname = System.Windows.Forms.Application.StartupPath + @"\DCUProject.proj";
            if (!File.Exists(fname))
            {
                FileStream myFs = new FileStream(fname, FileMode.Create);
                File.SetAttributes(fname, FileAttributes.Hidden);
                myFs.Close();
            }
            System.IO.StreamReader file = new StreamReader(fname);
            comboBox_Project.Text = "";
            if (File.Exists(fname))
            {
                comboBox_Project.Items.Clear();
                while (true)
                {
                    text = file.ReadLine();
                    if (text != null)
                    {
                        if (text.Equals("")) continue;
                        if (text.Substring(0, 1).Equals(":"))//头格式正确
                        {
                            ProjectName = text.Replace(":", "");
                            text = file.ReadLine();//读下一行
                            if (text != null)
                            {
                                ProjPath = System.Windows.Forms.Application.StartupPath + text.Replace("|", "");
                                if (File.Exists(ProjPath))
                                {
                                    ProjectCount++;
                                    comboBox_Project.Items.Add(ProjectName);
                                    //inserToProjComboBox(ProjectName);
                                    if (MYSettings.Default.ProjectName == ProjectName)
                                    {
                                        comboBox_Project.Text = ProjectName;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            file.Close();
        }

        public bool ReadParamFile()
        {
            string text;
            int row;
            bool ret = true;

            row = dataGridView_Param.Rows.Count;

            for(byte i = 0; i <= row-5; i++)
            {
                HUIWEI[i] = (byte)Convert.ToUInt32(dataGridView_Param.Rows[i].Cells[3].Value);
                ZUOGONG[i] = (byte)Convert.ToUInt32(dataGridView_Param.Rows[i].Cells[4].Value);
                PINLV[i] = (byte)Convert.ToUInt32(dataGridView_Param.Rows[i].Cells[5].Value);
            }

            for (byte i = 0; i <= row; i++)
            {
                try
                {
                        text = (dataGridView_Param.Rows[i].Cells[0].Value).ToString();                       
                        switch (text)
                        {
                        case "温度":
                            T_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                            T_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                            Place_T = true;
                            Console.WriteLine("温度=" + T_Low.ToString() + T_Up.ToString());
                            break;
                        case "压力":
                            P_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                            P_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                            Place_P = true;
                            Console.WriteLine("压力=" + P_Low.ToString() + P_Up.ToString());
                            break;
                        case "电流":
                                I_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                I_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place_I = true;
                                Console.WriteLine("电流=" + I_Low.ToString() + I_Up.ToString());
                                break;

                            case "电压":
                                V_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                V_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place_V = true;
                                Console.WriteLine("电流=" + V_Low.ToString() + V_Up.ToString());
                                break;
                            case "阶段-1":
                                Place1_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place1_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place1_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place1_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place1_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place1 = true;
                                Console.WriteLine("阶段-1=" + Place1_Low.ToString() + Place1_Up.ToString() + Place1_HuiWei.ToString() + Place1_ZuoGong.ToString() + Place1_PinLv.ToString());

                                break;

                            case "阶段-2":
                                Place2_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place2_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place2_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place2_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place2_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place2 = true;
                                Console.WriteLine("阶段-2=" + Place2_Low.ToString() + Place2_Up.ToString() + Place2_HuiWei.ToString() + Place2_ZuoGong.ToString() + Place2_PinLv.ToString());
                                break;

                            case "阶段-3":
                                Place3_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place3_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place3_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place3_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place3_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place3 = true;
                                Console.WriteLine("阶段-3=" + Place3_Low.ToString() + Place3_Up.ToString() + Place3_HuiWei.ToString() + Place3_ZuoGong.ToString() + Place3_PinLv.ToString());
                                break;

                            case "阶段-4":
                                Place4_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place4_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place4_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place4_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place4_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place4 = true;
                                Console.WriteLine("阶段-4=" + Place4_Low.ToString() + Place4_Up.ToString() + Place4_HuiWei.ToString() + Place4_ZuoGong.ToString() + Place4_PinLv.ToString());
                                break;

                            case "阶段-5":
                                Place5_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place5_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place5_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place5_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place5_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place5 = true;
                                Console.WriteLine("阶段-5=" + Place5_Low.ToString() + Place5_Up.ToString() + Place5_HuiWei.ToString() + Place5_ZuoGong.ToString() + Place5_PinLv.ToString());
                                break;

                            case "阶段-6":
                                Place6_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place6_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place6_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place6_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place6_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place6 = true;
                                Console.WriteLine("阶段-6=" + Place6_Low.ToString() + Place6_Up.ToString() + Place6_HuiWei.ToString() + Place6_ZuoGong.ToString() + Place6_PinLv.ToString());
                                break;

                            case "阶段-7":
                                Place7_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place7_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place7_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place7_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place7_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place7 = true;
                                Console.WriteLine("阶段-7=" + Place7_Low.ToString() + Place7_Up.ToString() + Place7_HuiWei.ToString() + Place7_ZuoGong.ToString() + Place7_PinLv.ToString());
                                break;

                            case "阶段-8":
                                Place8_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place8_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place8_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place8_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place8_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place8 = true;
                                Console.WriteLine("阶段-8=" + Place8_Low.ToString() + Place8_Up.ToString() + Place8_HuiWei.ToString() + Place8_ZuoGong.ToString() + Place8_PinLv.ToString());
                                break;

                            case "阶段-9":
                                Place9_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place9_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place9_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place9_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place9_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place9 = true;
                                Console.WriteLine("阶段-9=" + Place9_Low.ToString() + Place9_Up.ToString() + Place9_HuiWei.ToString() + Place9_ZuoGong.ToString() + Place9_PinLv.ToString());
                                break;

                            case "阶段-10":
                                Place10_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place10_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place10_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place10_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place10_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place10 = true;
                                Console.WriteLine("阶段-1=" + Place10_Low.ToString() + Place10_Up.ToString() + Place10_HuiWei.ToString() + Place10_ZuoGong.ToString() + Place10_PinLv.ToString());
                                break;

                            case "阶段-11":
                                Place11_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place11_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place11_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place11_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place11_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place11 = true;
                                Console.WriteLine("阶段-11=" + Place11_Low.ToString() + Place11_Up.ToString() + Place11_HuiWei.ToString() + Place11_ZuoGong.ToString() + Place11_PinLv.ToString());
                                break;

                            case "阶段-12":
                                Place12_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place12_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place12_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place12_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place12_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place12 = true;
                                Console.WriteLine("阶段-12=" + Place12_Low.ToString() + Place12_Up.ToString() + Place12_HuiWei.ToString() + Place12_ZuoGong.ToString() + Place12_PinLv.ToString());
                                break;

                            case "阶段-13":
                                Place13_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place13_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place13_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place13_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place13_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place13 = true;
                                Console.WriteLine("阶段-13=" + Place13_Low.ToString() + Place13_Up.ToString() + Place13_HuiWei.ToString() + Place13_ZuoGong.ToString() + Place13_PinLv.ToString());
                                break;

                            case "阶段-14":
                                Place14_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place14_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place14_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place14_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place14_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place14 = true;
                                Console.WriteLine("阶段-14=" + Place14_Low.ToString() + Place14_Up.ToString() + Place14_HuiWei.ToString() + Place14_ZuoGong.ToString() + Place14_PinLv.ToString());
                                break;

                            case "阶段-15":
                                Place15_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place15_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place15_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place15_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place15_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place15 = true;
                                Console.WriteLine("阶段-15=" + Place15_Low.ToString() + Place15_Up.ToString() + Place15_HuiWei.ToString() + Place15_ZuoGong.ToString() + Place15_PinLv.ToString());
                                break;

                            case "阶段-16":
                                Place16_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place16_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place16_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place16_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place16_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place16 = true;
                                Console.WriteLine("阶段-1=" + Place16_Low.ToString() + Place16_Up.ToString() + Place16_HuiWei.ToString() + Place16_ZuoGong.ToString() + Place16_PinLv.ToString());
                                break;

                            case "阶段-17":
                                Place17_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place17_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place17_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place17_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place17_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place17 = true;
                                Console.WriteLine("阶段-1=" + Place17_Low.ToString() + Place17_Up.ToString() + Place17_HuiWei.ToString() + Place17_ZuoGong.ToString() + Place17_PinLv.ToString());
                                break;

                            case "阶段-18":
                                Place18_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place18_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place18_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place18_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place18_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place18 = true;
                                Console.WriteLine("阶段-18=" + Place18_Low.ToString() + Place18_Up.ToString() + Place18_HuiWei.ToString() + Place18_ZuoGong.ToString() + Place18_PinLv.ToString());
                                break;

                            case "阶段-19":
                                Place19_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place19_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place19_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place19_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place19_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place19 = true;
                                Console.WriteLine("阶段-19=" + Place19_Low.ToString() + Place19_Up.ToString() + Place19_HuiWei.ToString() + Place19_ZuoGong.ToString() + Place19_PinLv.ToString());
                                break;

                            case "阶段-20":
                                Place20_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place20_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place20_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place20_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place20_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place20 = true;
                                Console.WriteLine("阶段-20=" + Place20_Low.ToString() + Place20_Up.ToString() + Place20_HuiWei.ToString() + Place20_ZuoGong.ToString() + Place20_PinLv.ToString());
                                break;

                            case "阶段-21":
                                Place21_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place21_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place21_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place21_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place21_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place21 = true;
                                Console.WriteLine("阶段-21=" + Place21_Low.ToString() + Place21_Up.ToString() + Place21_HuiWei.ToString() + Place21_ZuoGong.ToString() + Place21_PinLv.ToString());
                                break;

                            case "阶段-22":
                                Place22_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place22_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place22_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place22_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place22_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place22 = true;
                                Console.WriteLine("阶段-22=" + Place22_Low.ToString() + Place22_Up.ToString() + Place22_HuiWei.ToString() + Place22_ZuoGong.ToString() + Place22_PinLv.ToString());
                                break;

                            case "阶段-23":
                                Place23_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place23_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place23_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place23_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place23_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place23 = true;
                                Console.WriteLine("阶段-23=" + Place23_Low.ToString() + Place23_Up.ToString() + Place23_HuiWei.ToString() + Place23_ZuoGong.ToString() + Place23_PinLv.ToString());
                                break;
                            case "阶段-24":
                                Place24_Low = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[1].Value.ToString());
                                Place24_Up = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[2].Value.ToString());
                                Place24_HuiWei = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[3].Value.ToString());
                                Place24_ZuoGong = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[4].Value.ToString());
                                Place24_PinLv = Convert.ToUInt16((string)dataGridView_Param.Rows[i].Cells[5].Value.ToString());
                                Place24 = true;
                                Console.WriteLine("阶段-24=" + Place24_Low.ToString() + Place24_Up.ToString() + Place24_HuiWei.ToString() + Place24_ZuoGong.ToString() + Place24_PinLv.ToString());
                                break;
                            default:
                                break;

                            
                    }
                }
                catch //(Exception ex)
                {
                    MessageBox.Show("文档格式错误" + "（" + i.ToString() + "行）", "系统提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    ret = false;
                }
            }
            return ret;
        }//获取范围值存入判断

        private void button_manage_Click_1(object sender, EventArgs e)
        {
            ManageCoifig++;
            DeleteOption = false;
            if (ManageCoifig >= 1)
            {
                ProjectManageForm ProjectManage = new ProjectManageForm(this);
                ProjectManage.StartPosition = FormStartPosition.CenterParent;
                DialogResult dr = ProjectManage.ShowDialog();
                if (dr == System.Windows.Forms.DialogResult.OK)
                {
                    AddProject2Select();
                    dataGridView_Param.DataSource = null; //每次打开清空内容
                    OpenFileDialog ofd = new OpenFileDialog();
                    dataGridView_Param.DataSource = ReadFromExcel(PrjectCoifigFile);
                }
                else if (DeleteOption)
                {
                    AddProject2Select();
                    dataGridView_Param.DataSource = null; //每次打开清空内容
                    ProjectName = "";
                    MYSettings.Default.ProjectName = "";
                    MYSettings.Default.Save();
                }
                ManageCoifig = 0;
            }
        }

        /// <summary>
        /// 点击启动按键
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void start_Click(object sender, EventArgs e)
        {
            if (SNComboBox.Text == "")
            {
                MessageBox.Show("请输入产品编号");
            }
            if (SNComboBox.Text != "")
            {
                initivar(); //初始化变量和数组
                start_tick();               
                uint G = VCI_StartCAN(m_devtype, m_devind, m_canind);//为了验证CAN连接是否正常
                if (G == 0)
                {
                    MessageBox.Show("CAN通讯异常！请检查是否正确连接。");
                }
                else if (Can_Send(MYSettings.Default.start, MYSettings.Default.send_ID))
                {
                    text_lond.Items.Add("开始启动");
                    text_lond.SelectedIndex = text_lond.Items.Count - 1;
                }
            }
        }


        /// <summary>
        /// can进行数据发送
        /// </summary>
        /// <param name="str"></param>
        /// <param name="ID"></param>
        /// <returns></returns>
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

        /// <summary>
        /// 开启CAN 设备通讯
        /// </summary>
        public void start_tick()
        {        
            textms_lond(); // 开始测试UI界面显示状态
            LOND(CAN_time); //调用can进行通讯
            VCI_ClearBuffer(m_devtype, m_devind, m_canind);//设置CAN配置信息
            VCI_ResetCAN(4, 0, 0);
        }



        /// <summary>
        /// 测试弹窗显示界面
        /// </summary>
        public void textms_lond()
        {
            listBox1.Items.Clear();
            listBox1.Visible = false;
            listBox2.Items.Clear();
            listBox2.Visible = false;
            listBox3.Items.Clear();
            listBox3.Visible = false;

            text_lond.Items.Clear();
            leb_88.Text = "测试中....";
            leb_88.BackColor = Color.Green;
            leb_88.Visible = true;
            button_result1.Visible = false; //隐藏控件
            button_result2.Visible = false;
            button_result3.Visible = false;
           
            textBox1.BackColor = Color.Lime;  //温度显示
            textBox1.ForeColor = Color.Black;
            textBox1.Text = "";

            textBox_pres.BackColor = Color.Gold; //压力显示
            textBox_pres.ForeColor = Color.Black;
            textBox_pres.Text = "";

            textBox_electric.BackColor = Color.Green;//电流显示
            textBox_electric.ForeColor = Color.Black;
            textBox_electric.Text = "";
           
            textBox2.BackColor = Color.Coral; //电压显示
            textBox2.ForeColor = Color.Black;
            textBox2.Text = "";
            receive_T1Get = 0;           

        }
        /// <summary>
        /// 初始化变量
        /// </summary>
        public void initivar()
        {
            
            if (OPen_scale == 1)
            {
                Openserial = true; //称重标志位
            }
            Flag_time = 0;//之前测试周期清零
            
            //Flag_clear = 1; //电子秤清零标志位  
            start_scale =0; //开始测试标志位
            weight_sum = "";  
            weight_average = 0;
            weight_all = 0;
            for (int i = 0; i<24; i++)
            {
                weight[i] = 0;           //泵流量采样值
            }
            weight_n = 0;

            for (int i = 0; i < 30000; i++) 
            {
                electric_save[i] = 0;  //电流采样值 存入该数组中进行保存
                voltage_save[i] = 0;   //电压采样值 存入该数组中进行保存
                T_save[i] = 0;         //温度采样数据 
                P_save[i] = 0;         //压力采样数据
            }

            volt_average = 0;   //电压采样平均值
            elect_average = 0;  //电流采样平均值
            T_average = 0;      //温度平均采样值
            P_average = 0;      //压力平均采样值

            volt_sum = 0;   //电压采样总值
            elect_sum = 0;  //电流采样总值
            T_sum = 0;      //温度采样总值
            P_sum = 0;      //压力采样总值    

            overrun_I = 0;  //电流不合格次数
            overrun_P = 0;  //压力不合格次数
            overrun_T = 0;  //温度不合格次数
            overrun_V = 0;  //电压不合格次数
        }

        public void GKum()  //初始化称重
        {
            G1_1 = 0;
            G1_2 = 0;
            G1_3 = 0;
            G1_4 = 0;
            G1_5 = 0;
            G1_6 = 0;
            G1_7 = 0;
            G1_8 = 0;
            G1_9 = 0;
            G1_10 = 0;
            G1_11 = 0;
            G1_12 = 0;
            G1_13 = 0;
            G1_14 = 0;
            G1_15 = 0;
            G1_16 = 0;
            G1_17 = 0;
            G1_18 = 0;
            G1_19 = 0;
            G1_20 = 0;
            G1_21 = 0;
            G1_22 = 0;
            G1_23 = 0;
            
            progress_kk = 0; ///进度条显示进度变量
        }
        /// <summary>
        /// 对合格产品进行记录
        /// </summary>
        public void ok_progress_kk()//计数
        {
            MYSettings.Default.test_ok++;  //设置界面计数加1
            label_Qualified.Text = "合格：" + Convert.ToString(MYSettings.Default.test_ok);
        }
        /// <summary>
        /// 对不合格界面进行记录
        /// </summary>
        public void bad_progress_kk()
        {
            MYSettings.Default.test_bad++;
            this.Invoke((EventHandler)(delegate
            {
                label_UnQualified.Text = "不合格:" + Convert.ToString(MYSettings.Default.test_bad);
            }));
        }

        public void Can_Connect(System.Timers.Timer time)  //打开设备》设置参数》设备:USBCAN2 通道：0 索引：0 波特率250kbps;
        {
            if (m_bOpen == 1)
            {
                VCI_CloseDevice(m_devtype, m_devind);//关闭设备(设备类型，设备索引) 返回1成功 0失败 -1 设备不存在
                time.Enabled = false;              
                m_bOpen = 0;            
                button_openOrCloseCAN.Text = "连接CAN";            
            }
            else
            {
                m_devtype = 4;   //can类型
                m_devind = 0;   //CAN索引
                m_canind = 0;  //CAN通道
                if (VCI_OpenDevice(m_devtype, m_devind, 0) == 0)
                {
                    MessageBox.Show("打开设备失败,请检查设备类型和设备索引号是否正确", "错误",
                            MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                    return;
                }
                m_bOpen = 1;

                VCI_INIT_CONFIG coifig = new VCI_INIT_CONFIG();//结构体赋值
                coifig.AccCode = 0;
                coifig.AccMask = 4294967295;
                coifig.Timing0 = 0x01;
                coifig.Timing1 = 0x1C;
                coifig.Filter = 1;
                coifig.Mode = 0;
                VCI_InitCAN(m_devtype, m_devind, m_canind, ref coifig);

                //MessageBox.Show(System.Convert.ToString(m_devtype));
                if (System.Convert.ToBoolean(VCI_StartCAN(m_devtype, m_devind, m_canind)))//启动can的某一个通道（设备类型，设备索引，CAN通道索引）
                {
                    button_openOrCloseCAN.Text = "断开CAN";
                    
                    time.Enabled = true;
                }
                else { MessageBox.Show("CAN启动失败！"); }               
            }
        }

        /// <summary>
        /// CAN连接界面显示，如果成功显CAN连接，不成功显示连接失败；
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void can_init_Click(object sender, EventArgs e)
        {
            Can_Connect(CAN_time); //判断can是否连接成功
        }

        //点击停止按键
        private void stop_Click(object sender, EventArgs e)
        {
            Stop_trick();
            GKum();
        }

        public void Stop_trick() //按下停止后界面显示
        {
            leb_88.Text = "停止!";
            leb_88.BackColor = Color.Yellow;
            progressBar2.Visible = false;
            //VCI_ClearBuffer(m_devtype, m_devind, m_canind);
            //VCI_ResetCAN(4, 0, 0);
            Can_Send(MYSettings.Default.stop, MYSettings.Default.send_ID);
            text_lond.Items.Add("停止");
            text_lond.SelectedIndex = text_lond.Items.Count - 1;
            if (Can_Send(MYSettings.Default.stop, MYSettings.Default.send_ID))
            {
                text_lond.Items.Add("停止");
                text_lond.SelectedIndex = text_lond.Items.Count - 1;
            }
        }

        /// <summary>
        /// 接收到can信息  进行下位机之间的通讯
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        unsafe private void Receive_CAN(object sender, System.Timers.ElapsedEventArgs e)
        {
            
            UInt32 res = new UInt32();
            m_devtype = 4;   //can类型
            m_devind = 0;    //CAN索引
            m_canind = 0;    //CAN通道
            Byte[] Data_R = new Byte[8] { 0, 0, 0, 0, 0, 0, 0, 0 };
            res = VCI_Receive(m_devtype, m_devind, m_canind, ref m_recobj[0], 1000, 100);
            String str = "";          
            for (UInt32 i = 0; i < res; i++)
            {
                if (m_recobj[i].RemoteFlag == 0)
                {
                    byte mode = 0, function = 0;
                    uint ID = m_recobj[i].ID;
                    byte[] re_date = new byte[8];
                    VCI_CAN_OBJ sendobj = new VCI_CAN_OBJ();
                    sendobj.ExternFlag = 0;  //帧类型
                    sendobj.RemoteFlag = 0; //帧格式
                    byte len = (byte)(m_recobj[i].DataLen % 9);
                    byte j = 0;
                    fixed (VCI_CAN_OBJ* m_recobj1 = &m_recobj[i]) //对收到的CAN数据进行处理保存
                    {
                        if (j++ < len)
                            re_date[0] = m_recobj1->Data[0];             // 将0位传入Date_R[0]里面
                        str += " " + System.Convert.ToString(m_recobj1->Data[0], 16);
                        if (j++ < len)
                            re_date[1] = m_recobj1->Data[1];
                        str += " " + System.Convert.ToString(m_recobj1->Data[1], 16);
                        if (j++ < len)
                            re_date[2] = m_recobj1->Data[2];
                        str += " " + System.Convert.ToString(m_recobj1->Data[2], 16);
                        if (j++ < len)
                            re_date[3] = m_recobj1->Data[3];
                        str += " " + System.Convert.ToString(m_recobj1->Data[3], 16);
                        if (j++ < len)
                            re_date[4] = m_recobj1->Data[4];
                        str += " " + System.Convert.ToString(m_recobj1->Data[4], 16);
                        if (j++ < len)
                            re_date[5] = m_recobj1->Data[5];
                        str += " " + System.Convert.ToString(m_recobj1->Data[5], 16);
                        if (j++ < len)
                            re_date[6] = m_recobj1->Data[6];
                        str += " " + System.Convert.ToString(m_recobj1->Data[6], 16);
                        if (j++ < len)
                            re_date[7] = m_recobj1->Data[7];
                        str += " " + System.Convert.ToString(m_recobj1->Data[7], 16);
                    }
                    
                    mode = re_date[0];     //第一个数据位
                    function = re_date[1]; //第二个数据位
                    switch (ID)
                    {
                       case 0x10: 
                           {
                                if (mode == 0x10) //第一个数据位 data[0]
                                {
                                    if (function == 0x10) //// 为了可以进行按钮控制启动--暂时没有使用后期添加
                                    {
                                        uint G = VCI_StartCAN(4, 0, 0);
                                        if (G == 0)
                                        {
                                            MessageBox.Show("CAN通讯异常！请检查是否正确连接。");
                                        }
                                        else
                                        {
                                            start_tick();
                                            if (Can_Send(MYSettings.Default.start, MYSettings.Default.send_ID))
                                            {
                                                text_lond.Items.Add("开始启动");
                                                text_lond.SelectedIndex = text_lond.Items.Count - 1;
                                            }
                                            if (VCI_Transmit(4, 0, 0, ref sendobj, 1) == 0)
                                            {
                                                MessageBox.Show("发送失败", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                                            }
                                        }
                                    }

                                    if (function == 0x44) //发送泵的各项参数给下位机
                                    {
                                        //将设置的频率次数 通过can通讯传给下位机  67 两个数据位用来进行数据次数数据传输
                                        sendobj.ID = System.Convert.ToUInt32("0x" + MYSettings.Default.send_ID, 16);
                                        sendobj.DataLen = 8;
                                        sendobj.Data[0] = 0x40;
                                        sendobj.Data[1] = 0x12;
                                        sendobj.Data[2] = (byte)System.Convert.ToInt32("0x" + Convert.ToInt32(Place2_HuiWei).ToString("x2"), 16); //回位时间 
                                        sendobj.Data[3] = (byte)System.Convert.ToInt32("0x" + Convert.ToInt32(Place2_ZuoGong).ToString("x2"), 16); //做功时间 
                                        sendobj.Data[4] = (byte)System.Convert.ToInt32("0x" + Convert.ToInt32(Place2_PinLv).ToString("x2"), 16); //频率时间 
                                        sendobj.Data[5] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.Cycle, 10)).ToString("x2"), 16);     //喷射周期
                                        sendobj.Data[6] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.Spray_time, 10)).ToString("x4"), 16); //喷射次数
                                        sendobj.Data[7] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.Spray_time, 10)).ToString("x4").Substring(0, 2), 16);//喷射次数

                                        /*sendobj.Data[2] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.TIME_T1, 10)).ToString("x2"), 16);   //回位时间
                                        sendobj.Data[3] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.TIME_T2, 10)).ToString("x2"), 16);   //做工时间
                                        sendobj.Data[4] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.TIEM_T, 10)).ToString("x2"), 16);    //喷射一次时间
                                        sendobj.Data[5] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.Cycle, 10)).ToString("x2"), 16);     //喷射周期
                                        sendobj.Data[6] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.Spray_time, 10)).ToString("x4"), 16); //喷射次数
                                        sendobj.Data[7] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.Spray_time, 10)).ToString("x4").Substring(0, 2), 16);//喷射次数*/

                                        if (VCI_Transmit(4, 0, 0, ref sendobj, 1) == 0)
                                        {
                                            MessageBox.Show("发送失败", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                                        }
                                    }
                                    if (function == 0x65) //赋值
                                    {
                                        
                                        
                                                                                 
                                        sendobj.ID = System.Convert.ToUInt32("0x" + MYSettings.Default.send_ID, 16);
                                        sendobj.DataLen = 8;
                                        sendobj.Data[0] = 0x40;
                                        sendobj.Data[1] = 0x33;
                                        sendobj.Data[2] = (byte)System.Convert.ToInt32("0x" + Convert.ToInt32(HUIWEI[ii]).ToString("x2"), 16);
                                        sendobj.Data[3] = (byte)System.Convert.ToInt32("0x" + Convert.ToInt32(ZUOGONG[ii]).ToString("x2"), 16); //做功时间 
                                        sendobj.Data[4] = (byte)System.Convert.ToInt32("0x" + Convert.ToInt32(PINLV[ii]).ToString("x2"), 16); //频率时间 
                                        sendobj.Data[5] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.Cycle, 10)).ToString("x2"), 16);     //喷射周期
                                        sendobj.Data[6] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.Spray_time, 10)).ToString("x4"), 16); //喷射次数
                                        sendobj.Data[7] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.Spray_time, 10)).ToString("x4").Substring(0, 2), 16);//喷射次数
                                        ii++;
                                        /*sendobj.Data[2] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.TIME_T1, 10)).ToString("x2"), 16);   //回位时间
                                        sendobj.Data[2] = (byte)System.Convert.ToInt32("0x" + Convert.ToInt32(Place2_HuiWei).ToString("x2"), 16); //回位时间                                  
                                        sendobj.Data[3] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.TIME_T2, 10)).ToString("x2"), 16);   //做工时间
                                        sendobj.Data[4] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.TIEM_T, 10)).ToString("x2"), 16);    //喷射一次时间
                                        sendobj.Data[5] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.Cycle, 10)).ToString("x2"), 16);     //喷射周期
                                        sendobj.Data[6] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.Spray_time, 10)).ToString("x4"), 16); //喷射次数
                                        sendobj.Data[7] = (byte)System.Convert.ToUInt32("0x" + (Convert.ToInt32(MYSettings.Default.Spray_time, 10)).ToString("x4").Substring(0, 2), 16);//喷射次数*/

                                        if (VCI_Transmit(4, 0, 0, ref sendobj, 1) == 0)
                                        {
                                            MessageBox.Show("发送失败", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                                        }
                                    }

                                    if (function == 0x60) //程序初始化命令
                                    {
                                        count_num = 0;
                                        GKum();
                                        chart1.Series[0].Points.Clear();
                                        progressBar2.Visible = true;
                                        label7.Visible = true;
                                        sendobj.ID = System.Convert.ToUInt32("0x" + MYSettings.Default.send_ID, 16);
                                        sendobj.DataLen = 2;
                                        sendobj.Data[0] = 0x40;
                                        sendobj.Data[1] = 0x40;
                                        Auto_Test = true;
                                        if (VCI_Transmit(4, 0, 0, ref sendobj, 1) == 0)
                                        {
                                            MessageBox.Show("发送失败", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                                        }
                                    }

                                    
                                    if (function == 0x66) //程序初始化命令
                                    {

                                        sendobj.ID = System.Convert.ToUInt32("0x" + MYSettings.Default.send_ID, 16);
                                        sendobj.DataLen = 2;
                                        sendobj.Data[0] = 0x40;
                                        sendobj.Data[1] = 0x22;
                                        if (VCI_Transmit(4, 0, 0, ref sendobj, 1) == 0)
                                        {
                                            MessageBox.Show("发送失败", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                                        }
                                    }




                                    if (function == 0x61) //电压电流数据
                                    {
                                       
                                        count_num++;  //数组下标
                                        textBox2.Text = Convert.ToString(Convert.ToDouble((Convert.ToInt32(re_date[5] + re_date[4] * 256) * 0.038314 - 0.19748)).ToString("F2")) + "V";  //窗体显示供电电压
                                        if(Convert.ToInt32(re_date[2] * 256 + re_date[3]) <= 512)
                                        textBox_electric.Text = Convert.ToString(((2.5-(Convert.ToDouble(re_date[2]*256+re_date[3])*5 / 1024))/20.02*1000).ToString("F2")) + "mA";   //窗体显示喷嘴电流
                                        else
                                        textBox_electric.Text = Convert.ToString((((Convert.ToDouble(re_date[2] * 256 + re_date[3]) * 5 / 1024)-2.5) / 20.02*1000).ToString("F2")) + "mA";   //窗体显示喷嘴电流

                                        textBox_DianLiu.Text = Convert.ToString(Convert.ToInt32(re_date[2] * 256 + re_date[3]));  //标定显示电流十进制数据
                                        textBox_DianYa.Text = Convert.ToString(Convert.ToInt32(re_date[5] + re_date[4] * 256));   //窗体显示电压十进制数据

                                        voltage_save[count_num] = Math.Round(Convert.ToDouble(Convert.ToInt32(re_date[5] + re_date[4] * 256) * 0.038314 - 0.19748),2);   //供电电压
                                        if (Convert.ToInt32(re_date[2] * 256 + re_date[3]) <= 512)
                                            electric_save[count_num] = Math.Round((2.5 - Convert.ToDouble(re_date[2] * 256 + re_date[3]) * 5 / 1024) / 20.02 * 1000, 2); //电流
                                        else
                                            electric_save[count_num] = Math.Round(((Convert.ToDouble(re_date[2] * 256 + re_date[3]) * 5 / 1024) - 2.5) / 20.02*1000, 2); //电流
                                    }

                                    if (function == 0x51)  //接收上位机发送的数据
                                    {
                                        if (progress_kk == 1000)
                                        {
                                            progress_kk = 0;
                                            
                                        }
                                        progress_kk++;
                                        progressBar2.Value = progress_kk;  //显示进度条
                                        ///////////////**********温度显示条件*********/////////////                                                     
                                        int Temp_3 = Convert.ToInt32(re_date[3] + re_date[2] * 256);
                                        if (Temp_3 >= 592)
                                            textBox1.Text = "0 ℃";
                                            
                                        else if ((Temp_3 >= 536) && (Temp_3 < 592)) { textBox1.Text = "5 ℃"; Temp_record = 5; }
                                        else if ((Temp_3 >= 482) && (Temp_3 < 536)) { textBox1.Text = "10 ℃"; Temp_record = 10; }
                                        else if ((Temp_3 >= 429) && (Temp_3 < 482)) { textBox1.Text = "15 ℃"; Temp_record = 15; }
                                        else if ((Temp_3 >= 380) && (Temp_3 < 429)) { textBox1.Text = "20 ℃"; Temp_record = 20; }
                                        else if ((Temp_3 >= 334) && (Temp_3 < 380)) { textBox1.Text = "25 ℃"; Temp_record = 25; }
                                        else if ((Temp_3 >= 293) && (Temp_3 < 334)) { textBox1.Text = "30 ℃"; Temp_record = 30; }
                                        else if ((Temp_3 >= 256) && (Temp_3 < 293)) { textBox1.Text = "35 ℃"; Temp_record = 35; }
                                        else if ((Temp_3 >= 224) && (Temp_3 < 256)) { textBox1.Text = "40 ℃"; Temp_record = 40; }
                                        else if ((Temp_3 >= 194) && (Temp_3 < 224)) { textBox1.Text = "45 ℃"; Temp_record = 45; }
                                        else if ((Temp_3 >= 169) && (Temp_3 < 194)) { textBox1.Text = "50 ℃"; Temp_record = 50; }
                                        else if ((Temp_3 >= 147) && (Temp_3 < 169)) { textBox1.Text = "55 ℃"; Temp_record = 55; }
                                        else if ((Temp_3 >= 127) && (Temp_3 < 147)) { textBox1.Text = "60 ℃"; Temp_record = 60; }
                                        else if ((Temp_3 >= 111) && (Temp_3 < 127)) { textBox1.Text = "65 ℃"; Temp_record = 65; }
                                        else if ((Temp_3 >= 97) && (Temp_3 < 111)) { textBox1.Text = "70 ℃"; Temp_record = 70; }
                                        else if ((Temp_3 >= 85) && (Temp_3 < 97)) { textBox1.Text = "75 ℃"; Temp_record = 75; }
                                        else if ((Temp_3 >= 74) && (Temp_3 < 85)) { textBox1.Text = "80 ℃"; Temp_record = 80; }
                                        else if ((Temp_3 >= 65) && (Temp_3 < 74)) { textBox1.Text = "85 ℃"; Temp_record = 85; }
                                        else if ((Temp_3 >= 56) && (Temp_3 < 65)) { textBox1.Text = "90 ℃"; Temp_record = 90; }
                                        else if ((Temp_3 >= 50) && (Temp_3 < 56)) { textBox1.Text = "95 ℃"; Temp_record = 95; }
                                        else if ((Temp_3 >= 45) && (Temp_3 < 50)) { textBox1.Text = "100 ℃"; Temp_record = 100; }
                                        else
                                        {
                                            Stop_trick();
                                            MessageBox.Show("温度过高请停止使用练习管理员进行维修", "警告");
                                            System.Windows.Forms.Application.Exit();
                                        }
                                        textBox_pres.Text = Convert.ToString((Convert.ToDouble(re_date[4] * 256 + re_date[5]) * 0.005 * 0.750 - 0.125).ToString("F2")) + "MPa";   //窗体显示压力
                                        textBox_WenDu.Text = Convert.ToString(Convert.ToInt32(re_date[3] + re_date[2] * 256));    //显示温度采样值
                                        textBox_YaLi.Text = Convert.ToString(Convert.ToInt32(re_date[5] + re_date[4] * 256));     //显示压力采样值
                                        T_save[count_num] += Convert.ToInt32(Temp_record);   //温度电阻值采样
                                        P_save[count_num] += Math.Round((Convert.ToDouble(re_date[4] * 256 + re_date[5]) * 0.005 * 0.750 - 0.125),2);  //获取压力值 

                                        
                                    }
                                    if (function == 0x62)////测试结束停止指令
                                    {
                                        start_scale = 1;
                                        Flag_time++;
                                        if (Flag_time >= System.Convert.ToInt32(MYSettings.Default.Cycle, 10))
                                        {
                                            Flag_time = 0;
                                            sendobj.ID = System.Convert.ToUInt32("0x" + MYSettings.Default.send_ID, 16);
                                            sendobj.DataLen = 2;
                                            sendobj.Data[0] = 0x40;
                                            sendobj.Data[1] = 0x99;
                                            GKum();
                                            if (Openserial) //最后一次称重
                                            {
                                                getWeightOnScale();
                                                Openserial = false;
                                            }
                                            if (VCI_Transmit(4, 0, 0, ref sendobj, 1) == 0)
                                            {
                                                MessageBox.Show("发送失败", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                                            }
                                        }
                                        else
                                        {
                                            sendobj.ID = System.Convert.ToUInt32("0x" + MYSettings.Default.send_ID, 16);
                                            sendobj.DataLen = 2;
                                            sendobj.Data[0] = 0x40;
                                            sendobj.Data[1] = 0x40;
                                            if (VCI_Transmit(4, 0, 0, ref sendobj, 1) == 0)
                                            {
                                                MessageBox.Show("发送失败", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                                            }
                                        }
                                    }

                                    if (function == 0x63)  //发送开始喷射的指令
                                    {
                                        if (start_scale == 1)
                                        {
                                            if (Openserial)
                                                getWeightOnScale();
                                        }
                                        if ((Flag_clear == 1) && (start_scale == 1))
                                        {
                                            MessageBox.Show("请点击电子秤清零按钮在进行下一步操作");
                                            Flag_clear = 0;
                                        }
                                        sendobj.ID = System.Convert.ToUInt32("0x" + MYSettings.Default.send_ID, 16);
                                        sendobj.DataLen = 2;
                                        sendobj.Data[0] = 0x40;
                                        sendobj.Data[1] = 0x88;
                                        if (VCI_Transmit(4, 0, 0, ref sendobj, 1) == 0)
                                        {
                                            MessageBox.Show("发送失败", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                                        }
                                    }

                                    if (function == 0x64) //发送接收停止命令
                                    {
                                        ii = 2;
                                        progressBar2.Visible = true;
                                        label7.Visible = true;
                                        chart1.Series[0].Points.Clear();
                                        text_lond.Items.Add("测试完成");
                                        sendobj.ID = System.Convert.ToUInt32("0x" + MYSettings.Default.send_ID, 16);
                                        sendobj.DataLen = 2;
                                        sendobj.Data[0] = 0x40;
                                        sendobj.Data[1] = 0x55;
                                        if (VCI_Transmit(4, 0, 0, ref sendobj, 1) == 0)
                                        {
                                            MessageBox.Show("发送失败", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                                        }
                                        leb_88.Visible = false;  //测试中字样                                     
                                        label7.Visible = false;
                                        progressBar2.Visible = false;

                                        ///////////////////////////电流、电压遍历判断数据是否合格，并记录不合格的次数//////////////////////////
                                        for (i = 0; i <=  (Convert.ToInt32(MYSettings.Default.Spray_time, 10) * Convert.ToInt32(MYSettings.Default.Cycle,10)); i++)
                                        {
                                            if ((electric_save[i] != 0) || (voltage_save[i] != 0))  //由于测试数据的限制 出现数据为0的值不进行保存
                                            {
                                                if ((I_Low > electric_save[i]) || (electric_save[i] > I_Up))  //对电流超过限制值的数据进行记录                                              
                                                    overrun_I++;      //电流值超过限制值的次数

                                                if ((V_Low > voltage_save[i]) || (voltage_save[i] > V_Up))    //对电压超过限制的数据进行记录                                               
                                                    overrun_V++;     //电压值超过限制值的次数

                                                if ((T_Low > T_save[i]) || (T_Up < T_save[i]))   //对温度超限的值进行记录
                                                    overrun_T++;     //温度值超过限制值的次数

                                                if ((P_Low > P_save[i]) || (P_Up < P_save[i]))   //对压力超限的值进行记录
                                                    overrun_P++;     //压力超过限制值的次数                                                
                                            }

                                            elect_sum += electric_save[i];
                                            volt_sum += voltage_save[i];
                                            T_sum += T_save[i];
                                            P_sum += P_save[i];
                                        }
                                        for (i = 0; i <= Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_1 += electric_save[i];
                                            volt_sum_1 += voltage_save[i];
                                        }
                                        for ( i = (uint)Convert.ToInt32(MYSettings.Default.Spray_time, 10); i <= 2*Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_2 += electric_save[i];
                                            volt_sum_2 += voltage_save[i];
                                        }
                                        for ( i = (uint)(2*Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 3 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_3 += electric_save[i];
                                            volt_sum_3 += voltage_save[i];
                                        }
                                        for (i = (uint)(3 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 4 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_4 += electric_save[i];
                                            volt_sum_4 += voltage_save[i];
                                        }
                                        for (i = (uint)(4 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 5 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_5 += electric_save[i];
                                            volt_sum_5 += voltage_save[i];
                                        }
                                        for (i = (uint)(5 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 6 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_6 += electric_save[i];
                                            volt_sum_6 += voltage_save[i];
                                        }
                                        for (i = (uint)(6 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 7 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_7 += electric_save[i];
                                            volt_sum_7 += voltage_save[i];
                                        }
                                        for (i = (uint)(7 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 8 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_8 += electric_save[i];
                                            volt_sum_8 += voltage_save[i];
                                        }
                                        for (i = (uint)(8 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 9 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_9 += electric_save[i];
                                            volt_sum_9 += voltage_save[i];
                                        }
                                        for (i = (uint)(9 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 10 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_10 += electric_save[i];
                                            volt_sum_10 += voltage_save[i];
                                        }
                                        for (i = (uint)(10 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 11 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_11 += electric_save[i];
                                            volt_sum_11 += voltage_save[i];
                                        }
                                        for (i = (uint)(11 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 12 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_12 += electric_save[i];
                                            volt_sum_12 += voltage_save[i];
                                        }
                                        for (i = (uint)(12 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 13 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_13 += electric_save[i];
                                            volt_sum_13 += voltage_save[i];
                                        }
                                        for (i = (uint)(13 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 14 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_14 += electric_save[i];
                                            volt_sum_14 += voltage_save[i];
                                        }
                                        for (i = (uint)(14 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 15 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_15 += electric_save[i];
                                            volt_sum_15 += voltage_save[i];
                                        }
                                        for (i = (uint)(15 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 16 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_16 += electric_save[i];
                                            volt_sum_16 += voltage_save[i];
                                        }
                                        for (i = (uint)(16 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 17 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_17 += electric_save[i];
                                            volt_sum_17 += voltage_save[i];
                                        }
                                        for (i = (uint)(17 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 18 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_18 += electric_save[i];
                                            volt_sum_18 += voltage_save[i];
                                        }
                                        for (i = (uint)(18 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 19 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_19 += electric_save[i];
                                            volt_sum_19 += voltage_save[i];
                                        }
                                        for (i = (uint)(19 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 20 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_20 += electric_save[i];
                                            volt_sum_20 += voltage_save[i];
                                        }
                                        for (i = (uint)(20 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 21 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_21 += electric_save[i];
                                            volt_sum_21 += voltage_save[i];
                                        }
                                        for (i = (uint)(21 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 22 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_22 += electric_save[i];
                                            volt_sum_22 += voltage_save[i];
                                        }
                                        for (i = (uint)(22 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 23 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_23 += electric_save[i];
                                            volt_sum_23 += voltage_save[i];
                                        }
                                        for (i = (uint)(23 * Convert.ToInt32(MYSettings.Default.Spray_time, 10)); i <= 24 * Convert.ToInt32(MYSettings.Default.Spray_time, 10); i++)
                                        {
                                            elect_sum_24 += electric_save[i];
                                            volt_sum_24 += voltage_save[i];
                                        }

                                        ///////////////////////计算数据的平均值填入表格中进行存储///////////////////////////                            
                                        volt_average = Math.Round(volt_sum / (Convert.ToInt32(MYSettings.Default.Cycle, 10) * Convert.ToInt32(MYSettings.Default.Spray_time, 10)),2);  //对采集的电压求平均值
                                        elect_average = Math.Round(elect_sum / (Convert.ToInt32(MYSettings.Default.Cycle, 10) * Convert.ToInt32(MYSettings.Default.Spray_time, 10)),2); //电流平均值
                                        T_average = Math.Round(T_sum / (Convert.ToInt32(MYSettings.Default.Cycle, 10) * Convert.ToInt32(MYSettings.Default.Spray_time, 10)),2);      //温度平均值
                                        P_average = Math.Round(P_sum / (Convert.ToInt32(MYSettings.Default.Cycle, 10) * Convert.ToInt32(MYSettings.Default.Spray_time, 10)),2);      //压力平均值

                                        volt_average_1 = Math.Round(volt_sum_1 / 1000,2);
                                        volt_average_2 = Math.Round(volt_sum_2 / 1000, 2);
                                        volt_average_3 = Math.Round(volt_sum_3 / 1000, 2);
                                        volt_average_4 = Math.Round(volt_sum_4 / 1000, 2);
                                        volt_average_5 = Math.Round(volt_sum_5 / 1000, 2);
                                        volt_average_6 = Math.Round(volt_sum_6 / 1000, 2);
                                        volt_average_7 = Math.Round(volt_sum_7 / 1000, 2);
                                        volt_average_8 = Math.Round(volt_sum_8 / 1000, 2);
                                        volt_average_9 = Math.Round(volt_sum_9 / 1000, 2);
                                        volt_average_10 = Math.Round(volt_sum_10 / 1000, 2);
                                        volt_average_11 = Math.Round(volt_sum_11 / 1000, 2);
                                        volt_average_12 = Math.Round(volt_sum_12 / 1000, 2);
                                        volt_average_13 = Math.Round(volt_sum_13 / 1000, 2);
                                        volt_average_14 = Math.Round(volt_sum_14 / 1000, 2);
                                        volt_average_15 = Math.Round(volt_sum_15 / 1000, 2);
                                        volt_average_16 = Math.Round(volt_sum_16 / 1000, 2);
                                        volt_average_17 = Math.Round(volt_sum_17 / 1000, 2);
                                        volt_average_18 = Math.Round(volt_sum_18 / 1000, 2);
                                        volt_average_19 = Math.Round(volt_sum_19 / 1000, 2);
                                        volt_average_20 = Math.Round(volt_sum_20 / 1000, 2);
                                        volt_average_21 = Math.Round(volt_sum_21 / 1000, 2);
                                        volt_average_22 = Math.Round(volt_sum_22 / 1000, 2);
                                        volt_average_23 = Math.Round(volt_sum_23 / 1000, 2);
                                        volt_average_24 = Math.Round(volt_sum_24 / 1000, 2);

                                        volt_all =    "阶段-1:" + volt_average_1.ToString() + "阶段-2:" + volt_average_2.ToString() + "阶段-3:" + volt_average_3.ToString() + "阶段-4:" + volt_average_4.ToString() + "阶段-5:" + volt_average_5.ToString() + "阶段-6:" + volt_average_6.ToString() +
                                                      "阶段-7:" + volt_average_7.ToString() + "阶段-8:" + volt_average_8.ToString() + "阶段-9:" + volt_average_9.ToString() + "阶段-10:" + volt_average_10.ToString() + "阶段-11:" + volt_average_11.ToString() + "阶段-12:" + volt_average_12.ToString() +
                                                      "阶段-13:" + volt_average_13.ToString() + "阶段-14:" + volt_average_14.ToString() + "阶段-15:" + volt_average_15.ToString() + "阶段-16:" + volt_average_16.ToString() + "阶段-17:" + volt_average_17.ToString() + "阶段-18:" + volt_average_18.ToString() +
                                                      "阶段-19:" + volt_average_19.ToString() + "阶段-20:" + volt_average_20.ToString() + "阶段-21:" + volt_average_21.ToString() + "阶段-22:" + volt_average_22.ToString() + "阶段-23:" + volt_average_23.ToString() + "阶段-24:" + volt_average_24.ToString();

                                        elect_average_1 = Math.Round(elect_sum_1 / 1000,2);
                                        elect_average_2 = Math.Round(elect_sum_2 / 1000,2);
                                        elect_average_3 = Math.Round(elect_sum_3 / 1000, 2);
                                        elect_average_4 = Math.Round(elect_sum_4 / 1000, 2);
                                        elect_average_5 = Math.Round(elect_sum_5 / 1000, 2);
                                        elect_average_6 = Math.Round(elect_sum_6 / 1000, 2);
                                        elect_average_7 = Math.Round(elect_sum_7 / 1000, 2);
                                        elect_average_8 = Math.Round(elect_sum_8 / 1000, 2);
                                        elect_average_9 = Math.Round(elect_sum_9 / 1000, 2);
                                        elect_average_10 = Math.Round(elect_sum_10 / 1000, 2);
                                        elect_average_11 = Math.Round(elect_sum_11 / 1000, 2);
                                        elect_average_12 = Math.Round(elect_sum_12 / 1000, 2);
                                        elect_average_13 = Math.Round(elect_sum_13 / 1000, 2);
                                        elect_average_14 = Math.Round(elect_sum_14 / 1000, 2);
                                        elect_average_15 = Math.Round(elect_sum_15 / 1000, 2);
                                        elect_average_16 = Math.Round(elect_sum_16 / 1000, 2);
                                        elect_average_17 = Math.Round(elect_sum_17 / 1000, 2);
                                        elect_average_18 = Math.Round(elect_sum_18 / 1000, 2);
                                        elect_average_19 = Math.Round(elect_sum_19 / 1000, 2);
                                        elect_average_20 = Math.Round(elect_sum_20 / 1000, 2);
                                        elect_average_21 = Math.Round(elect_sum_21 / 1000, 2);
                                        elect_average_22 = Math.Round(elect_sum_22 / 1000, 2);
                                        elect_average_23 = Math.Round(elect_sum_23 / 1000, 2);
                                        elect_average_24 = Math.Round(elect_sum_24 / 1000, 2);

                                        electric_all = "阶段-1:" + elect_average_1.ToString() + "阶段-2:" + elect_average_2.ToString() + "阶段-3:" + elect_average_3.ToString() + "阶段-4:" + elect_average_4.ToString() + "阶段-5:" + elect_average_5.ToString() + "阶段-6:" + elect_average_6.ToString() +
                                                       "阶段-7:" + elect_average_7.ToString() + "阶段-8:" + elect_average_8.ToString() + "阶段-9:" + elect_average_9.ToString() + "阶段-10:" + elect_average_10.ToString() + "阶段-11:" + elect_average_11.ToString() + "阶段-12:" + elect_average_12.ToString() +
                                                       "阶段-13:" + elect_average_13.ToString() + "阶段-14:" + elect_average_14.ToString() + "阶段-15:" + elect_average_15.ToString() + "阶段-16:" + elect_average_16.ToString() + "阶段-17:" + elect_average_17.ToString() + "阶段-18:" + elect_average_18.ToString() +
                                                       "阶段-19:" + elect_average_19.ToString() + "阶段-20:" + elect_average_20.ToString() + "阶段-21:" + elect_average_21.ToString() + "阶段-22:" + elect_average_22.ToString() + "阶段-23:" + elect_average_23.ToString() + "阶段-24:" + elect_average_24.ToString();

                                        /////////////////////////////流量数据判断遍历每段数据//////////////////////                                             
                                        for (weight_n = 1; weight_n < 24; weight_n++)
                                        {
                                            chart1.Series[0].Points.AddXY(weight_n, weight[weight_n]);//添加曲线集合   绘制流量采集图形 
                                            weight_sum += weight[weight_n] + ","; //记录重量采样的数据
                                            weight_all += weight[weight_n];      //记录重量数据的总的采样值
                                        }
                                        weight_average = weight_all / 23;
                                        //判断测试流量是否超过标准值
                                        if ((Place1_Low > weight[1]) || (weight[1] > Place1_Up)) { G1_1++; }
                                        if ((Place2_Low > weight[2]) || (weight[2] > Place2_Up)) { G1_2++; }
                                        if ((Place3_Low > weight[3]) || (weight[3] > Place3_Up)) { G1_3++; }
                                        if ((Place4_Low > weight[4]) || (weight[4] > Place4_Up)) { G1_4++; }
                                        if ((Place5_Low > weight[5]) || (weight[5] > Place5_Up)) { G1_5++; }
                                        if ((Place6_Low > weight[6]) || (weight[6] > Place6_Up)) { G1_6++; }
                                        if ((Place7_Low > weight[7]) || (weight[7] > Place7_Up)) { G1_7++; }
                                        if ((Place8_Low > weight[8]) || (weight[8] > Place8_Up)) { G1_8++; }
                                        if ((Place9_Low > weight[9]) || (weight[9] > Place9_Up)) { G1_9++; }
                                        if ((Place10_Low > weight[10]) || (weight[10] > Place10_Up)) { G1_10++; }
                                        if ((Place11_Low > weight[11]) || (weight[11] > Place11_Up)) { G1_11++; }
                                        if ((Place12_Low > weight[12]) || (weight[12] > Place12_Up)) { G1_12++; }
                                        if ((Place13_Low > weight[13]) || (weight[13] > Place13_Up)) { G1_13++; }
                                        if ((Place14_Low > weight[14]) || (weight[14] > Place14_Up)) { G1_14++; }
                                        if ((Place15_Low > weight[15]) || (weight[15] > Place15_Up)) { G1_15++; }
                                        if ((Place16_Low > weight[16]) || (weight[16] > Place16_Up)) { G1_16++; }
                                        if ((Place17_Low > weight[17]) || (weight[17] > Place17_Up)) { G1_17++; }
                                        if ((Place18_Low > weight[18]) || (weight[18] > Place18_Up)) { G1_18++; }
                                        if ((Place19_Low > weight[19]) || (weight[19] > Place19_Up)) { G1_19++; }
                                        if ((Place20_Low > weight[20]) || (weight[20] > Place20_Up)) { G1_20++; }
                                        if ((Place21_Low > weight[21]) || (weight[21] > Place21_Up)) { G1_21++; }
                                        if ((Place22_Low > weight[22]) || (weight[22] > Place22_Up)) { G1_22++; }
                                        if ((Place23_Low > weight[23]) || (weight[23] > Place23_Up)) { G1_23++; }
                                        //流量数据判别是否合格
                                        if (G1_ParamCheck()) //判断流量合格的构造函数
                                        {
                                            ok_progress_kk();
                                            button_result2.BackColor = Color.Green;
                                            button_result2.Text = "流量合格";
                                            button_result2.Visible = true;
                                            string strsuccful_1 = comboBox_Project.Text.ToString() + "," + SNComboBox.Text.ToString() + "," + DateTime.Now.Date.ToShortDateString() + "-" + DateTime.Now.Hour.ToString("00") + ":" +
                                            DateTime.Now.Minute.ToString("00") + ":" + DateTime.Now.Second.ToString("00") + "," + "流量合格" + ",";
                                            writedata(strsuccful_1); //写数据 1
                                        }
                                        else
                                        {
                                            bad_progress_kk();  //不合格次数加 窗体显示不合格次数加1
                                            button_result2.BackColor = Color.Red;
                                            button_result2.Text = "流量不合格";
                                            button_result2.Visible = false;
                                            string strsuccful_1 = comboBox_Project.Text.ToString() + "," + SNComboBox.Text.ToString() + "," + DateTime.Now.Date.ToShortDateString() + "-" + DateTime.Now.Hour.ToString("00") + ":" +
                                            DateTime.Now.Minute.ToString("00") + ":" + DateTime.Now.Second.ToString("00") + "," + "流量不合格" + ",";
                                            writedata(strsuccful_1); //写数据 1
                                        }

                                        ////////////温度压力测试是否合格////////////////////
                                        //如果超限次数大于20次判为不合格
                                        if ((overrun_T < 5) && (overrun_P < 5)) //判断电流、电压合格的构造函数
                                        {
                                            //ok_progress_kk();
                                            button_result3.BackColor = Color.Green;
                                            button_result3.Text = "温度压力合格";
                                            button_result3.Visible = true;
                                            listBox3.Visible = false;
                                            string strsuccful_3 = "温度压力合格" + ",";
                                            writedata(strsuccful_3); //写数据 2
                                        }
                                        else
                                        {
                                            //bad_progress_kk();
                                            button_result3.BackColor = Color.Red;
                                            button_result3.Text = "温度压力不合格";
                                            button_result3.Visible = true;
                                            listBox3.Visible = false;
                                            string strsuccful_3 = "温度压力不合格" + ",";
                                            writedata(strsuccful_3);//写数据 2
                                        }
                                        //如果电压 电流数据中超过限制值5次 判为不合格
                                        if ((overrun_I < 5) && (overrun_V < 5)) //判断电流、电压合格的构造函数
                                        {
                                            //ok_progress_kk();
                                            button_result1.BackColor = Color.Green;
                                            button_result1.Text = "电流电压合格";
                                            button_result1.Visible = true;
                                            string strsuccful_2 = "电流电压合格" + "," + electric_all + "," + volt_all + "," + overrun_I + "," + overrun_V + "," + T_average + "," + P_average + "," + weight_average + "," + weight_sum + "\r\n";
                                            writedata(strsuccful_2); //写数据 3
                                        }
                                        else
                                        {
                                            //bad_progress_kk();
                                            button_result1.BackColor = Color.Red;
                                            button_result1.Text = "电流电压不合格";
                                            button_result1.Visible = true;
                                            string strsuccful_2 = "电流电压不合格" + "," + electric_all + "," + volt_all + "," + overrun_I + "," + overrun_V + "," + T_average + "," + P_average + "," + weight_average + "," + weight_sum + "\r\n";
                                            writedata(strsuccful_2);//写数据 3
                                        }
                                    }// if (function == 0x64)

                                    if (function == 0x88)  //后期开发加入停止按钮的协议配置
                                    {
                                        Stop_trick();
                                        if (VCI_Transmit(4, 0, 0, ref sendobj, 1) == 0)
                                        {
                                            MessageBox.Show("发送失败", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                                        }
                                    }
                                }//if (mode == 0x10)
                                break;
                           }//case 0x10:
                    }
                }//if (m_recobj[i].RemoteFlag == 0)
            }//for
        }//CAN接收数据

        /// <summary>
        /// 对采集到的重量进行判断 显示才文本框中
        /// </summary>
        /// <returns></returns>
        private bool G1_ParamCheck()
        {
            bool ret = true;
            byte ret_v = 0;                
            if (Place1) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_1, "1段"); }
            if (Place2) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_2, "2段"); }
            if (Place3) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_3, "3段"); }
            if (Place4) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_4, "4段"); }
            if (Place5) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_5, "5段"); }
            if (Place6) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_6, "6段"); }
            if (Place7) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_7, "7段"); }
            if (Place8) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_8, "8段"); }
            if (Place9) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_9, "9段"); }
            if (Place10) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_10, "10段"); }
            if (Place11) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_11, "11段"); }
            if (Place12) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_12, "12段"); }
            if (Place13) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_13, "13段"); }
            if (Place14) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_14, "14段"); }
            if (Place15) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_15, "15段"); }
            if (Place16) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_16, "16段"); }
            if (Place17) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_17, "17段"); }
            if (Place18) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_18, "18段"); }
            if (Place19) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_19, "19段"); }
            if (Place20) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_20, "20段"); }
            if (Place21) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_21, "21段"); }
            if (Place22) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_22, "22段"); }
            if (Place23) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_23, "23段"); }
            if (Place24) { ret_v |= TextBoxCheckLimitShow(listBox1, G1_23, "24段"); }
            if (ret_v != 0)
            {
                ret = false;
            }
            return ret;
        }
        /// <summary>
        /// 对电流、电压采样数据进行判断
        /// </summary>
        /// <returns></returns>
        private bool G2_ParamCheck()
        {
            bool ret = true;
            byte ret_v = 0;
            if (Place_V) { ret_v |= TextBoxCheckLimitShow_2(listBox2, overrun_V, "电压"); } //电压合格判断
            if (Place_I) { ret_v |= TextBoxCheckLimitShow_2(listBox2, overrun_I, "电流"); } //电流合格判断

            if (Place_T) { ret_v |= T_Show(textBox1, overrun_T, listBox3); } //温度合格判断
            if (Place_P) { ret_v |= P_Show(textBox_pres, overrun_P, listBox3); } //压力合格判断

            if (ret_v != 0)
            {
                ret = false;
            }
            return ret;
        }
        /// <summary>
        /// 判断流量是否合格的构造函数
        /// </summary>
        /// <param name="Tbx"></param>
        /// <param name="Value"></param>
        /// <param name="Place_BUZZ"></param>
        /// <returns></returns>
        private byte TextBoxCheckLimitShow(System.Windows.Forms.ListBox Tbx, uint Value, String Place_BUZZ)  // 流量限值检测
        {
            byte ret = 0;
            if (Value!=0)
            {
                Tbx.BackColor = Color.Red;
                Tbx.ForeColor = Color.White;
                this.Invoke((EventHandler)(delegate
                {
                    Tbx.Visible = true;
                    Tbx.Items.Add(Place_BUZZ + "异常");
                    Tbx.SelectedIndex = Tbx.Items.Count - 1;

                }));
                ret = 1;
            }
            else
            {
                ret = 0;
            }
            return ret;
        }
        /// <summary>
        /// 电流、电压窗体显示调用函数
        /// </summary>
        /// <param name="Tbx"></param>
        /// <param name="Value"></param>
        /// <param name="Place_BUZZ"></param>
        /// <returns></returns>
        private byte TextBoxCheckLimitShow_2(System.Windows.Forms.ListBox Tbx, uint Value, String Place_BUZZ)  //电流、电压限值检测
        {
            byte ret = 0;
            if (Value >20)
            {
                Tbx.BackColor = Color.Red;
                Tbx.ForeColor = Color.White;
                this.Invoke((EventHandler)(delegate
                {
                    Tbx.Visible = true;
                    Tbx.Items.Add(Place_BUZZ + "异常、");
                    Tbx.SelectedIndex = Tbx.Items.Count - 1;
                }));
                ret = 1;
            }
            else
            {
                ret = 0;
            }
            return ret;
        }
        /// <summary>
        /// 判断温度是否合格的构造函数
        /// </summary>
        /// <param name="Tbx"></param>
        /// <param name="Value"></param>
        /// <param name="Lbx"></param>
        /// <returns></returns>
        private byte T_Show(System.Windows.Forms.TextBox Tbx, uint Value, System.Windows.Forms.ListBox Lbx)  //温度检测
        {
            byte ret = 0;
            if (Value>20)
            {
                Tbx.BackColor = Color.Red;
                Tbx.ForeColor = Color.White;
                this.Invoke((EventHandler)(delegate
                {
                    Lbx.Visible = true;
                    Lbx.Items.Add("温度异常、");
                    Lbx.SelectedIndex = Lbx.Items.Count - 1;
                }));
                ret = 1;
            }
            else
            {
                Tbx.BackColor = Color.Lime;
                Tbx.ForeColor = Color.Black;
                ret = 0;
            }        
            return ret;
        }
        /// <summary>
        /// 判断压力是否合格的构造函数
        /// </summary>
        /// <param name="Tbx"></param>
        /// <param name="Value"></param>
        /// <param name="Lbx"></param>
        /// <returns></returns>
        private byte P_Show(System.Windows.Forms.TextBox Tbx, uint Value, System.Windows.Forms.ListBox Lbx)  //压力检测
        {
            byte ret = 0;
            if (Value>20)
            {
                Tbx.BackColor = Color.Red;
                Tbx.ForeColor = Color.White;
                this.Invoke((EventHandler)(delegate
                {
                    Lbx.Visible = true;
                    Lbx.Items.Add("压力异常、");
                    Lbx.SelectedIndex = Lbx.Items.Count - 1;
                }));
                ret = 1;
            }
            else
            {
                Tbx.BackColor = Color.Lime;
                Tbx.ForeColor = Color.Black;
                ret = 0;
            }
            return ret;
        }
        /// <summary>
        /// 选择更换测试项目的调用函数
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void comboBox_Project_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.Cursor = Cursors.WaitCursor;
            string L_ProjectName;
            string text;
            string ProjCoifig;
            string fname = System.Windows.Forms.Application.StartupPath + @"\DCUProject.proj";
            System.IO.StreamReader file = new StreamReader(fname);
            if (File.Exists(fname))
            {
                while (true)
                {
                    text = file.ReadLine();
                    if (text != null)
                    {
                        if (text.Equals("")) continue;//终止本次循环
                        if (text.Substring(0, 1).Equals(":"))//头格式正确
                        {
                            L_ProjectName = text.Replace(":", "");
                            if (comboBox_Project.Text.Equals(L_ProjectName))
                            {
                                ProjectName = L_ProjectName;
                                text = file.ReadLine();
                                ProjCoifig = System.Windows.Forms.Application.StartupPath + text.Replace("|", "");
                                if (!File.Exists(ProjCoifig))
                                {
                                    MessageBox.Show("不正确的工程配置文件路径", "错误", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
                                }
                                else
                                {
                                    Plice_False();
                                    MYSettings.Default.ProjectName = ProjectName;
                                    MYSettings.Default.Save();
                                    PrjectCoifigFile = ProjCoifig;
                                    dataGridView_Param.DataSource = null;//清空
                                    OpenFileDialog ofd = new OpenFileDialog();//打开文件
                                    dataGridView_Param.DataSource = ReadFromExcel(PrjectCoifigFile);
                                    if (ReadParamFile())//存入值
                                    {
                                       // MessageBox.Show("参数读取成功");
                                    }
                                    Get_T = false;
                                    Auto_Test = false;
                                }
                            }
                        }
                    }
                    else
                    {
                        break;//跳出循环
                    }
                }
            }
            file.Close();
            this.Cursor = Cursors.Default;
        }
        /// <summary>
        /// 关闭主窗口 关闭程序系统
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form_Main_FormClosed(object sender, FormClosedEventArgs e)
        {
            System.Windows.Forms.Application.Exit();
            Environment.Exit(0);
        }
        [DllImport("user32.dll")]
        public static extern bool ReleaseCapture();
        [DllImport("user32.dll")]
        public static extern bool SendMessage(IntPtr hwnd, int wMsg, int wParam, int lParam);
        public const int WM_SYSCOMMAND = 0x0112;
        public const int SC_MOVE = 0xF010;
        public const int HTCAPTION = 0x0002;
        /// <summary>
        /// 为了主窗体可以移动
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form_Main_MouseDown(object sender, MouseEventArgs e)
        {
            ReleaseCapture();
            SendMessage(this.Handle, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
        /// <summary>
        ////////////////////////////////////////////串口配置信息////////////////////////////////////////////////////////// 
        /// </summary>
        public class IniFile
        {
            private string fileName;

            [DllImport("kernel32")]
            private static extern int GetPrivateProfileInt(string lpAppName, string lpKeyName, int nDefault, string lpFileName);
            [DllImport("kernel32")]
            private static extern int GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault,
                StringBuilder lpReturnedString, int nSize, string lpFileName);
            [DllImport("kernel32")]
            private static extern bool WritePrivateProfileString(string lpAppName, string lpKeyName, string lpString, string lpFileName);
            public IniFile(string filename)
            {
                fileName = filename;
            }
            public int ReadInt(string section, string key, int def)
            {
                return GetPrivateProfileInt(section, key, def, fileName);
            }
            public string ReadString(string section, string key, string def)
            {
                StringBuilder temp = new StringBuilder(512);
                GetPrivateProfileString(section, key, def, temp, 512, fileName);
                return temp.ToString();
            }
            public void WriteString(string section, string key, string strVal)
            {
                WritePrivateProfileString(section, key, strVal, fileName);
            }
            public void DelKey(string section, string key)
            {
                WritePrivateProfileString(section, key, "", fileName);
            }

            public void DelSection(string section)
            {
                WritePrivateProfileString(section, "", "", fileName);
            }
            public bool ExistINIFile()
            {
                return File.Exists(fileName);
            }
            public void CreatFile(FileAttributes fileAttributes)
            {
                try
                {
                    using (FileStream fs = File.Create(fileName)) { }
                    File.SetAttributes(fileName, fileAttributes);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "系统提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }          
            public void hide()
            {
                File.SetAttributes(fileName, FileAttributes.Hidden);
            }
        };  
        public void creatConfigFile()
        {
            ini.CreatFile(FileAttributes.Hidden);                 
        }
        public struct _scaleData
        {
            public double Data;
            public string Unit;
        };

        _scaleData ScaleData = new _scaleData();//电子秤数据
        private void addDataToScaleStruct()
        {
            string loadString = "";
            byte[] data = new byte[1];
            System.Text.ASCIIEncoding AasciiEncoding = new ASCIIEncoding();
            data[0] = scaleBuff[0];
            loadString = AasciiEncoding.GetString(data);
            for (int i = 0; i < 7; i++)
            {
                data[0] = scaleBuff[i + 2];
                loadString += AasciiEncoding.GetString(data);
            }
            loadString = loadString.Replace(" ", "");
            ScaleData.Data = Convert.ToDouble(loadString);

            loadString = "";
            for (int i = 0; i < 3; i++)
            {
                data[0] = scaleBuff[i + 9];
                loadString += AasciiEncoding.GetString(data);
            }
            loadString = loadString.Replace(" ", "");
            ScaleData.Unit = loadString;
            addDataToScaleStructFlag = true;
        }
        private void scaleSerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (mIsTryToClosePort) return;
            scaleComReceive = true;
            try
            {
                int n = scaleSerialPort.BytesToRead;
                byte[] readBuf = new byte[n];
                scaleSerialPort.Read(readBuf, 0, n);
                if (n == 14)
                {
                    Array.Copy(readBuf, 0, scaleBuff, 0, scaleBuff.Length);
                    if (scaleBuff[7] == 0x2E && scaleBuff[12] == 0x0D && scaleBuff[13] == 0x0A)
                    {
                        bool correctData = true;
                        //如果数据错误，不会存入数据
                        for (int i = 2; i < 7; i++)
                        {
                            if (i == 7) continue;
                            if (scaleBuff[i] != 0x20 && (scaleBuff[i] < 0x30 || scaleBuff[i] > 0x39))
                            {
                                correctData = false;
                            }
                        }
                        //如果数据错误，不会存入数据
                        if (correctData)
                        {
                            //scaleDataReceive = true;
                            mIsAddingDataToScaleStruct = true;
                            addDataToScaleStruct();
                            mIsAddingDataToScaleStruct = false;
                        }
                    }
                    scaleSerialPort.DiscardInBuffer();
                }
                else
                {
                    scaleSerialPort.DiscardInBuffer();
                }
                readBuf = null;
            }
            catch (SystemException ex)
            {
                MessageBox.Show(ex.Message, "出错提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            scaleComReceive = false;
        }
        //文件串口号读取判断
        private bool ComReadError(string str)
        {
            bool ret = false;
            if (str.Length > 0)
                if (str[0] != 'C' && str[1] != 'O' && str[2] != 'M') ret = true;
            return ret;
        }
        IntPtr[] ptArray = new IntPtr[1];
        //文件串口波特率号读取判断
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

        //等待电子秤数据
        private int waitForScaleData()
        {
            while (mIsAddingDataToScaleStruct) { Thread.Sleep(1); }

            Thread.Sleep(100);
            if (!addDataToScaleStructFlag)
            {
                if (adjustStep == TEST_ABORT || verifyStep == VERIFY_ABORT)
                {
                    return -1;
                }
                if (ScaleTimeTick > 200)
                {
                    MessageBox.Show("与电子秤通讯失败，请检查线缆连接状况！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    if (adjustEnable) adjustStep = TEST_ABORT;
                    else if (verifyEnable) verifyStep = VERIFY_ABORT;
                    return -1;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                ScaleTimeTick = 0;
                return 1;
            }
        }

        //显示清零电子秤的对话框
        private void dispDialogClearScale()
        {
            this.Invoke((EventHandler)(delegate
            {
                //清零电子秤
                ClearScaleForm clearScale = new ClearScaleForm();
                clearScale.StartPosition = FormStartPosition.CenterParent;
                clearScale.ShowDialog();
                while (clearScale.DialogResult == System.Windows.Forms.DialogResult.Cancel)
                {
                    clearScale.ShowDialog();
                    Thread.Sleep(1);
                }
                if (clearScale.DialogResult == System.Windows.Forms.DialogResult.OK)
                {
                    clearScale.Close();
                }
            }));
        }

        //输入电子秤读数
        private string inputScaleWeight()
        {
            string str = "nn";
            //string retStr = "nn";
            this.Invoke((EventHandler)(delegate
            {
                GetScaleWeightForm GetScaleWeight = new GetScaleWeightForm();
                GetScaleWeight.StartPosition = FormStartPosition.CenterParent;
                GetScaleWeight.ShowDialog();
                while (GetScaleWeight.DialogResult == System.Windows.Forms.DialogResult.Cancel)
                {
                    GetScaleWeight.ShowDialog();
                    Thread.Sleep(1);
                }
                if (GetScaleWeight.DialogResult == System.Windows.Forms.DialogResult.OK)
                {
                    str = GetScaleWeight.ScaleWeight;
                    GetScaleWeight.Close();
                }
            }));            
            return str;
        }

        //打开电子秤串口
        private bool scaleSerialPortOpen()
        {
            string portName = "COM3";
            string baudRate = "9600";

            if (!ini.ExistINIFile())
            {
                creatConfigFile();
            }
            else
            {
                portName = ini.ReadString("COM", "ScaleCom", "COM3");
                baudRate = ini.ReadString("BAUD", "ScaleComBaud", "9600");
                if (ComReadError(portName))
                {
                    portName = "COM3";
                }
                if (BaudReadError(baudRate))
                {
                    baudRate = "9600";
                }
            }

            if (!ScaleSerialPortIsOpen)
            {
                scaleSerialPort.PortName = portName;
                scaleSerialPort.BaudRate = Convert.ToInt16(baudRate);
                scaleSerialPort.DataBits = 8;
                scaleSerialPort.StopBits = StopBits.One;
                scaleSerialPort.Parity = Parity.None;
                scaleSerialPort.ReceivedBytesThreshold = 14;
                scaleSerialPort.ReadTimeout = 300;
                scaleSerialPort.DataReceived += new SerialDataReceivedEventHandler(scaleSerialPort_DataReceived);
                try
                {
                    scaleSerialPort.Open();
                    scaleSerialPort.DiscardInBuffer();
                    scaleSerialPort.DiscardOutBuffer();
                    mIsTryToClosePort = false;
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "系统提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
            }
            return true;
        }

        //关闭电子秤串口
        private bool scaleSerialPortClose()
        {
            mIsTryToClosePort = true;
            while (scaleComReceive) { Thread.Sleep(1); }
            try
            {
                scaleSerialPort.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "系统提示", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }
            return true;
        }

        public struct _configParam
        {
            public string AutoReadScale;
            public int TestSuccess;//标定成功
            public int TestFail;//标定失败
            public int TestSuccessTime;
            public int TestFailTime;
            public int VerifySuccess;//气助校验成功
            public int VerifyFail;//气助校验失败
            public int VerifySuccessTime;
            public int VerifyFailTime;
            public int NoGasVerifySuccess;//非气助校验成功
            public int NoGasVerifyFail;//非气助校验失败
            public int MessJetSuccess;//计量喷嘴成功
            public int MessJetFail;//计量喷嘴失败
            public int ScaleStableDelay;
            public string NowDate;
            public string LastSN;
            public int T1MStableDiff;
            public string TotalUseMax;
            public string TotalUse;
        }

        public class _Config
        {
            public _configParam Param;
            public void ReadData(IniFile ini)
            {
                Param.TestSuccess = ini.ReadInt("TEST", "TestSuccess", 0);  //当日成功数
                Param.TestFail = ini.ReadInt("TEST", "TestFail", 0);        //当日失败数
                Param.NowDate = ini.ReadString("DATE", "NowDate", DateTime.Now.ToLongDateString()); //记录时间是否更新
                Param.LastSN = ini.ReadString("SN", "LastSN", "0000000000");                        //记录上次编号？？
                Param.TotalUseMax = ini.ReadString("RECORED", "TotalUseMax", "");                    //最大使用次数？
                Param.TotalUse = ini.ReadString("RECORED", "TotalUse", "");                          //设置使用最大的次数？
                Param.AutoReadScale = ini.ReadString("GETWEIGHT", "AutoReadScale", "");//
                Param.ScaleStableDelay = ini.ReadInt("SCALESTABLETIME", "ScaleStableDelay", 1);
            }
        };
        public _Config Config = new _Config();
        UInt32 Flag_clear = 0;
        UInt32 OPen_scale = 0;
        UInt32 start_scale = 0;
        private void Button6_Click(object sender, EventArgs e)
        {
            
            if ("Y" == Config.Param.AutoReadScale)  //如果是自动读数的
            {
                Openserial = true; //读取标志位
                if (!ScaleSerialPortIsOpen)
                {
                    if (scaleSerialPortOpen())
                    {
                        ScaleSerialPortIsOpen = true;
                        openComButton.Text = "关闭串口";
                        infoRichTextBox.AppendText("\n启动电子秤、流量自动称重\n电子秤串口打开成功!\n串口号：" + scaleSerialPort.PortName +" 波特率：" + scaleSerialPort.BaudRate);
                        infoRichTextBox.ScrollToCaret();
                    }
                }
                else
                {
                    
                    if (scaleSerialPortClose())
                    {
                        ScaleSerialPortIsOpen = false;
                        openComButton.Text = "打开串口";
                        infoRichTextBox.AppendText("\n关闭自动测试称重 \n关闭电子秤串口！");
                        infoRichTextBox.ScrollToCaret();
                    }
                }
                //如果无法同时打开或者关闭，则将打开的关闭

            }
            else          //如果是手动测试的
            {
                if (OPen_scale == 0)
                {
                    openComButton.Text = "关闭串口";
                    Openserial = true; //读取标志位
                    Flag_clear = 1;
                    OPen_scale = 1;
                    infoRichTextBox.AppendText("\n启动手动测试流量!");
                }
                else
                {
                    openComButton.Text = "打开串口";
                    OPen_scale = 0;
                    Openserial = false;
                    infoRichTextBox.AppendText("\n关闭手动流量测试!");
                    infoRichTextBox.ScrollToCaret();
                }
            }
            SNComboBox.Focus();
        }   
        private void 自动读秤ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            自动读秤ToolStripMenuItem.Checked = !自动读秤ToolStripMenuItem.Checked;
            if (自动读秤ToolStripMenuItem.Checked)
            {
                ini.WriteString("GETWEIGHT", "AutoReadScale", "Y");
            }
            else
            {
                ini.WriteString("GETWEIGHT", "AutoReadScale", "N");
            }
            设置ToolStripMenuItem.ShowDropDown();
            Config.ReadData(ini);
        }

        private void 通讯设置ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form_balance comSet = new Form_balance(this);
            comSet.StartPosition = FormStartPosition.CenterParent;
            comSet.ShowDialog();
        }
        private void 文档记录ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TestFileSetForm TestFileSet = new TestFileSetForm(this);
            TestFileSet.StartPosition = FormStartPosition.CenterParent;
            TestFileSet.ShowDialog();
        }
        private void 记录ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormRecord_2 Record = new FormRecord_2(this);
            Record.StartPosition = FormStartPosition.CenterParent;
            Record.ShowDialog();
        }
        private void button1_Click_1(object sender, EventArgs e)//测试称重
        {
            if (!ScaleSerialPortIsOpen)
            {
                MessageBox.Show("串口打开失败");
                return;
            }
            //clearScale();
            getWeightOnScale();
        }

        /// <summary>
        /// 获取程序重量，两种方式自动获取和手动获取 可供选择
        /// </summary>
        /// <returns></returns>
        private string getWeightOnScale()
        {
            string value = "";

            if ("Y" == Config.Param.AutoReadScale)   //自动读取电子秤的总量
            {
                //输入秤的重量
                delayByTimer(Config.Param.ScaleStableDelay);//延时等待稳定
                if (GetScaleWeightStable())     //将秤的读数有串口发送给上位机
                {
                    value = ScaleData.Data.ToString();
                    if (weight_n < 30)
                    {
                        weight[weight_n] = System.Convert.ToInt32(value);
                        weight_n++;
                    }
                }
                else return "COM ERROR";
                delayByTimer(1);//延时等待稳定
                dispInfoOnTextBox("\n获取重量：" + value + ScaleData.Unit);
            }
            else                                    //手动输入电子秤的总量
            {
                value = inputScaleWeight();         //跳出用户输入读数界面
                if (weight_n < 30)
                {
                    weight[weight_n] = System.Convert.ToInt32(value);
                    weight_n++;
                }
                dispInfoOnTextBox("\n获取重量：" + value + "mg");
            }
            return value;
        }
        /// <summary>
        /// 清零电子秤
        /// </summary>
        /// <returns></returns>
        private bool clearScale()
        {
            ScaleTimeTick = 0;
            do
            {
                if (!scaleComReceive)
                {
                    data[0] = 0x1B;
                    data[1] = 0X74;
                    addDataToScaleStructFlag = false;
                    writeScaleData(data);
                    Thread.Sleep(200);
                    data[0] = 0x1B;
                    data[1] = 0X70;

                    writeScaleData(data);
                    Thread.Sleep(300);
                    if (-1 == waitForScaleData())
                        return false;
                }
                Thread.Sleep(5);
            } while (ScaleData.Data != 0);

            this.Invoke((EventHandler)(delegate
            {
                infoRichTextBox.AppendText("\n清零电子秤为 " + ScaleData.Data.ToString() + "g");
                infoRichTextBox.ScrollToCaret();
            }));
            return true;
        }
        /// <summary>
        /// 串口发送数据界面显示
        /// </summary>
        /// <param name="_data"></param>
        private void writeScaleData(byte[] _data)
        {
            if (mIsTryToClosePort) return;
            if (ScaleSerialPortIsOpen)
            {
                try
                {
                    this.scaleSerialPort.Write(_data, 0, 2);
                    this.Invoke((EventHandler)(delegate
                    {
                        //infoRichTextBox.AppendText("\n发送正常");
                        //infoRichTextBox.ScrollToCaret();
                    }));
                }
                catch (SystemException ex)
                {
                    MessageBox.Show(ex.Message, "提示");
                    this.Invoke((EventHandler)(delegate
                    {
                        infoRichTextBox.AppendText("发送异常");
                        infoRichTextBox.ScrollToCaret();
                    }));
                }
            }
        }
        private void button2_Click(object sender, EventArgs e) //测试去皮
        {
            if (!ScaleSerialPortIsOpen)
            {
                MessageBox.Show("串口打开失败");
                return;
            }
            clearTheScale();
            //clearScale();
        }

        private bool clearTheScale()  //清除电子秤的读数
        {
            if ("Y" == Config.Param.AutoReadScale)
            {
                Thread.Sleep(200);
                if (clearScale()) return true;
                else return false;
            }
            else
            {
                dispDialogClearScale();
                string dispStr = "\n清零电子秤";
                dispInfoOnTextBox(dispStr);
                return true;
            }
        }
        private void dispInfoOnTextBox(string str)  //向窗体中显示称重数据
        {
            this.Invoke((EventHandler)(delegate
            {
                
                infoRichTextBox.AppendText(str);
                infoRichTextBox.ScrollToCaret();
            }));
        }

        //获取称重数据
        private bool getScaleData()
        {
            data[0] = 0x1B;
            data[1] = 0x70;
            ScaleTimeTick = 0;
            addDataToScaleStructFlag = false;
            do
            {
                if (!scaleComReceive)
                {
                    writeScaleData(data);
                    Thread.Sleep(400);
                    if (-1 == waitForScaleData())
                        return false;
                }
                Thread.Sleep(2);
            } while (!addDataToScaleStructFlag);
            return true;
        }

        private bool GetScaleWeightStable()
        {
            bool scaleDataStable = true;
            double[] tempWeight = new double[3] { 0, 0, 0 };
            while (true)
            {
                int i = 0;
                for (i = 0; i < tempWeight.Length; i++)
                {
                    if (!getScaleData())
                        return false;
                    tempWeight[i] = ScaleData.Data;
                    Thread.Sleep(2);
                }

                scaleDataStable = true;
                for (i = 1; i < tempWeight.Length; i++)
                {
                    if (tempWeight[0] != tempWeight[i])
                    {
                        scaleDataStable = false;
                    }
                }

                if (scaleDataStable)
                {
                    return true;
                }
                Thread.Sleep(2);
            }
        }
      
        //延时,s
        private bool delayByTimer(int second)
        {
            int delayTime = 0;
            int delayNumber = (second * 1000) / TICK_TIME;
            while (true)
            {
                delayTime++;
                if (delayTime > delayNumber)
                {
                    return true;
                }
                Thread.Sleep(TICK_TIME);
            }
        }
        private void 参数设置ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form_argement canshu  = new Form_argement(this);
            canshu.StartPosition = FormStartPosition.CenterParent;
            canshu.ShowDialog();
        }

        /// <summary>
        /// 绘制称重图像
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void chart1_GetToolTipText(object sender, ToolTipEventArgs e)
        {
            HitTestResult myTestResult = chart1.HitTest(e.X, e.Y, ChartElementType.DataPoint);//获取命中测试的结果
            if (myTestResult.ChartElementType == ChartElementType.DataPoint)
            {
                int n = myTestResult.PointIndex;
                DataPoint dp = myTestResult.Series.Points[n];
                string XValue = dp.XValue.ToString();//获取数据点的X值
                string YValue = dp.YValues[0].ToString();//获取数据点的Y值
                e.Text = "道址:" + XValue + "\r\n数值" + YValue;
            }
        }

        private void 帮助ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form_Hint hint = new Form_Hint(this);
            hint.StartPosition = FormStartPosition.CenterParent;
            hint.ShowDialog();
        }

        private void button_result2_Click(object sender, EventArgs e)
        {

        }
        private void listBox2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void 标定ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //textBox_DianYa.Visible = true;
            //textBox_DianLiu.Visible = true;
            ///textBox_YaLi.Visible = true;
            //textBox_WenDu.Visible = true;
        }

        private void 测试数据ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            textBox_DianYa.Visible = true;
            textBox_DianLiu.Visible = true;
            textBox_YaLi.Visible = true;
            textBox_WenDu.Visible = true;
            label15.Visible = true;
            label16.Visible= true;
            label17.Visible= true;
            label18.Visible= true;
            label19.Visible= true;

        }

        private void 退出标定ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            textBox_DianYa.Visible = false;
            textBox_DianLiu.Visible = false;
            textBox_YaLi.Visible = false;
            textBox_WenDu.Visible = false;
            label15.Visible = false;
            label16.Visible = false;
            label17.Visible = false;
            label18.Visible = false;
            label19.Visible = false;
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int n = scaleSerialPort.BytesToRead;
            byte[] readBuf = new byte[n];
            scaleComReceive = true;
            scaleSerialPort.Read(readBuf, 0, n);
            if (n == 14)
            {
                Array.Copy(readBuf, 0, scaleBuff, 0, scaleBuff.Length);
                
                addDataToScaleStruct();
                scaleSerialPort.DiscardInBuffer();
            }
            else
            {
                scaleSerialPort.DiscardInBuffer();
            }
            readBuf = null;
            scaleComReceive = false;
        }

        private void time_Tick(object sender, EventArgs e)
        {

        }
    }
}
