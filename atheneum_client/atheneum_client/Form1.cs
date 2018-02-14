using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;

namespace atheneum_client
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            try
            {

                TcpClient client = new TcpClient(settings.addr, settings.port);
                settings.client = client;
                client.Client.Send(Encoding.ASCII.GetBytes("0 /" + settings.usr + " /" + settings.pas));

                byte[] resp = new byte[500];

                client.Client.Receive(resp);

                if (resp[0] != '1') {
                    MessageBox.Show("Incorrect password");
                    Application.Exit();
                }
                   

            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);

            }
          


        }

        void add_header(ListView lv,string[] data)
        {
            lv.Columns.Clear();
            data[data.Length-1] = "";
            foreach (string str in data)
                lv.Columns.Add(str, 100);
        }

        void add_row(ListView lv, string[] data)
        {
            ListViewItem item = new ListViewItem();
            item.Text = data[0];
            for (int i = 1; i < data.Length; i++) {
                item.SubItems.Add(data[i]);
            }
            lv.Items.Add(item);
        }

        void parse_and_assign(ListView lv1,string data,bool header)
        {
            string sTmp = "";
            List<string> list = new List<string>();

            for (int i = 0; i < data.Length; i++)
            {
                if (data[i] == '{') {
                    i++;
                    while (data[i] != '}')
                    {
                        sTmp += data[i];
                        i++;
                    }
                    list.Add(sTmp);
                    sTmp = "";
                }
                else
                {
                    if (data[i] != ',')
                    {
                        if (data[i] != ';')
                            sTmp += data[i];
                        else
                        {
                            if (header)
                            {
                                add_header(lv1, list.ToArray());
                                break;
                            }
                            else
                            {

                                add_row(lv1, list.ToArray());
                                list.Clear();
                            }
                        }
                    }
                    else
                    {
                        list.Add(sTmp);
                        sTmp = "";
                    }
                }

            }
              
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void button2_Click(object sender, EventArgs e)
        {

            settings.client.Client.Send(Encoding.ASCII.GetBytes("3"));

            byte[] by = new byte[50000];

            settings.client.Client.Receive(by);

            textBox1.Text = Encoding.ASCII.GetString(by).Replace("\n","\r\n");
           
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            try {
                settings.client.Client.Send(Encoding.ASCII.GetBytes("DIE"));
            }
            catch { }
        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            
        }

        private void listView1_Enter(object sender, EventArgs e)
        {
        }

        private void listView1_TabIndexChanged(object sender, EventArgs e)
        {
        }

        private void refreshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            settings.client.Client.Send(Encoding.ASCII.GetBytes("10"));
            byte[] bytes = new byte[50000];
            settings.client.Client.Receive(bytes);
            add_header(listView1, Encoding.ASCII.GetString(bytes).Split('|')[0].Split(','));
            listView1.Items.Clear();
            parse_and_assign(listView1, Encoding.ASCII.GetString(bytes).Split('|')[1], false);

            /*
            add_header(listView1, Encoding.ASCII.GetString(bytes).Split('|')[0].Split(','));
            listView1.Items.Clear();

            foreach (string ar in Encoding.ASCII.GetString(bytes).Split('|')[1].Split(';'))
                add_row(listView1, ar.Split(','));
            */
            settings.client.Client.Send(Encoding.ASCII.GetBytes("11"));
            bytes = new byte[50000];
            settings.client.Client.Receive(bytes);


            add_header(listView2, Encoding.ASCII.GetString(bytes).Split('|')[0].Split(','));
            listView2.Items.Clear();

            parse_and_assign(listView2, Encoding.ASCII.GetString(bytes).Split('|')[1], false);

            /*
            add_header(listView2, Encoding.ASCII.GetString(bytes).Split('|')[0].Split(','));

            listView2.Items.Clear();

            foreach (string ar in Encoding.ASCII.GetString(bytes).Split('|')[1].Split(';'))
                add_row(listView2, ar.Split(','));
            */
        }

        private void helpToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Use the tab navigtors to select functionality\nPress 'Refresh' in order to reload results", "HELP", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string type;
            string id; 
            if (checkBox1.Checked)
                type = "Teacher";
            else
                type = "Student";

            if (checkBox2.Checked)
                id = textBox4.Text;
            else
                id = "0";

            settings.client.Client.Send(Encoding.ASCII.GetBytes("2 /" + id + " /" + textBox2.Text + "," + textBox3.Text + "," + type + ","+"{},"));
            
            textBox2.Text = "";
            textBox3.Text = "";
        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            settings.client.Client.Send(Encoding.ASCII.GetBytes("4 /"+textBox5.Text + " /" + textBox6.Text));
            byte[] data = new byte[1];
            settings.client.Client.Receive(data);
            if (data[0] != '1')
            {
                MessageBox.Show("Book already issued");
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            settings.client.Client.Send(Encoding.ASCII.GetBytes("5 /" + textBox8.Text + " /" + textBox7.Text));
            byte[] data = new byte[1];
            settings.client.Client.Receive(data);
            if (data[0] != '1')
            {
                MessageBox.Show("Book already returned");
            }
        }
    }
    
}
