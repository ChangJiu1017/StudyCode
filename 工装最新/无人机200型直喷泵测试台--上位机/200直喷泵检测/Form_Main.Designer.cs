namespace _200直喷泵检测
{
    partial class Form_Main
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form_Main));
            this.label_project = new System.Windows.Forms.Label();
            this.comboBox_Project = new System.Windows.Forms.ComboBox();
            this.button_manage = new System.Windows.Forms.Button();
            this.button_record = new System.Windows.Forms.Button();
            this.dataGridView_Param = new System.Windows.Forms.DataGridView();
            this.label1 = new System.Windows.Forms.Label();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.listBox2 = new System.Windows.Forms.ListBox();
            this.label2 = new System.Windows.Forms.Label();
            this.start = new System.Windows.Forms.Button();
            this.stop = new System.Windows.Forms.Button();
            this.text_lond = new System.Windows.Forms.ListBox();
            this.button_openOrCloseCAN = new System.Windows.Forms.Button();
            this.leb_88 = new System.Windows.Forms.Label();
            this.label_Qualified = new System.Windows.Forms.Label();
            this.label_UnQualified = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox_pres = new System.Windows.Forms.TextBox();
            this.button_result1 = new System.Windows.Forms.Button();
            this.button_result2 = new System.Windows.Forms.Button();
            this.label10 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.progressBar2 = new System.Windows.Forms.ProgressBar();
            this.SNComboBox = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.openComButton = new System.Windows.Forms.Button();
            this.time = new System.Windows.Forms.Timer(this.components);
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.设置ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.自动读秤ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.通讯设置ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.文档记录ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.参数设置ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.记录ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.帮助ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.标定ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.测试数据ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.退出标定ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.infoRichTextBox = new System.Windows.Forms.RichTextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.label9 = new System.Windows.Forms.Label();
            this.listBox3 = new System.Windows.Forms.ListBox();
            this.button_result3 = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox_DianYa = new System.Windows.Forms.TextBox();
            this.textBox_DianLiu = new System.Windows.Forms.TextBox();
            this.textBox_WenDu = new System.Windows.Forms.TextBox();
            this.textBox_YaLi = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.textBox_electric = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView_Param)).BeginInit();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            this.SuspendLayout();
            // 
            // label_project
            // 
            this.label_project.AutoSize = true;
            this.label_project.BackColor = System.Drawing.Color.Transparent;
            this.label_project.Font = new System.Drawing.Font("楷体", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_project.ForeColor = System.Drawing.SystemColors.Desktop;
            this.label_project.Location = new System.Drawing.Point(15, 89);
            this.label_project.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label_project.Name = "label_project";
            this.label_project.Size = new System.Drawing.Size(156, 20);
            this.label_project.TabIndex = 0;
            this.label_project.Text = "选择测试项目：";
            // 
            // comboBox_Project
            // 
            this.comboBox_Project.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox_Project.FormattingEnabled = true;
            this.comboBox_Project.Location = new System.Drawing.Point(163, 86);
            this.comboBox_Project.Margin = new System.Windows.Forms.Padding(4);
            this.comboBox_Project.Name = "comboBox_Project";
            this.comboBox_Project.Size = new System.Drawing.Size(178, 26);
            this.comboBox_Project.TabIndex = 1;
            this.comboBox_Project.SelectedIndexChanged += new System.EventHandler(this.comboBox_Project_SelectedIndexChanged);
            // 
            // button_manage
            // 
            this.button_manage.BackColor = System.Drawing.Color.Azure;
            this.button_manage.Location = new System.Drawing.Point(19, 40);
            this.button_manage.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button_manage.Name = "button_manage";
            this.button_manage.Size = new System.Drawing.Size(106, 30);
            this.button_manage.TabIndex = 2;
            this.button_manage.Text = "项目设置";
            this.button_manage.UseVisualStyleBackColor = false;
            this.button_manage.Click += new System.EventHandler(this.button_manage_Click_1);
            // 
            // button_record
            // 
            this.button_record.BackColor = System.Drawing.Color.Azure;
            this.button_record.Location = new System.Drawing.Point(163, 42);
            this.button_record.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button_record.Name = "button_record";
            this.button_record.Size = new System.Drawing.Size(79, 30);
            this.button_record.TabIndex = 3;
            this.button_record.Text = "记录";
            this.button_record.UseVisualStyleBackColor = false;
            this.button_record.Click += new System.EventHandler(this.button_record_Click);
            // 
            // dataGridView_Param
            // 
            this.dataGridView_Param.AllowUserToAddRows = false;
            this.dataGridView_Param.AllowUserToDeleteRows = false;
            this.dataGridView_Param.AllowUserToResizeColumns = false;
            this.dataGridView_Param.AllowUserToResizeRows = false;
            this.dataGridView_Param.ColumnHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
            this.dataGridView_Param.ColumnHeadersHeight = 29;
            this.dataGridView_Param.GridColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.dataGridView_Param.Location = new System.Drawing.Point(19, 122);
            this.dataGridView_Param.Margin = new System.Windows.Forms.Padding(4);
            this.dataGridView_Param.Name = "dataGridView_Param";
            this.dataGridView_Param.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.AutoSizeToAllHeaders;
            this.dataGridView_Param.RowTemplate.Height = 27;
            this.dataGridView_Param.Size = new System.Drawing.Size(321, 565);
            this.dataGridView_Param.TabIndex = 4;
            this.dataGridView_Param.TabStop = false;
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("楷体", 15F, System.Drawing.FontStyle.Bold);
            this.label1.ForeColor = System.Drawing.SystemColors.Desktop;
            this.label1.Location = new System.Drawing.Point(468, 42);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(356, 34);
            this.label1.TabIndex = 5;
            this.label1.Text = "无人机200型直喷泵检测工装";
            // 
            // listBox1
            // 
            this.listBox1.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.listBox1.FormattingEnabled = true;
            this.listBox1.ItemHeight = 20;
            this.listBox1.Location = new System.Drawing.Point(530, 122);
            this.listBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(163, 104);
            this.listBox1.TabIndex = 6;
            this.listBox1.Visible = false;
            this.listBox1.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);
            // 
            // listBox2
            // 
            this.listBox2.FormattingEnabled = true;
            this.listBox2.ItemHeight = 18;
            this.listBox2.Location = new System.Drawing.Point(350, 122);
            this.listBox2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listBox2.Name = "listBox2";
            this.listBox2.Size = new System.Drawing.Size(175, 112);
            this.listBox2.Sorted = true;
            this.listBox2.TabIndex = 7;
            this.listBox2.Visible = false;
            this.listBox2.SelectedIndexChanged += new System.EventHandler(this.listBox2_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(358, 301);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 22);
            this.label2.TabIndex = 10;
            this.label2.Text = "温度：";
            // 
            // start
            // 
            this.start.BackColor = System.Drawing.Color.Lime;
            this.start.Location = new System.Drawing.Point(1107, 122);
            this.start.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.start.Name = "start";
            this.start.Size = new System.Drawing.Size(97, 34);
            this.start.TabIndex = 17;
            this.start.Text = "启动";
            this.start.UseVisualStyleBackColor = false;
            this.start.Click += new System.EventHandler(this.start_Click);
            // 
            // stop
            // 
            this.stop.BackColor = System.Drawing.Color.Coral;
            this.stop.Location = new System.Drawing.Point(1107, 200);
            this.stop.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.stop.Name = "stop";
            this.stop.Size = new System.Drawing.Size(97, 34);
            this.stop.TabIndex = 18;
            this.stop.Text = "停止";
            this.stop.UseVisualStyleBackColor = false;
            this.stop.Click += new System.EventHandler(this.stop_Click);
            // 
            // text_lond
            // 
            this.text_lond.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.text_lond.FormattingEnabled = true;
            this.text_lond.ItemHeight = 18;
            this.text_lond.Location = new System.Drawing.Point(867, 122);
            this.text_lond.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.text_lond.Name = "text_lond";
            this.text_lond.Size = new System.Drawing.Size(235, 112);
            this.text_lond.TabIndex = 31;
            // 
            // button_openOrCloseCAN
            // 
            this.button_openOrCloseCAN.BackColor = System.Drawing.Color.MediumAquamarine;
            this.button_openOrCloseCAN.Location = new System.Drawing.Point(867, 82);
            this.button_openOrCloseCAN.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button_openOrCloseCAN.Name = "button_openOrCloseCAN";
            this.button_openOrCloseCAN.Size = new System.Drawing.Size(122, 30);
            this.button_openOrCloseCAN.TabIndex = 32;
            this.button_openOrCloseCAN.Text = "开启CAN通讯";
            this.button_openOrCloseCAN.UseVisualStyleBackColor = false;
            this.button_openOrCloseCAN.Click += new System.EventHandler(this.can_init_Click);
            // 
            // leb_88
            // 
            this.leb_88.BackColor = System.Drawing.Color.DarkGreen;
            this.leb_88.Font = new System.Drawing.Font("宋体", 16.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.leb_88.ForeColor = System.Drawing.SystemColors.ControlText;
            this.leb_88.Location = new System.Drawing.Point(350, 122);
            this.leb_88.Name = "leb_88";
            this.leb_88.Size = new System.Drawing.Size(511, 112);
            this.leb_88.TabIndex = 35;
            this.leb_88.Text = "测试中...";
            this.leb_88.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.leb_88.Visible = false;
            // 
            // label_Qualified
            // 
            this.label_Qualified.BackColor = System.Drawing.Color.DarkOliveGreen;
            this.label_Qualified.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_Qualified.ForeColor = System.Drawing.SystemColors.ControlText;
            this.label_Qualified.Location = new System.Drawing.Point(446, 660);
            this.label_Qualified.Name = "label_Qualified";
            this.label_Qualified.Size = new System.Drawing.Size(127, 22);
            this.label_Qualified.TabIndex = 36;
            this.label_Qualified.Text = " 合格:";
            // 
            // label_UnQualified
            // 
            this.label_UnQualified.BackColor = System.Drawing.Color.Firebrick;
            this.label_UnQualified.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_UnQualified.ForeColor = System.Drawing.SystemColors.ControlText;
            this.label_UnQualified.Location = new System.Drawing.Point(643, 660);
            this.label_UnQualified.Name = "label_UnQualified";
            this.label_UnQualified.Size = new System.Drawing.Size(139, 22);
            this.label_UnQualified.TabIndex = 37;
            this.label_UnQualified.Text = "不合格:";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(417, 295);
            this.textBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(75, 28);
            this.textBox1.TabIndex = 38;
            // 
            // textBox_pres
            // 
            this.textBox_pres.Location = new System.Drawing.Point(562, 295);
            this.textBox_pres.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox_pres.Name = "textBox_pres";
            this.textBox_pres.ReadOnly = true;
            this.textBox_pres.Size = new System.Drawing.Size(76, 28);
            this.textBox_pres.TabIndex = 39;
            // 
            // button_result1
            // 
            this.button_result1.BackColor = System.Drawing.Color.White;
            this.button_result1.Cursor = System.Windows.Forms.Cursors.Default;
            this.button_result1.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_result1.Location = new System.Drawing.Point(350, 122);
            this.button_result1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button_result1.Name = "button_result1";
            this.button_result1.Size = new System.Drawing.Size(176, 112);
            this.button_result1.TabIndex = 42;
            this.button_result1.UseVisualStyleBackColor = false;
            // 
            // button_result2
            // 
            this.button_result2.BackColor = System.Drawing.Color.White;
            this.button_result2.Cursor = System.Windows.Forms.Cursors.Default;
            this.button_result2.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_result2.Location = new System.Drawing.Point(530, 122);
            this.button_result2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button_result2.Name = "button_result2";
            this.button_result2.Size = new System.Drawing.Size(163, 112);
            this.button_result2.TabIndex = 43;
            this.button_result2.UseVisualStyleBackColor = false;
            this.button_result2.Click += new System.EventHandler(this.button_result2_Click);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(498, 301);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(62, 18);
            this.label10.TabIndex = 104;
            this.label10.Text = "压力：";
            // 
            // label5
            // 
            this.label5.Font = new System.Drawing.Font("楷体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(380, 242);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(127, 22);
            this.label5.TabIndex = 109;
            this.label5.Text = "电流、电压";
            // 
            // label6
            // 
            this.label6.Font = new System.Drawing.Font("楷体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label6.Location = new System.Drawing.Point(558, 242);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(102, 22);
            this.label6.TabIndex = 110;
            this.label6.Text = "流量采样";
            // 
            // label8
            // 
            this.label8.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label8.Location = new System.Drawing.Point(922, 242);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(125, 22);
            this.label8.TabIndex = 111;
            this.label8.Text = "操作步骤显示";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(1079, 274);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(125, 18);
            this.label7.TabIndex = 103;
            this.label7.Text = "数据处理中...";
            this.label7.Visible = false;
            // 
            // progressBar2
            // 
            this.progressBar2.ForeColor = System.Drawing.Color.Lime;
            this.progressBar2.Location = new System.Drawing.Point(350, 266);
            this.progressBar2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.progressBar2.MarqueeAnimationSpeed = 1000;
            this.progressBar2.Maximum = 1000;
            this.progressBar2.Name = "progressBar2";
            this.progressBar2.Size = new System.Drawing.Size(724, 23);
            this.progressBar2.TabIndex = 102;
            this.progressBar2.Visible = false;
            // 
            // SNComboBox
            // 
            this.SNComboBox.FormattingEnabled = true;
            this.SNComboBox.Location = new System.Drawing.Point(498, 86);
            this.SNComboBox.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.SNComboBox.Name = "SNComboBox";
            this.SNComboBox.Size = new System.Drawing.Size(263, 26);
            this.SNComboBox.TabIndex = 112;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.ForeColor = System.Drawing.SystemColors.Desktop;
            this.label3.Location = new System.Drawing.Point(358, 89);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(134, 18);
            this.label3.TabIndex = 113;
            this.label3.Text = "输入产品编号：";
            // 
            // openComButton
            // 
            this.openComButton.BackColor = System.Drawing.Color.MediumSpringGreen;
            this.openComButton.Location = new System.Drawing.Point(997, 82);
            this.openComButton.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.openComButton.Name = "openComButton";
            this.openComButton.Size = new System.Drawing.Size(104, 30);
            this.openComButton.TabIndex = 114;
            this.openComButton.Text = "打开串口";
            this.openComButton.UseVisualStyleBackColor = false;
            this.openComButton.Click += new System.EventHandler(this.Button6_Click);
            // 
            // time
            // 
            this.time.Interval = 1000;
            this.time.Tick += new System.EventHandler(this.time_Tick);
            // 
            // serialPort1
            // 
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // menuStrip1
            // 
            this.menuStrip1.AutoSize = false;
            this.menuStrip1.BackColor = System.Drawing.Color.LightSeaGreen;
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.设置ToolStripMenuItem,
            this.记录ToolStripMenuItem,
            this.帮助ToolStripMenuItem,
            this.标定ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Padding = new System.Windows.Forms.Padding(7, 2, 0, 2);
            this.menuStrip1.Size = new System.Drawing.Size(1293, 31);
            this.menuStrip1.TabIndex = 116;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // 设置ToolStripMenuItem
            // 
            this.设置ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.自动读秤ToolStripMenuItem,
            this.通讯设置ToolStripMenuItem,
            this.文档记录ToolStripMenuItem,
            this.参数设置ToolStripMenuItem});
            this.设置ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.设置ToolStripMenuItem.Name = "设置ToolStripMenuItem";
            this.设置ToolStripMenuItem.Size = new System.Drawing.Size(60, 27);
            this.设置ToolStripMenuItem.Text = "设置";
            // 
            // 自动读秤ToolStripMenuItem
            // 
            this.自动读秤ToolStripMenuItem.Checked = true;
            this.自动读秤ToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.自动读秤ToolStripMenuItem.Name = "自动读秤ToolStripMenuItem";
            this.自动读秤ToolStripMenuItem.Size = new System.Drawing.Size(166, 28);
            this.自动读秤ToolStripMenuItem.Text = "自动读秤";
            this.自动读秤ToolStripMenuItem.Click += new System.EventHandler(this.自动读秤ToolStripMenuItem_Click);
            // 
            // 通讯设置ToolStripMenuItem
            // 
            this.通讯设置ToolStripMenuItem.Name = "通讯设置ToolStripMenuItem";
            this.通讯设置ToolStripMenuItem.Size = new System.Drawing.Size(166, 28);
            this.通讯设置ToolStripMenuItem.Text = "通讯设置";
            this.通讯设置ToolStripMenuItem.Click += new System.EventHandler(this.通讯设置ToolStripMenuItem_Click);
            // 
            // 文档记录ToolStripMenuItem
            // 
            this.文档记录ToolStripMenuItem.Name = "文档记录ToolStripMenuItem";
            this.文档记录ToolStripMenuItem.Size = new System.Drawing.Size(166, 28);
            this.文档记录ToolStripMenuItem.Text = "文档记录";
            this.文档记录ToolStripMenuItem.Click += new System.EventHandler(this.文档记录ToolStripMenuItem_Click);
            // 
            // 参数设置ToolStripMenuItem
            // 
            this.参数设置ToolStripMenuItem.Name = "参数设置ToolStripMenuItem";
            this.参数设置ToolStripMenuItem.Size = new System.Drawing.Size(166, 28);
            this.参数设置ToolStripMenuItem.Text = "参数设置";
            this.参数设置ToolStripMenuItem.Click += new System.EventHandler(this.参数设置ToolStripMenuItem_Click);
            // 
            // 记录ToolStripMenuItem
            // 
            this.记录ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.记录ToolStripMenuItem.Name = "记录ToolStripMenuItem";
            this.记录ToolStripMenuItem.Size = new System.Drawing.Size(60, 27);
            this.记录ToolStripMenuItem.Text = "记录";
            this.记录ToolStripMenuItem.Click += new System.EventHandler(this.记录ToolStripMenuItem_Click);
            // 
            // 帮助ToolStripMenuItem
            // 
            this.帮助ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.帮助ToolStripMenuItem.Name = "帮助ToolStripMenuItem";
            this.帮助ToolStripMenuItem.Size = new System.Drawing.Size(60, 27);
            this.帮助ToolStripMenuItem.Text = "帮助";
            this.帮助ToolStripMenuItem.Click += new System.EventHandler(this.帮助ToolStripMenuItem_Click);
            // 
            // 标定ToolStripMenuItem
            // 
            this.标定ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.测试数据ToolStripMenuItem,
            this.退出标定ToolStripMenuItem});
            this.标定ToolStripMenuItem.Font = new System.Drawing.Font("Microsoft YaHei UI", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.标定ToolStripMenuItem.Name = "标定ToolStripMenuItem";
            this.标定ToolStripMenuItem.Size = new System.Drawing.Size(60, 27);
            this.标定ToolStripMenuItem.Text = "标定";
            this.标定ToolStripMenuItem.Click += new System.EventHandler(this.标定ToolStripMenuItem_Click);
            // 
            // 测试数据ToolStripMenuItem
            // 
            this.测试数据ToolStripMenuItem.Name = "测试数据ToolStripMenuItem";
            this.测试数据ToolStripMenuItem.Size = new System.Drawing.Size(166, 28);
            this.测试数据ToolStripMenuItem.Text = "测试数据";
            this.测试数据ToolStripMenuItem.Click += new System.EventHandler(this.测试数据ToolStripMenuItem_Click);
            // 
            // 退出标定ToolStripMenuItem
            // 
            this.退出标定ToolStripMenuItem.Name = "退出标定ToolStripMenuItem";
            this.退出标定ToolStripMenuItem.Size = new System.Drawing.Size(166, 28);
            this.退出标定ToolStripMenuItem.Text = "退出标定";
            this.退出标定ToolStripMenuItem.Click += new System.EventHandler(this.退出标定ToolStripMenuItem_Click);
            // 
            // infoRichTextBox
            // 
            this.infoRichTextBox.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.infoRichTextBox.Location = new System.Drawing.Point(844, 330);
            this.infoRichTextBox.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.infoRichTextBox.Name = "infoRichTextBox";
            this.infoRichTextBox.Size = new System.Drawing.Size(258, 271);
            this.infoRichTextBox.TabIndex = 117;
            this.infoRichTextBox.Text = "";
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.Aquamarine;
            this.button1.Location = new System.Drawing.Point(1107, 575);
            this.button1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(97, 26);
            this.button1.TabIndex = 118;
            this.button1.Text = "称重";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // button2
            // 
            this.button2.BackColor = System.Drawing.SystemColors.GrayText;
            this.button2.ForeColor = System.Drawing.Color.Black;
            this.button2.Location = new System.Drawing.Point(1107, 506);
            this.button2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(97, 30);
            this.button2.TabIndex = 119;
            this.button2.Text = "去皮";
            this.button2.UseVisualStyleBackColor = false;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("楷体", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.Location = new System.Drawing.Point(872, 620);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(202, 23);
            this.label4.TabIndex = 120;
            this.label4.Text = "电子秤操作及数值";
            // 
            // chart1
            // 
            chartArea1.Name = "ChartArea1";
            this.chart1.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.chart1.Legends.Add(legend1);
            this.chart1.Location = new System.Drawing.Point(350, 330);
            this.chart1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chart1.Name = "chart1";
            this.chart1.Palette = System.Windows.Forms.DataVisualization.Charting.ChartColorPalette.None;
            series1.ChartArea = "ChartArea1";
            series1.Legend = "Legend1";
            series1.Name = "Series1";
            this.chart1.Series.Add(series1);
            this.chart1.Size = new System.Drawing.Size(487, 271);
            this.chart1.TabIndex = 121;
            this.chart1.Text = "chart1";
            this.chart1.GetToolTipText += new System.EventHandler<System.Windows.Forms.DataVisualization.Charting.ToolTipEventArgs>(this.chart1_GetToolTipText);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("楷体", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label9.Location = new System.Drawing.Point(469, 620);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(202, 23);
            this.label9.TabIndex = 122;
            this.label9.Text = "流量采集显示图像";
            // 
            // listBox3
            // 
            this.listBox3.FormattingEnabled = true;
            this.listBox3.ItemHeight = 18;
            this.listBox3.Location = new System.Drawing.Point(698, 122);
            this.listBox3.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.listBox3.Name = "listBox3";
            this.listBox3.Size = new System.Drawing.Size(163, 112);
            this.listBox3.TabIndex = 123;
            this.listBox3.Visible = false;
            // 
            // button_result3
            // 
            this.button_result3.BackColor = System.Drawing.Color.White;
            this.button_result3.Cursor = System.Windows.Forms.Cursors.Default;
            this.button_result3.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_result3.Location = new System.Drawing.Point(698, 122);
            this.button_result3.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button_result3.Name = "button_result3";
            this.button_result3.Size = new System.Drawing.Size(163, 112);
            this.button_result3.TabIndex = 124;
            this.button_result3.UseVisualStyleBackColor = false;
            // 
            // label11
            // 
            this.label11.Font = new System.Drawing.Font("楷体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label11.Location = new System.Drawing.Point(722, 242);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(115, 22);
            this.label11.TabIndex = 125;
            this.label11.Text = "温度、压力";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(781, 301);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(62, 18);
            this.label12.TabIndex = 126;
            this.label12.Text = "电压：";
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(844, 295);
            this.textBox2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.Size = new System.Drawing.Size(80, 28);
            this.textBox2.TabIndex = 127;
            // 
            // textBox_DianYa
            // 
            this.textBox_DianYa.Location = new System.Drawing.Point(1161, 330);
            this.textBox_DianYa.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox_DianYa.Name = "textBox_DianYa";
            this.textBox_DianYa.ReadOnly = true;
            this.textBox_DianYa.Size = new System.Drawing.Size(90, 28);
            this.textBox_DianYa.TabIndex = 128;
            // 
            // textBox_DianLiu
            // 
            this.textBox_DianLiu.Location = new System.Drawing.Point(1161, 373);
            this.textBox_DianLiu.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox_DianLiu.Name = "textBox_DianLiu";
            this.textBox_DianLiu.ReadOnly = true;
            this.textBox_DianLiu.Size = new System.Drawing.Size(90, 28);
            this.textBox_DianLiu.TabIndex = 129;
            // 
            // textBox_WenDu
            // 
            this.textBox_WenDu.Location = new System.Drawing.Point(1161, 464);
            this.textBox_WenDu.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox_WenDu.Name = "textBox_WenDu";
            this.textBox_WenDu.ReadOnly = true;
            this.textBox_WenDu.Size = new System.Drawing.Size(90, 28);
            this.textBox_WenDu.TabIndex = 131;
            // 
            // textBox_YaLi
            // 
            this.textBox_YaLi.Location = new System.Drawing.Point(1161, 421);
            this.textBox_YaLi.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox_YaLi.Name = "textBox_YaLi";
            this.textBox_YaLi.ReadOnly = true;
            this.textBox_YaLi.Size = new System.Drawing.Size(90, 28);
            this.textBox_YaLi.TabIndex = 130;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(644, 301);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(62, 18);
            this.label13.TabIndex = 132;
            this.label13.Text = "电流：";
            // 
            // textBox_electric
            // 
            this.textBox_electric.Location = new System.Drawing.Point(698, 295);
            this.textBox_electric.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.textBox_electric.Name = "textBox_electric";
            this.textBox_electric.ReadOnly = true;
            this.textBox_electric.Size = new System.Drawing.Size(77, 28);
            this.textBox_electric.TabIndex = 133;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(1109, 330);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(0, 18);
            this.label14.TabIndex = 134;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(1108, 333);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(44, 18);
            this.label15.TabIndex = 135;
            this.label15.Text = "电压";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(1108, 376);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(44, 18);
            this.label16.TabIndex = 136;
            this.label16.Text = "电流";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(1111, 467);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(44, 18);
            this.label17.TabIndex = 138;
            this.label17.Text = "温度";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(1109, 424);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(44, 18);
            this.label18.TabIndex = 137;
            this.label18.Text = "压力";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(1158, 301);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(80, 18);
            this.label19.TabIndex = 139;
            this.label19.Text = "AD采样值";
            // 
            // Form_Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.LightBlue;
            this.ClientSize = new System.Drawing.Size(1293, 691);
            this.Controls.Add(this.label19);
            this.Controls.Add(this.label17);
            this.Controls.Add(this.label18);
            this.Controls.Add(this.label16);
            this.Controls.Add(this.label15);
            this.Controls.Add(this.label14);
            this.Controls.Add(this.textBox_electric);
            this.Controls.Add(this.label13);
            this.Controls.Add(this.textBox_WenDu);
            this.Controls.Add(this.textBox_YaLi);
            this.Controls.Add(this.textBox_DianLiu);
            this.Controls.Add(this.textBox_DianYa);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.label12);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.leb_88);
            this.Controls.Add(this.listBox3);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.chart1);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.infoRichTextBox);
            this.Controls.Add(this.openComButton);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.SNComboBox);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.progressBar2);
            this.Controls.Add(this.text_lond);
            this.Controls.Add(this.textBox_pres);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.label_UnQualified);
            this.Controls.Add(this.label_Qualified);
            this.Controls.Add(this.button_openOrCloseCAN);
            this.Controls.Add(this.stop);
            this.Controls.Add(this.start);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.button_record);
            this.Controls.Add(this.button_manage);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.dataGridView_Param);
            this.Controls.Add(this.comboBox_Project);
            this.Controls.Add(this.label_project);
            this.Controls.Add(this.listBox2);
            this.Controls.Add(this.listBox1);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.button_result3);
            this.Controls.Add(this.button_result1);
            this.Controls.Add(this.button_result2);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.ForeColor = System.Drawing.Color.Black;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Form_Main";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "无人机200型直喷泵测试工装";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form_Main_FormClosed);
            this.Load += new System.EventHandler(this.Form_Main_Load);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Form_Main_MouseDown);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView_Param)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label_project;
        private System.Windows.Forms.ComboBox comboBox_Project;
        private System.Windows.Forms.Button button_manage;
        private System.Windows.Forms.DataGridView dataGridView_Param;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.ListBox listBox2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button start;
        private System.Windows.Forms.Button stop;
        private System.Windows.Forms.ListBox text_lond;
        private System.Windows.Forms.Button button_openOrCloseCAN;
        private System.Windows.Forms.Button button_record;
        private System.Windows.Forms.Label leb_88;
        private System.Windows.Forms.Label label_Qualified;
        private System.Windows.Forms.Label label_UnQualified;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox_pres;
        private System.Windows.Forms.Button button_result1;
        private System.Windows.Forms.Button button_result2;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ProgressBar progressBar2;
        private System.Windows.Forms.ComboBox SNComboBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button openComButton;
        private System.Windows.Forms.Timer time;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 设置ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 自动读秤ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 通讯设置ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 文档记录ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 参数设置ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 记录ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 帮助ToolStripMenuItem;
        private System.Windows.Forms.RichTextBox infoRichTextBox;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ListBox listBox3;
        private System.Windows.Forms.Button button_result3;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBox_DianYa;
        private System.Windows.Forms.TextBox textBox_DianLiu;
        private System.Windows.Forms.TextBox textBox_WenDu;
        private System.Windows.Forms.TextBox textBox_YaLi;
        private System.Windows.Forms.ToolStripMenuItem 标定ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 测试数据ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 退出标定ToolStripMenuItem;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox textBox_electric;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label19;
    }
}