/*
 * Created by SharpDevelop.
 * User: nowind
 * Date: 2013/3/7
 * Time: 10:14
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Runtime.InteropServices; 
using System.Management;
using System.Collections;
using System.IO;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.Net.Sockets;
namespace mslogin
{
	/// <summary>
	/// Description of MainForm.
	/// </summary>
	public partial class MainForm : Form
	{
        [DllImport("winredirip.dll", EntryPoint = "DllInit", CharSet = CharSet.Ansi, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        private static  extern UInt32 DllInit();
        [DllImport("winredirip.dll", EntryPoint = "redirIP",  CharSet =CharSet.Ansi, ExactSpelling=true,CallingConvention = CallingConvention.StdCall)]
		private static  extern UInt32 redirIp(string devname,string orip, string redip , byte protocol ,
		                      UInt16 port );
        [DllImport("winredirip.dll", EntryPoint = "Free", ExactSpelling = true,SetLastError=true, CallingConvention = CallingConvention.StdCall)]
		private static extern UInt32 Free( byte closelib);
        [DllImport("kernel32.dll", EntryPoint = "GetLastError")]
        private static extern UInt32 GetLastError();
		[DllImport("kernel32.dll",EntryPoint="GetPrivateProfileStringA")]
        private static extern int GetPrivateProfileString(string section, string key, 
                                                          string def, StringBuilder retVal, 
                                                          int size, string filePath);
		[DllImport("kernel32.dll",EntryPoint="WritePrivateProfileStringA")]
        private static extern long WritePrivateProfileString(string section, string key, 
                                                             string val, string filePath);

    //    private delegate UInt32 BeginCall();
        private delegate void pStatusLabSet(string i);

        pStatusLabSet StatusLableSet;
    //    EndCall pEndCall;
    //    

      //  Thread mainthread;
     //   static  UInt32 mainworkret;


		 LangString ls;
         string filepath="";
        Boolean Opened = false;
        Boolean Worked = false;
        string lastip = "";
		Dictionary<String, String> locamap=new Dictionary<string, string>()
		{
			{"Chinese","221.231.130.70"}
		};
		public MainForm()
		{
			//
			// The InitializeComponent() call is required for Windows Forms designer support.
			//
			InitializeComponent();
			try{
				ls=new LangString("zh");}
			catch
			{
					MessageBox.Show("Lang File not exist or open error.");
			}
			if(ls!=null)
			{
			Text=ls.get("title");
			UI1.Text=ls.get("UI1");
			UI2.Text=ls.get("UI2");
			UI3.Text=ls.get("UI3");
            SetPath.Text = ls.get("UI4");
            Check.Text = ls.get("UI5");
			Commit.Text=ls.get("Commit");
			StatusLabel.Text=ls.get("StatusLabel_1");
			}
			localeBox.SelectedIndex=0;
          //  pBeginCall = dBeginCall;
          //  pEndCall = dEndCall;
			
		}
        void StatusLabSet(string i)
        {
            StatusLabel.Text = i;
        }
		void MainFormLoad(object sender, EventArgs e)
		{	
		if(ls==null)Close();
        try
        {
            DllInit();
        }
        catch 
        {
            MessageBox.Show(ls.get("msg/m1"));
            Close();
        }
		StringBuilder sb=new StringBuilder(100);
		GetPrivateProfileString("conf","ip","",sb,100,"./conf.ini");
		string ip=sb.ToString();
		GetPrivateProfileString("conf","loc","",sb,100,"./conf.ini");
		string loc=sb.ToString();
		GetPrivateProfileString("conf","dev","",sb,100,"./conf.ini");
		string dev=sb.ToString();
        GetPrivateProfileString("conf", "path", "", sb, 100, "./conf.ini");
        filepath = sb.ToString();
        GetPrivateProfileString("conf", "port", "8484", sb, 100, "./conf.ini");
        portBox.Text = sb.ToString();
		int select=0;
		ManagementClass   mc   =   new   ManagementClass( "Win32_NetworkAdapterConfiguration"); 
		ManagementObjectCollection   moc   =   mc.GetInstances(); 
		foreach(ManagementObject   mo   in   moc)   {
			string[] ips=(string [])(mo["IPAddress"]);
			if (ips!=null)
			{
                if (!AdaptorcomboBox.Items.Contains(ips[0]))
				AdaptorcomboBox.Items.Add(ips[0]);
			}
			
		}
		AdaptorcomboBox.Items.Add("\\DEVICE\\NDISWANIP");
		for (int i=0;i<AdaptorcomboBox.Items.Count;i++)
			if(((string)AdaptorcomboBox.Items[i]).Equals(dev))select=i;
		AdaptorcomboBox.SelectedIndex=select;
		ipBox.Text=ip;
        StatusLableSet=StatusLabSet;
        Opened = true;
		}
		
	/*	UInt32 dBeginCall()
        {
            //return 1;
            uint r;
            StringBuilder sb = new StringBuilder(500);
            sb.Append(AdaptorcomboBox.Text);
            string cDev = sb.ToString();
            sb.Remove(0, sb.Length);
            sb.Append(locamap[localeBox.Text]);
            string corIp = sb.ToString();
            sb.Remove(0, sb.Length);
            sb.Append(ipBox.Text);
            string creip = sb.ToString();
            r=redirIp(cDev, corIp, creip, (byte)1, (UInt16)0);
            return r;
        }
        UInt32 dEndCall(int i)
        {
            return Free((byte)i);
        }*/
		void StatusStripItemClicked(object sender, ToolStripItemClickedEventArgs e)
		{
			
		}
		
		void MainFormLayout(object sender, LayoutEventArgs e)
		{

		}
      /*  void  MainWork(object para)
        {
            object[] data = (object [])para;
            Worked = true;
            UInt32 ret = redirIp((string)data[0], (string)data[1], (string)data[2], (byte)1, (UInt16)0);
            if (ret != 0)
            {
                Worked = false;
                MessageBox.Show(ret.ToString() + ":" + ls.get("msg/m" + ret));
                 this.Invoke(StatusLableSet,new object []{ls.get("StatusLabel_3")});
                return;
            }

        }*/
        string[] CheckServer(string testip,ushort testport)
        {
            byte[] buff=new byte[100];
            int tmp=0;
            Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            sock.ReceiveTimeout = sock.SendTimeout = 1000;
            try { 
            sock.Connect(testip,(int) testport);
           tmp= sock.Receive(buff, 100, SocketFlags.None);
            }
            catch
            {
                return null;
            }


            if (tmp < 10) return null;
            string[] ret = new string[3];
            tmp = buff[2];
            tmp += (int)buff[3] << 8;
            ret[0] = tmp.ToString();
            ret[1] = (buff[6] - 0x30).ToString();
            ret[2] = buff[15].ToString();
            sock.Close();
            return ret;
        }


		void CommitClick(object sender, EventArgs e)
		{
            UInt32 ret = 0;
            ushort ipport;
            StatusLabel.Text = ls.get("StatusLabel_1");
            if (!File.Exists(filepath))
            {
                MessageBox.Show(ls.get("imsg/m4"));
                return;
            }
			if(ipBox.Text=="")
			{
				MessageBox.Show(ls.get("imsg/m1"));
				return;
			}
            if (!ushort.TryParse(portBox.Text,out ipport))
            {
                MessageBox.Show(ls.get("imsg/m5"));
                return;
            }
            string[] result = CheckServer(ipBox.Text, ipport);
            if (result == null)
            {
                MessageBox.Show(ls.get("imsg/m6"));
                return;
            }
			if(!locamap.ContainsKey(localeBox.Text))
			{
				MessageBox.Show(ls.get("imsg/m2"));
				return;
			}
            if(lastip!=ipBox.Text)
            {          
           /* object[] data = new object[]
            {
                AdaptorcomboBox.Text.Clone(),
                locamap[localeBox.Text].Clone(),
                ipBox.Text.Clone()
            };*/
            if (Worked)
            {
                ret=Free(0);
                if (ret > 0)
                {
                    MessageBox.Show(ret.ToString() + ":" + ls.get("msg/m" + ret));
                    StatusLabel.Text = ls.get("StatusLabel_3");
                    return;
                }
                Worked = false;
              /*  if (mainthread.IsAlive)
                    try
                    {
                            mainthread.Abort();
                    }
                    
                     catch
                    {
                        
                        }*/
            }
            
           // mainworkret = redirIp((string)data[0], (string)data[1], (string)data[2], (byte)1, (UInt16)0);
           // 
       
          //  mainthread = new Thread(MainWork);
          //  mainthread.Start(data);
          //  

            ret = redirIp(AdaptorcomboBox.Text, locamap[localeBox.Text], ipBox.Text, (byte)1, (UInt16)0);

             if (ret != 0)
             {
                 MessageBox.Show(ret.ToString() + ":" + ls.get("msg/m" + ret));
                 StatusLabel.Text = ls.get("StatusLabel_3");
                 return;
             }

            } 
             StatusLabel.Text = ls.get("StatusLabel_2")+ipBox.Text;
            
             Worked = true;
             Process Maple = new Process();
                Maple.StartInfo.FileName = filepath;
                Maple.StartInfo.Arguments =locamap[localeBox.Text]+" "+portBox.Text;
                Maple.Start();
                lastip = ipBox.Text.Clone().ToString();
		/*	*/
		}
		
		void MainFormFormClosed(object sender, FormClosedEventArgs e)
		{
            if (Opened)
            {
                WritePrivateProfileString("conf", "ip", ipBox.Text, "./conf.ini");
                WritePrivateProfileString("conf", "loc", localeBox.Text, "./conf.ini");
                WritePrivateProfileString("conf", "dev", AdaptorcomboBox.Text, "./conf.ini");
                WritePrivateProfileString("conf", "path", filepath, "./conf.ini");
                WritePrivateProfileString("conf", "port", portBox.Text, "./conf.ini");
            }
           
		}

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
             if (Opened)Free(1);
        }

        private void SetPath_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "MsMainFile|MapleStory.exe";
            ofd.CheckPathExists = true;
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                filepath = ofd.FileName;
            }
        }

        private void Check_Click(object sender, EventArgs e)
        {
            ushort ipport;
            if (ipBox.Text == "")
            {
                MessageBox.Show(ls.get("imsg/m1"));
                return;
            }
            if (!ushort.TryParse(portBox.Text, out ipport))
            {
                MessageBox.Show(ls.get("imsg/m5"));
                return;
            }
            string[] result = CheckServer(ipBox.Text, ipport);
            if (result == null)
            {
                MessageBox.Show(ls.get("imsg/m6"));
                return;
            }
            MessageBox.Show(string.Format(ls.get("imsg/m7"), result[0], result[1], result[2]));
        }
	}
}
