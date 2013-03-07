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
    //    private delegate UInt32 EndCall(int i);

    //    BeginCall pBeginCall;
    //    EndCall pEndCall;
    //    

        Thread mainthread;
        static  UInt32 mainworkret;
		static LangString ls;
        Boolean Opened = false;
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
			Commit.Text=ls.get("Commit");
			StatusLabel.Text=ls.get("StatusLabel_1");
			}
			LocationBox.SelectedIndex=0;
          //  pBeginCall = dBeginCall;
          //  pEndCall = dEndCall;
			
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
		StringBuilder sb=new StringBuilder(50);
		GetPrivateProfileString("conf","ip","",sb,50,"./conf.ini");
		string ip=sb.ToString();
		GetPrivateProfileString("conf","loc","",sb,50,"./conf.ini");
		string loc=sb.ToString();
		GetPrivateProfileString("conf","dev","",sb,50,"./conf.ini");
		string dev=sb.ToString();
		int select=0;
		ManagementClass   mc   =   new   ManagementClass( "Win32_NetworkAdapterConfiguration"); 
		ManagementObjectCollection   moc   =   mc.GetInstances(); 
		foreach(ManagementObject   mo   in   moc)   {
			string[] ips=(string [])(mo["IPAddress"]);
			if (ips!=null)
			{
				AdaptorcomboBox.Items.Add(ips[0]);
			}
			
		}
		AdaptorcomboBox.Items.Add("\\DEVICE\\NDISWANIP");
		for (int i=0;i<AdaptorcomboBox.Items.Count;i++)
			if(((string)AdaptorcomboBox.Items[i]).Equals(dev))select=i;
		AdaptorcomboBox.SelectedIndex=select;
		ipBox.Text=ip;
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
            sb.Append(locamap[LocationBox.Text]);
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
       static  void MainWork(Object paras)
        {
            object[] data = (object[])paras;
            mainworkret = redirIp((string)data[0], (string)data[1], (string)data[2], (byte)1, (UInt16)0);
           if(mainworkret!=0)
               {
                   MessageBox.Show(mainworkret.ToString() + ":" + ls.get("msg/m" + mainworkret));
                   return;
               }
        }


		void CommitClick(object sender, EventArgs e)
		{
			if(ipBox.Text=="")
			{
				MessageBox.Show(ls.get("imsg/m1"));
				return;
			}
			if(!locamap.ContainsKey(LocationBox.Text))
			{
				MessageBox.Show(ls.get("imsg/m2"));
				return;
			}
            mainthread = new Thread(MainWork);
          //  ret = redirIp(AdaptorcomboBox.Text, locamap[LocationBox.Text], ipBox.Text, (byte)1, (UInt16)0);
            mainthread.Start(new object[] { AdaptorcomboBox.Text.Clone(), locamap[LocationBox.Text].Clone(), ipBox.Text.Clone() });

		/*	*/
		}
		
		void MainFormFormClosed(object sender, FormClosedEventArgs e)
		{
            if (Opened)
            {
                WritePrivateProfileString("conf", "ip", ipBox.Text, "./conf.ini");
                WritePrivateProfileString("conf", "loc", LocationBox.Text, "./conf.ini");
                WritePrivateProfileString("conf", "dev", AdaptorcomboBox.Text, "./conf.ini");
                //Free(1);
            }
           
		}
	}
}
