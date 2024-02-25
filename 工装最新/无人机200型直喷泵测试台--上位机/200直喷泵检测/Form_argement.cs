using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace _200直喷泵检测
{
    public partial class Form_argement : Form
    {
        Form_Main parentForm;
        public Form_argement(Form_Main _parentForm)
        {
            this.parentForm = _parentForm;
            InitializeComponent();
            

        }

        private void Form_argement_Load(object sender, EventArgs e)
        {
            textBox1.Text = MYSettings.Default.TIME_T1; //回位时间
            textBox2.Text = MYSettings.Default.TIME_T2; //做工时间        
            textBox3.Text = MYSettings.Default.TIEM_T; //喷射频率
            textBox4.Text = MYSettings.Default.Cycle; //喷射周期
            textBox5.Text = MYSettings.Default.Spray_time; //喷射次数
            
        }

        private void button_save_Click(object sender, EventArgs e)
        {
            MYSettings.Default.TIME_T1 = textBox1.Text.ToString();  //回位时间  
            MYSettings.Default.TIME_T2 = textBox2.Text.ToString();  //做工时间        
            MYSettings.Default.TIEM_T  = textBox3.Text.ToString();  //喷射频率
            MYSettings.Default.Cycle =  textBox4.Text.ToString(); //喷射周期
            MYSettings.Default.Spray_time = textBox5.Text.ToString();  //喷射次数
            MYSettings.Default.Save();//保存数据
            MessageBox.Show("保存成功！");
        }

        private void button_canal_Click(object sender, EventArgs e)
        {
            
            
            Form_argement_Load(sender, e);
            MessageBox.Show("已取消当前操作！");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            textBox1.Text = "21"; //回位时间
            textBox2.Text = "61"; //做工时间        
            textBox3.Text = "100"; //喷射频率
            textBox4.Text = "24"; //喷射周期
            textBox5.Text = "1000"; //喷射次数
            MessageBox.Show("恢复默认值，请再次点击保存");
        }
    }
}
