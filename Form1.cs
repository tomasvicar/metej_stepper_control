using System.IO.Ports;
using System.Windows.Forms;
using static System.Windows.Forms.DataFormats;

namespace metej_stepper_control
{
    public partial class Form1 : Form
    {
        private SerialPort serialPort;

        public Form1()
        {
            InitializeComponent();
            openPumpSerial();
        }

        private void label1_Click(object sender, EventArgs e)
        {
            openPumpSerial();
        }


        public void openPumpSerial()
        {
            this.label_COM.Text = "Connecting";
            this.label_COM.ForeColor = System.Drawing.Color.Gray;
            this.label_COM.Refresh();

            var ArrayComPortsNames = SerialPort.GetPortNames();



            if (!(serialPort == null))
            {
                if (serialPort.IsOpen)
                {
                    serialPort.Close();
                    Thread.Sleep(3000);
                }

            }



            foreach (string port in ArrayComPortsNames)
            {





                try
                {



                    Console.WriteLine(port);
                    serialPort = new SerialPort(port, 9600);



                    serialPort.Open();


                    serialPort.WriteTimeout = 500;
                    serialPort.ReadTimeout = 2000;

                    serialPort.WriteLine("ok?");



                    string message = serialPort.ReadLine();
                    Console.WriteLine(message);

                    if (message.Trim() == "ok")
                    {
                        this.label_COM.Text = port;

                        this.label_COM.ForeColor = System.Drawing.Color.Green;

                        serialPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);



                    }
                    else
                    {
                        serialPort.Close();

                    }

                }
                catch (TimeoutException)
                {
                    Console.WriteLine("time out");
                    serialPort.Close();
                }
                catch (Exception ex)
                {

                    Console.WriteLine(ex.ToString());
                    serialPort.Close();
                }
            }

            if (this.label_COM.Text == "Connecting")
            {
                Console.WriteLine("COM step 10");


                this.label_COM.Text = "NA";

                this.label_COM.ForeColor = System.Drawing.Color.Red;
            }

            Console.WriteLine("COM step 11");


        }
        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                SerialPort sp = (SerialPort)sender;
                string indata = sp.ReadLine();

                Console.WriteLine(indata);



                string to_write = DateTime.Now.ToString("HH:mm:ss.fff");

                this.listBox_comunication.BeginInvoke((MethodInvoker)delegate
                {
                    this.listBox_comunication.BeginUpdate();
                    this.listBox_comunication.Items.Add(to_write + ": " + indata);
                    if (this.listBox_comunication.Items.Count > 10)
                    {
                        this.listBox_comunication.Items.RemoveAt(0);
                    }
                    this.listBox_comunication.EndUpdate();
                });

            }
            catch
            {

            }

        }




        private void button1_Click(object sender, EventArgs e)
        {
            serialPort.WriteLine(button1.Text);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            serialPort.WriteLine(button2.Text);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            serialPort.WriteLine(button3.Text);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            serialPort.WriteLine(button4.Text);
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            serialPort.WriteLine("t" + ((int)numericUpDown1.Value).ToString());
        }

        private void button8_Click(object sender, EventArgs e)
        {
            serialPort.WriteLine("r");
        }

        private void button7_Click(object sender, EventArgs e)
        {
            serialPort.WriteLine("l");
        }

        private void button6_Click(object sender, EventArgs e)
        {
            serialPort.WriteLine("s");
        }

        private void button5_Click(object sender, EventArgs e)
        {
            serialPort.WriteLine("d");
        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {
            serialPort.WriteLine("q" + ((int)numericUpDown1.Value).ToString());
        }
    }
}