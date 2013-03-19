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
        //dll init function
        [DllImport("winredirip.dll", EntryPoint = "DllInit", CharSet = CharSet.Unicode, ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        private static  extern UInt32 DllInit();
        // dll main function
        [DllImport("winredirip.dll", EntryPoint = "redirIP",  CharSet =CharSet.Unicode, ExactSpelling=true,CallingConvention = CallingConvention.StdCall)]
		private static  extern UInt32 redirIp(string devname,string orip, string redip , byte protocol ,
		                      UInt16 port );
        // free dll resource
        [DllImport("winredirip.dll", EntryPoint = "Free", ExactSpelling = true,SetLastError=true, CallingConvention = CallingConvention.StdCall)]
		private static extern UInt32 Free( byte closelib);
        // extened function need newest 
        [DllImport("winredirip.dll", EntryPoint = "RegisterNoPort",  ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        private static extern void RegisterNoPort(UInt16[] portlist,UInt32 size);
        private delegate void MyCallBack(IntPtr data, UInt32 size);
        [DllImport("winredirip.dll", EntryPoint = "RegisterInCallBack", ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        private static extern void RegisterInCallBack(MyCallBack c);
        [DllImport("winredirip.dll", EntryPoint = "RegisterOutCallBack", ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        private static extern void RegisterOutCallBack(MyCallBack c);

        // win api
        [DllImport("kernel32.dll", EntryPoint = "GetLastError")]
        private static extern UInt32 GetLastError();
        [DllImport("kernel32.dll", EntryPoint = "GetPrivateProfileStringW", CharSet = CharSet.Unicode)]
        private static extern int GetPrivateProfileString(string section, string key, 
                                                          string def, StringBuilder retVal, 
                                                          int size, string filePath);
		[DllImport("kernel32.dll",EntryPoint="WritePrivateProfileStringW", CharSet = CharSet.Unicode)]
        private static extern long WritePrivateProfileString(string section, string key, 
                                                             string val, string filePath);
        //callback
        private MyCallBack InC, OutC;
        // i18 language file reader
		 LangString ls;
        // ms main file path
         string filepath="";
        // if main form opened? if lang file no load it will be false
        Boolean Opened = false;
        // if redirecting work?
        Boolean Worked = false;
        // if callRun
        bool callRun = false;
        // last status for ip and netinterface
        string[] laststatus = new string[4];
        // for connect 
        private static ManualResetEvent TimeoutObject = new ManualResetEvent(false);
        //
        byte fixPathch;
        // locale dic
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
            // try get i18 file 
			try{
                // if en??
				ls=new LangString("zh");}
			catch
			{
					MessageBox.Show("Lang File not exist or open error.");
			}
            // init ui
			if(ls!=null)
			{
			Text=ls.get("title");
			UI1.Text=ls.get("UI1");
			UI2.Text=ls.get("UI2");
			UI3.Text=ls.get("UI3");
            SetPath.Text = ls.get("UI4");
            Check.Text = ls.get("UI5");
            Stop.Text = ls.get("UI6");
			Commit.Text=ls.get("Commit");
			StatusLabel.Text=ls.get("StatusLabel_1");
			}
            localeBox.Items.Clear();
            // init localebox
            foreach (KeyValuePair<String,String> kvp in locamap)
            {
                localeBox.Items.Add(kvp.Key);
            }
			localeBox.SelectedIndex=0;
            
			
		}
		void MainFormLoad(object sender, EventArgs e)
		{	
		if(ls==null)Close();
        try
        {
            // try to init dll
            DllInit();
        }
        catch 
        { 
            // close if cannot
            MessageBox.Show(ls.get("msg/m1"));
            Close();
        }
            // read conf file
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
        GetPrivateProfileString("conf", "noport", "",sb,100, "./conf.ini");
        NoPortBox.Text = sb.ToString();
		int select=0;
            // read adapters
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
        InC = new MyCallBack(InCallBack);
       // opened
        Opened = true;
		}
		
		void StatusStripItemClicked(object sender, ToolStripItemClickedEventArgs e)
		{
			
		}
		
		void MainFormLayout(object sender, LayoutEventArgs e)
		{

		}
        // beginconnect  callback
        private void TimeOutCallBack(IAsyncResult asyncresult)
        {
            try { 
            Socket sock = asyncresult as Socket;
            sock.EndConnect(asyncresult);
            }
            catch
            {

            }
            TimeoutObject.Set();
        }
        // check server status return null if fail string[] = ver , path , locale no
        string[] CheckServer(string testip,ushort testport)
        {
            byte[] buff=new byte[100];
            int tmp=0;
            Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            sock.ReceiveTimeout = sock.SendTimeout = 1000;
            try {
           TimeoutObject.Reset();
           sock.BeginConnect(testip, (int)testport, TimeOutCallBack, sock);
           if (TimeoutObject.WaitOne(1000))
           {
               if (!sock.Connected)
               {
                   return null;
               }
           }
           else
           {
               return null; 
           }
           tmp= sock.Receive(buff, 100, SocketFlags.None);
            }
            catch
            {
                return null;
            }


            if (tmp < 10) return null;
            string[] ret = new string[3];
            // read ver
            tmp = buff[2];
            tmp += (int)buff[3] << 8;
            ret[0] = tmp.ToString();
            // read path
            ret[1] = (buff[6] - 0x30).ToString();
            // read locale
            ret[2] = buff[15].ToString();
            sock.Close();
            return ret;
        }
        // incallback
       unsafe void InCallBack(IntPtr data, UInt32 size)
        {
            if (size != 16) return;
            byte* d = (byte *)data.ToPointer();
            if (d[0] != 14) return;
            d[6] = (byte)(fixPathch + 0x30);
        }
       unsafe void OutCallBack(IntPtr data, UInt32 size)
       {
           if (size != 5) return;
           byte* d = (byte*)data.ToPointer();
           if (d[0] != 14) return;
           d[6] = (byte)(fixPathch + 0x30);
       }
		void CommitClick(object sender, EventArgs e)
		{
            RedirectOnlyButton_Click(sender, e);
            //start game
            if (!callRun) return;
             Process Maple = new Process();
                Maple.StartInfo.FileName = filepath;
                Maple.StartInfo.Arguments =locamap[localeBox.Text]+" "+portBox.Text;
                Maple.Start();
          
		/*	*/
		}
		
		void MainFormFormClosed(object sender, FormClosedEventArgs e)
		{
            // if opend wirte conf
            if (Opened)
            {
                WritePrivateProfileString("conf", "ip", ipBox.Text, "./conf.ini");
                WritePrivateProfileString("conf", "loc", localeBox.Text, "./conf.ini");
                WritePrivateProfileString("conf", "dev", AdaptorcomboBox.Text, "./conf.ini");
                WritePrivateProfileString("conf", "path", filepath, "./conf.ini");
                WritePrivateProfileString("conf", "port", portBox.Text, "./conf.ini");
                WritePrivateProfileString("conf", "noport", NoPortBox.Text, "./conf.ini");
            }
           
		}
         
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
             if (Opened)Free(1);
        }
        // set path
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
            // worked and check last ip will fail
            if (Worked&&laststatus[0] == ipBox.Text)
            {
                MessageBox.Show(ls.get("imsg/m8"));
                return;
            }
            string[] result = CheckServer(ipBox.Text, ipport);
            if (result == null)
            {
                MessageBox.Show(ls.get("imsg/m6"));
                return;
            }
            // show msg
            MessageBox.Show(string.Format(ls.get("imsg/m7"), result[0], result[1], result[2]));
        }

        private void Stop_Click(object sender, EventArgs e)
        {
            if (!Worked)
            {
                MessageBox.Show(ls.get("imsg/m9"));
                return;
            }
            UInt32 ret = Free(0);
            if (ret == 0)
            {
                MessageBox.Show(ls.get("imsg/m10"));
                Worked = false;
            }
            else
            {
                MessageBox.Show(ls.get("imsg/m11"));
            }
            StatusLabel.Text = ls.get("StatusLabel_1");
        }

        private void RedirectOnlyButton_Click(object sender, EventArgs e)
        {
            UInt32 ret = 0;
            ushort ipport;
            StatusLabel.Text = ls.get("StatusLabel_1");
            // check ms mainfile exist
            callRun = false;
            if (!File.Exists(filepath))
            {
                MessageBox.Show(ls.get("imsg/m4"));
                return;
            }
            if (laststatus[3] != fixpatchbox.Text)
            {

            
            if (byte.TryParse(fixpatchbox.Text, out fixPathch))
                RegisterInCallBack(InC);
            else
                RegisterInCallBack(null);
            }

            // ipbox empty
           if (ipBox.Text == "")
                {
                    MessageBox.Show(ls.get("imsg/m1"));
                    return;
                }
            // port right
            if (!ushort.TryParse(portBox.Text, out ipport))
            {
                MessageBox.Show(ls.get("imsg/m5"));
                return;
            }
            // locamap right
            if (!locamap.ContainsKey(localeBox.Text))
            {
                MessageBox.Show(ls.get("imsg/m2"));
                return;
            }
            // if change noport
            if (NoPortBox.Text!="" && laststatus[2] != NoPortBox.Text)
            {
                String[] portlist = NoPortBox.Text.Split(',');
                UInt16[] plist =new UInt16[portlist.Length];
                for(int i=0;i<portlist.Length; i++)
                {
                    UInt16 res;
                    if (UInt16.TryParse(portlist[i], out res))
                        plist[i] = res;
                }
                RegisterNoPort(plist,unchecked((UInt32)plist.Length));
            }
            // if  change  ??
            if (laststatus[0] != ipBox.Text || laststatus[1] != AdaptorcomboBox.Text)
            {
                // if work free
                if (Worked)
                {
                    ret = Free(0);
                    if (ret > 0)
                    {
                        MessageBox.Show(ret.ToString() + ":" + ls.get("msg/m" + ret));
                        StatusLabel.Text = ls.get("StatusLabel_3");
                        return;
                    }
                    Worked = false;
                }
                // check 
                string[] result = CheckServer(ipBox.Text, ipport);
                if (result == null)
                {
                    MessageBox.Show(ls.get("imsg/m6"));
                    return;
                }

            }//end if change
            // if not worked redirect
            if (!Worked)
            {
                ret = redirIp(AdaptorcomboBox.Text, locamap[localeBox.Text], ipBox.Text, (byte)1, (UInt16)0);

                if (ret != 0)
                {
                    MessageBox.Show(ret.ToString() + ":" + ls.get("msg/m" + ret));
                    StatusLabel.Text = ls.get("StatusLabel_3");
                    return;
                }
            }
            //finsh
            StatusLabel.Text = ls.get("StatusLabel_2") + ipBox.Text;

            Worked = true;
            // set last status
            laststatus[0] = ipBox.Text.Clone().ToString();
            laststatus[1] = AdaptorcomboBox.Text.Clone().ToString();
            laststatus[2] = NoPortBox.Text.Clone().ToString();
            laststatus[3] = fixpatchbox.Text.Clone().ToString();
            callRun = true;
        }


	}
}
