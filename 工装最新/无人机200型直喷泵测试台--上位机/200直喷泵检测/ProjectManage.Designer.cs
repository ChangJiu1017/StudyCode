namespace _200直喷泵检测
{
    partial class ProjectManageForm
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.tabControl_Manage = new System.Windows.Forms.TabControl();
            this.tabPage_NewProject = new System.Windows.Forms.TabPage();
            this.label_projectExist = new System.Windows.Forms.Label();
            this.label_tipsName = new System.Windows.Forms.Label();
            this.label_tipsFile = new System.Windows.Forms.Label();
            this.button_OK = new System.Windows.Forms.Button();
            this.button_add = new System.Windows.Forms.Button();
            this.textBox_File = new System.Windows.Forms.TextBox();
            this.textBox_projectName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.tabPage_Remove = new System.Windows.Forms.TabPage();
            this.dataGridView_Manage = new System.Windows.Forms.DataGridView();
            this.tabPage_UseNumber = new System.Windows.Forms.TabPage();
            this.button_ClearUse = new System.Windows.Forms.Button();
            this.button_SetTotalUse = new System.Windows.Forms.Button();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.textBox_TotalUse = new System.Windows.Forms.TextBox();
            this.textBox_TotalUseMax = new System.Windows.Forms.TextBox();
            this.contextMenuStrip_Option = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.删除ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panel1.SuspendLayout();
            this.tabControl_Manage.SuspendLayout();
            this.tabPage_NewProject.SuspendLayout();
            this.tabPage_Remove.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView_Manage)).BeginInit();
            this.tabPage_UseNumber.SuspendLayout();
            this.contextMenuStrip_Option.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.tabControl_Manage);
            this.panel1.Location = new System.Drawing.Point(-1, 12);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(991, 663);
            this.panel1.TabIndex = 0;
            // 
            // tabControl_Manage
            // 
            this.tabControl_Manage.Controls.Add(this.tabPage_NewProject);
            this.tabControl_Manage.Controls.Add(this.tabPage_Remove);
            this.tabControl_Manage.Controls.Add(this.tabPage_UseNumber);
            this.tabControl_Manage.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tabControl_Manage.Location = new System.Drawing.Point(3, 3);
            this.tabControl_Manage.Name = "tabControl_Manage";
            this.tabControl_Manage.SelectedIndex = 0;
            this.tabControl_Manage.Size = new System.Drawing.Size(985, 660);
            this.tabControl_Manage.TabIndex = 0;
            this.tabControl_Manage.Click += new System.EventHandler(this.tabControl_Manage_Click);
            // 
            // tabPage_NewProject
            // 
            this.tabPage_NewProject.Controls.Add(this.label_projectExist);
            this.tabPage_NewProject.Controls.Add(this.label_tipsName);
            this.tabPage_NewProject.Controls.Add(this.label_tipsFile);
            this.tabPage_NewProject.Controls.Add(this.button_OK);
            this.tabPage_NewProject.Controls.Add(this.button_add);
            this.tabPage_NewProject.Controls.Add(this.textBox_File);
            this.tabPage_NewProject.Controls.Add(this.textBox_projectName);
            this.tabPage_NewProject.Controls.Add(this.label2);
            this.tabPage_NewProject.Controls.Add(this.label1);
            this.tabPage_NewProject.Location = new System.Drawing.Point(4, 30);
            this.tabPage_NewProject.Name = "tabPage_NewProject";
            this.tabPage_NewProject.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage_NewProject.Size = new System.Drawing.Size(977, 626);
            this.tabPage_NewProject.TabIndex = 1;
            this.tabPage_NewProject.Text = "新建项目";
            this.tabPage_NewProject.UseVisualStyleBackColor = true;
            // 
            // label_projectExist
            // 
            this.label_projectExist.Font = new System.Drawing.Font("楷体", 10.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_projectExist.ForeColor = System.Drawing.Color.Red;
            this.label_projectExist.Location = new System.Drawing.Point(619, 140);
            this.label_projectExist.Name = "label_projectExist";
            this.label_projectExist.Size = new System.Drawing.Size(208, 23);
            this.label_projectExist.TabIndex = 9;
            this.label_projectExist.Text = "该项目已经存在";
            // 
            // label_tipsName
            // 
            this.label_tipsName.Font = new System.Drawing.Font("楷体", 10.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_tipsName.ForeColor = System.Drawing.Color.Red;
            this.label_tipsName.Location = new System.Drawing.Point(619, 117);
            this.label_tipsName.Name = "label_tipsName";
            this.label_tipsName.Size = new System.Drawing.Size(208, 23);
            this.label_tipsName.TabIndex = 8;
            this.label_tipsName.Text = "项目名称不能为空";
            // 
            // label_tipsFile
            // 
            this.label_tipsFile.Font = new System.Drawing.Font("楷体", 10.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label_tipsFile.ForeColor = System.Drawing.Color.Red;
            this.label_tipsFile.Location = new System.Drawing.Point(184, 247);
            this.label_tipsFile.Name = "label_tipsFile";
            this.label_tipsFile.Size = new System.Drawing.Size(208, 23);
            this.label_tipsFile.TabIndex = 7;
            this.label_tipsFile.Text = "项目文件不能为空";
            // 
            // button_OK
            // 
            this.button_OK.Font = new System.Drawing.Font("楷体", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_OK.Location = new System.Drawing.Point(604, 380);
            this.button_OK.Name = "button_OK";
            this.button_OK.Size = new System.Drawing.Size(96, 34);
            this.button_OK.TabIndex = 6;
            this.button_OK.Text = "创建";
            this.button_OK.UseVisualStyleBackColor = true;
            this.button_OK.Click += new System.EventHandler(this.button_OK_Click);
            // 
            // button_add
            // 
            this.button_add.Font = new System.Drawing.Font("楷体", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_add.Location = new System.Drawing.Point(635, 200);
            this.button_add.Name = "button_add";
            this.button_add.Size = new System.Drawing.Size(96, 34);
            this.button_add.TabIndex = 5;
            this.button_add.Text = "添加";
            this.button_add.UseVisualStyleBackColor = true;
            this.button_add.Click += new System.EventHandler(this.button_add_Click);
            // 
            // textBox_File
            // 
            this.textBox_File.Location = new System.Drawing.Point(184, 200);
            this.textBox_File.Name = "textBox_File";
            this.textBox_File.Size = new System.Drawing.Size(429, 30);
            this.textBox_File.TabIndex = 4;
            // 
            // textBox_projectName
            // 
            this.textBox_projectName.Location = new System.Drawing.Point(184, 115);
            this.textBox_projectName.Name = "textBox_projectName";
            this.textBox_projectName.Size = new System.Drawing.Size(429, 30);
            this.textBox_projectName.TabIndex = 3;
            this.textBox_projectName.Leave += new System.EventHandler(this.textBox_projectName_Leave);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("楷体", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(64, 200);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(114, 20);
            this.label2.TabIndex = 2;
            this.label2.Text = "项目文件：";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("楷体", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(64, 115);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(114, 20);
            this.label1.TabIndex = 0;
            this.label1.Text = "项目名称：";
            // 
            // tabPage_Remove
            // 
            this.tabPage_Remove.Controls.Add(this.dataGridView_Manage);
            this.tabPage_Remove.Location = new System.Drawing.Point(4, 30);
            this.tabPage_Remove.Name = "tabPage_Remove";
            this.tabPage_Remove.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage_Remove.Size = new System.Drawing.Size(977, 626);
            this.tabPage_Remove.TabIndex = 0;
            this.tabPage_Remove.Text = "移除项目";
            this.tabPage_Remove.UseVisualStyleBackColor = true;
            // 
            // dataGridView_Manage
            // 
            this.dataGridView_Manage.BackgroundColor = System.Drawing.SystemColors.ButtonHighlight;
            this.dataGridView_Manage.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView_Manage.Location = new System.Drawing.Point(0, 0);
            this.dataGridView_Manage.Name = "dataGridView_Manage";
            this.dataGridView_Manage.RowHeadersVisible = false;
            this.dataGridView_Manage.RowHeadersWidth = 51;
            this.dataGridView_Manage.RowTemplate.Height = 27;
            this.dataGridView_Manage.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
            this.dataGridView_Manage.Size = new System.Drawing.Size(952, 620);
            this.dataGridView_Manage.TabIndex = 0;
            this.dataGridView_Manage.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView_Manage_CellContentClick);
            this.dataGridView_Manage.CellMouseClick += new System.Windows.Forms.DataGridViewCellMouseEventHandler(this.dataGridView_Manage_CellMouseClick);
            // 
            // tabPage_UseNumber
            // 
            this.tabPage_UseNumber.Controls.Add(this.button_ClearUse);
            this.tabPage_UseNumber.Controls.Add(this.button_SetTotalUse);
            this.tabPage_UseNumber.Controls.Add(this.label6);
            this.tabPage_UseNumber.Controls.Add(this.label5);
            this.tabPage_UseNumber.Controls.Add(this.textBox_TotalUse);
            this.tabPage_UseNumber.Controls.Add(this.textBox_TotalUseMax);
            this.tabPage_UseNumber.Location = new System.Drawing.Point(4, 30);
            this.tabPage_UseNumber.Name = "tabPage_UseNumber";
            this.tabPage_UseNumber.Size = new System.Drawing.Size(977, 626);
            this.tabPage_UseNumber.TabIndex = 2;
            this.tabPage_UseNumber.Text = "累计使用";
            this.tabPage_UseNumber.UseVisualStyleBackColor = true;
            // 
            // button_ClearUse
            // 
            this.button_ClearUse.Location = new System.Drawing.Point(610, 208);
            this.button_ClearUse.Name = "button_ClearUse";
            this.button_ClearUse.Size = new System.Drawing.Size(86, 30);
            this.button_ClearUse.TabIndex = 5;
            this.button_ClearUse.Text = "清零";
            this.button_ClearUse.UseVisualStyleBackColor = true;
            this.button_ClearUse.Click += new System.EventHandler(this.button_ClearUse_Click);
            // 
            // button_SetTotalUse
            // 
            this.button_SetTotalUse.Location = new System.Drawing.Point(610, 111);
            this.button_SetTotalUse.Name = "button_SetTotalUse";
            this.button_SetTotalUse.Size = new System.Drawing.Size(86, 30);
            this.button_SetTotalUse.TabIndex = 4;
            this.button_SetTotalUse.Text = "设定";
            this.button_SetTotalUse.UseVisualStyleBackColor = true;
            this.button_SetTotalUse.Click += new System.EventHandler(this.button_SetTotalUse_Click);
            // 
            // label6
            // 
            this.label6.Font = new System.Drawing.Font("楷体", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label6.Location = new System.Drawing.Point(27, 213);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(177, 25);
            this.label6.TabIndex = 3;
            this.label6.Text = "当前累计使用：";
            // 
            // label5
            // 
            this.label5.Font = new System.Drawing.Font("楷体", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(27, 111);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(208, 25);
            this.label5.TabIndex = 2;
            this.label5.Text = "累计可使用上限：";
            // 
            // textBox_TotalUse
            // 
            this.textBox_TotalUse.Location = new System.Drawing.Point(241, 208);
            this.textBox_TotalUse.Name = "textBox_TotalUse";
            this.textBox_TotalUse.Size = new System.Drawing.Size(339, 30);
            this.textBox_TotalUse.TabIndex = 1;
            this.textBox_TotalUse.TextChanged += new System.EventHandler(this.button_add_Click);
            this.textBox_TotalUse.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox_TotalUse_KeyPress);
            // 
            // textBox_TotalUseMax
            // 
            this.textBox_TotalUseMax.Location = new System.Drawing.Point(241, 111);
            this.textBox_TotalUseMax.Name = "textBox_TotalUseMax";
            this.textBox_TotalUseMax.Size = new System.Drawing.Size(339, 30);
            this.textBox_TotalUseMax.TabIndex = 0;
            this.textBox_TotalUseMax.TextChanged += new System.EventHandler(this.textBox_TotalUseMax_TextChanged);
            this.textBox_TotalUseMax.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox_TotalUseMax_KeyPress);
            // 
            // contextMenuStrip_Option
            // 
            this.contextMenuStrip_Option.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.contextMenuStrip_Option.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.删除ToolStripMenuItem});
            this.contextMenuStrip_Option.Name = "contextMenuStrip_Option";
            this.contextMenuStrip_Option.ShowCheckMargin = true;
            this.contextMenuStrip_Option.Size = new System.Drawing.Size(131, 28);
            this.contextMenuStrip_Option.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuStrip_Option_Opening);
            // 
            // 删除ToolStripMenuItem
            // 
            this.删除ToolStripMenuItem.Name = "删除ToolStripMenuItem";
            this.删除ToolStripMenuItem.Size = new System.Drawing.Size(232, 24);
            this.删除ToolStripMenuItem.Text = "删除";
            this.删除ToolStripMenuItem.Click += new System.EventHandler(this.删除ToolStripMenuItem_Click);
            // 
            // ProjectManageForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1005, 689);
            this.Controls.Add(this.panel1);
            this.Name = "ProjectManageForm";
            this.Text = "设置";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ProjectManageForm_FormClosed);
            this.Load += new System.EventHandler(this.ProjectManageForm_Load);
            this.panel1.ResumeLayout(false);
            this.tabControl_Manage.ResumeLayout(false);
            this.tabPage_NewProject.ResumeLayout(false);
            this.tabPage_NewProject.PerformLayout();
            this.tabPage_Remove.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView_Manage)).EndInit();
            this.tabPage_UseNumber.ResumeLayout(false);
            this.tabPage_UseNumber.PerformLayout();
            this.contextMenuStrip_Option.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TabControl tabControl_Manage;
        private System.Windows.Forms.TabPage tabPage_NewProject;
        private System.Windows.Forms.TabPage tabPage_Remove;
        private System.Windows.Forms.TabPage tabPage_UseNumber;
        private System.Windows.Forms.TextBox textBox_File;
        private System.Windows.Forms.TextBox textBox_projectName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBox_TotalUse;
        private System.Windows.Forms.TextBox textBox_TotalUseMax;
        private System.Windows.Forms.Button button_OK;
        private System.Windows.Forms.Button button_add;
        private System.Windows.Forms.Label label_tipsFile;
        private System.Windows.Forms.Label label_projectExist;
        private System.Windows.Forms.Label label_tipsName;
        private System.Windows.Forms.DataGridView dataGridView_Manage;
        private System.Windows.Forms.Button button_ClearUse;
        private System.Windows.Forms.Button button_SetTotalUse;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip_Option;
        private System.Windows.Forms.ToolStripMenuItem 删除ToolStripMenuItem;
    }
}