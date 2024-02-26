namespace _200直喷泵检测
{
    partial class FormRecord
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
            this.cboYears = new System.Windows.Forms.ComboBox();
            this.cboMonth = new System.Windows.Forms.ComboBox();
            this.cboDays = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.button_daochu = new System.Windows.Forms.Button();
            this.button_Found = new System.Windows.Forms.Button();
            this.button_empty = new System.Windows.Forms.Button();
            this.panel_Main = new System.Windows.Forms.Panel();
            this.cboState = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.dataGridView_Record = new System.Windows.Forms.DataGridView();
            this.saveFileDialog_RecordCSV = new System.Windows.Forms.SaveFileDialog();
            this.panel_Main.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView_Record)).BeginInit();
            this.SuspendLayout();
            // 
            // cboYears
            // 
            this.cboYears.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboYears.FormattingEnabled = true;
            this.cboYears.Location = new System.Drawing.Point(117, 44);
            this.cboYears.Name = "cboYears";
            this.cboYears.Size = new System.Drawing.Size(121, 23);
            this.cboYears.TabIndex = 0;
            this.cboYears.SelectedIndexChanged += new System.EventHandler(this.cboYears_SelectedIndexChanged);
            // 
            // cboMonth
            // 
            this.cboMonth.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboMonth.FormattingEnabled = true;
            this.cboMonth.Location = new System.Drawing.Point(368, 44);
            this.cboMonth.Name = "cboMonth";
            this.cboMonth.Size = new System.Drawing.Size(121, 23);
            this.cboMonth.TabIndex = 1;
            this.cboMonth.SelectedIndexChanged += new System.EventHandler(this.cboMonth_SelectedIndexChanged);
            // 
            // cboDays
            // 
            this.cboDays.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboDays.FormattingEnabled = true;
            this.cboDays.Location = new System.Drawing.Point(588, 48);
            this.cboDays.Name = "cboDays";
            this.cboDays.Size = new System.Drawing.Size(121, 23);
            this.cboDays.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(32, 44);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 20);
            this.label1.TabIndex = 3;
            this.label1.Text = "年份:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(277, 44);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(59, 20);
            this.label2.TabIndex = 4;
            this.label2.Text = "月份:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.Location = new System.Drawing.Point(526, 48);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(39, 20);
            this.label3.TabIndex = 5;
            this.label3.Text = "日:";
            // 
            // button_daochu
            // 
            this.button_daochu.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_daochu.Location = new System.Drawing.Point(1044, 28);
            this.button_daochu.Name = "button_daochu";
            this.button_daochu.Size = new System.Drawing.Size(104, 43);
            this.button_daochu.TabIndex = 6;
            this.button_daochu.Text = "导出文件";
            this.button_daochu.UseVisualStyleBackColor = true;
            this.button_daochu.Click += new System.EventHandler(this.button_daochu_Click);
            // 
            // button_Found
            // 
            this.button_Found.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_Found.Location = new System.Drawing.Point(464, 86);
            this.button_Found.Name = "button_Found";
            this.button_Found.Size = new System.Drawing.Size(90, 43);
            this.button_Found.TabIndex = 7;
            this.button_Found.Text = "查找";
            this.button_Found.UseVisualStyleBackColor = true;
            this.button_Found.Click += new System.EventHandler(this.button_Found_Click);
            // 
            // button_empty
            // 
            this.button_empty.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.button_empty.Location = new System.Drawing.Point(858, 25);
            this.button_empty.Name = "button_empty";
            this.button_empty.Size = new System.Drawing.Size(100, 43);
            this.button_empty.TabIndex = 8;
            this.button_empty.Text = "清空筛选";
            this.button_empty.UseVisualStyleBackColor = true;
            this.button_empty.Click += new System.EventHandler(this.button_empty_Click);
            // 
            // panel_Main
            // 
            this.panel_Main.Controls.Add(this.cboState);
            this.panel_Main.Controls.Add(this.label4);
            this.panel_Main.Controls.Add(this.dataGridView_Record);
            this.panel_Main.Controls.Add(this.label1);
            this.panel_Main.Controls.Add(this.button_daochu);
            this.panel_Main.Controls.Add(this.button_empty);
            this.panel_Main.Controls.Add(this.cboYears);
            this.panel_Main.Controls.Add(this.button_Found);
            this.panel_Main.Controls.Add(this.label2);
            this.panel_Main.Controls.Add(this.cboMonth);
            this.panel_Main.Controls.Add(this.cboDays);
            this.panel_Main.Controls.Add(this.label3);
            this.panel_Main.Location = new System.Drawing.Point(5, 3);
            this.panel_Main.Name = "panel_Main";
            this.panel_Main.Size = new System.Drawing.Size(1200, 700);
            this.panel_Main.TabIndex = 9;
            this.panel_Main.Paint += new System.Windows.Forms.PaintEventHandler(this.panel_Main_Paint);
            // 
            // cboState
            // 
            this.cboState.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboState.FormattingEnabled = true;
            this.cboState.Location = new System.Drawing.Point(161, 97);
            this.cboState.Name = "cboState";
            this.cboState.Size = new System.Drawing.Size(121, 23);
            this.cboState.TabIndex = 11;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.Location = new System.Drawing.Point(32, 97);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(99, 20);
            this.label4.TabIndex = 10;
            this.label4.Text = "检测结果:";
            // 
            // dataGridView_Record
            // 
            this.dataGridView_Record.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView_Record.Location = new System.Drawing.Point(36, 144);
            this.dataGridView_Record.Name = "dataGridView_Record";
            this.dataGridView_Record.RowHeadersWidth = 51;
            this.dataGridView_Record.RowTemplate.Height = 27;
            this.dataGridView_Record.Size = new System.Drawing.Size(1014, 499);
            this.dataGridView_Record.TabIndex = 9;
            this.dataGridView_Record.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView_Record_CellContentClick);
            // 
            // FormRecord
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1206, 703);
            this.Controls.Add(this.panel_Main);
            this.Name = "FormRecord";
            this.Text = "记录";
            this.Load += new System.EventHandler(this.FormRecord_Load);
            this.panel_Main.ResumeLayout(false);
            this.panel_Main.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView_Record)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox cboYears;
        private System.Windows.Forms.ComboBox cboMonth;
        private System.Windows.Forms.ComboBox cboDays;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button button_daochu;
        private System.Windows.Forms.Button button_Found;
        private System.Windows.Forms.Button button_empty;
        private System.Windows.Forms.Panel panel_Main;
        private System.Windows.Forms.DataGridView dataGridView_Record;
        private System.Windows.Forms.SaveFileDialog saveFileDialog_RecordCSV;
        private System.Windows.Forms.ComboBox cboState;
        private System.Windows.Forms.Label label4;
    }
}