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
    public partial class Form_Hint : Form
    {

        Form_Main parentForm;
        public Form_Hint(Form_Main _parentForm)
        {
            this.parentForm = _parentForm;
            InitializeComponent();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
