using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Security;

namespace atheneum_client
{
    public partial class startup : Form
    {
        public startup()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {

            settings.addr = textBox1.Text;
            settings.port = int.Parse(textBox2.Text);
            settings.usr = textBox3.Text;
            settings.pas = textBox4.Text;
            
            this.Close();
        }
    }
}
