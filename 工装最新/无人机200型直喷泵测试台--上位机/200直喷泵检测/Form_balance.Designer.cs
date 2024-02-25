namespace _200直喷泵检测
{
    partial class Form_balance
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
            this.label1 = new System.Windows.Forms.Label();
            this.ComDispTextBox = new System.Windows.Forms.TextBox();
            this.scaleGroupBox = new System.Windows.Forms.GroupBox();
            this.scaleBaudBox = new System.Windows.Forms.ComboBox();
            this.scaleComBox = new System.Windows.Forms.ComboBox();
            this.scaleBaudLabel = new System.Windows.Forms.Label();
            this.scaleComLabel = new System.Windows.Forms.Label();
            this.saveButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.scaleGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(34, 32);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(109, 20);
            this.label1.TabIndex = 0;
            this.label1.Text = "可用串口：";
            // 
            // ComDispTextBox
            // 
            this.ComDispTextBox.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.ComDispTextBox.Location = new System.Drawing.Point(140, 29);
            this.ComDispTextBox.Name = "ComDispTextBox";
            this.ComDispTextBox.Size = new System.Drawing.Size(201, 30);
            this.ComDispTextBox.TabIndex = 1;
            this.ComDispTextBox.TabStop = false;
            // 
            // scaleGroupBox
            // 
            this.scaleGroupBox.Controls.Add(this.scaleBaudBox);
            this.scaleGroupBox.Controls.Add(this.scaleComBox);
            this.scaleGroupBox.Controls.Add(this.scaleBaudLabel);
            this.scaleGroupBox.Controls.Add(this.scaleComLabel);
            this.scaleGroupBox.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.scaleGroupBox.Location = new System.Drawing.Point(46, 77);
            this.scaleGroupBox.Name = "scaleGroupBox";
            this.scaleGroupBox.Size = new System.Drawing.Size(385, 223);
            this.scaleGroupBox.TabIndex = 2;
            this.scaleGroupBox.TabStop = false;
            this.scaleGroupBox.Text = "电子秤";
            // 
            // scaleBaudBox
            // 
            this.scaleBaudBox.FormattingEnabled = true;
            this.scaleBaudBox.Items.AddRange(new object[] {
            "600",
            "1200",
            "9600"});
            this.scaleBaudBox.Location = new System.Drawing.Point(115, 108);
            this.scaleBaudBox.Name = "scaleBaudBox";
            this.scaleBaudBox.Size = new System.Drawing.Size(121, 28);
            this.scaleBaudBox.TabIndex = 3;
            this.scaleBaudBox.Text = "9600";
            // 
            // scaleComBox
            // 
            this.scaleComBox.FormattingEnabled = true;
            this.scaleComBox.Items.AddRange(new object[] {
            "COM1",
            "COM2",
            "COM3",
            "COM4",
            "COM5"});
            this.scaleComBox.Location = new System.Drawing.Point(115, 33);
            this.scaleComBox.Name = "scaleComBox";
            this.scaleComBox.Size = new System.Drawing.Size(121, 28);
            this.scaleComBox.TabIndex = 1;
            this.scaleComBox.TabStop = false;
            this.scaleComBox.Text = "COM3";
            // 
            // scaleBaudLabel
            // 
            this.scaleBaudLabel.AutoSize = true;
            this.scaleBaudLabel.Location = new System.Drawing.Point(29, 108);
            this.scaleBaudLabel.Name = "scaleBaudLabel";
            this.scaleBaudLabel.Size = new System.Drawing.Size(89, 20);
            this.scaleBaudLabel.TabIndex = 1;
            this.scaleBaudLabel.Text = "波特率：";
            // 
            // scaleComLabel
            // 
            this.scaleComLabel.AutoSize = true;
            this.scaleComLabel.Location = new System.Drawing.Point(29, 42);
            this.scaleComLabel.Name = "scaleComLabel";
            this.scaleComLabel.Size = new System.Drawing.Size(89, 20);
            this.scaleComLabel.TabIndex = 0;
            this.scaleComLabel.Text = "串口号：";
            // 
            // saveButton
            // 
            this.saveButton.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.saveButton.Location = new System.Drawing.Point(150, 306);
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(84, 34);
            this.saveButton.TabIndex = 3;
            this.saveButton.Text = "保存";
            this.saveButton.UseVisualStyleBackColor = true;
            this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // cancelButton
            // 
            this.cancelButton.Font = new System.Drawing.Font("宋体", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.cancelButton.Location = new System.Drawing.Point(282, 306);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(78, 34);
            this.cancelButton.TabIndex = 4;
            this.cancelButton.Text = "取消";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // Form_balance
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(522, 386);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.saveButton);
            this.Controls.Add(this.scaleGroupBox);
            this.Controls.Add(this.ComDispTextBox);
            this.Controls.Add(this.label1);
            this.Name = "Form_balance";
            this.Text = "通讯设置";
            this.Load += new System.EventHandler(this.Form_balance_Load);
            this.scaleGroupBox.ResumeLayout(false);
            this.scaleGroupBox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox ComDispTextBox;
        private System.Windows.Forms.GroupBox scaleGroupBox;
        private System.Windows.Forms.ComboBox scaleBaudBox;
        private System.Windows.Forms.ComboBox scaleComBox;
        private System.Windows.Forms.Label scaleBaudLabel;
        private System.Windows.Forms.Label scaleComLabel;
        private System.Windows.Forms.Button saveButton;
        private System.Windows.Forms.Button cancelButton;
    }
}