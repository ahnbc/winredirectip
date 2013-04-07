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
using System.Security.Cryptography;
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
        private static extern UInt32 redirIp(string devname, string orip, string redip, byte protocol,
		                      UInt16 port );
        // free dll resource
        [DllImport("winredirip.dll", EntryPoint = "Free", ExactSpelling = true,SetLastError=true, CallingConvention = CallingConvention.StdCall)]
        private static extern UInt32 Free(byte closelib);
        // extened function need newest 
        [DllImport("winredirip.dll", EntryPoint = "RegisterNoPort",  ExactSpelling = true, CallingConvention = CallingConvention.StdCall)]
        private static extern void RegisterNoPort(UInt16[] portlist, UInt32 size);
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
        MyCallBack m_callInC, m_callOutC;
        // i18 language file reader
		LangString m_i18ls;
        // ms main file path
        string m_strFilepath="";
        // if main form opened? if lang file no load it will be false
        Boolean m_bOpened = false;
        // if redirecting work?
        Boolean m_bWorked = false;
        // if callRun
        bool m_bcallRun = false;
        // last status for ip and netinterface
        string[] m_arrLaststatus = new string[4];
        // for connect 
        private static ManualResetEvent TimeoutObject = new ManualResetEvent(false);
        //
        byte m_byOrginPatch=0;
        byte m_byFixPathch = 0;
        private bool isExtendPageShow = false;

        private static byte[] sSecretKey = new byte[] {
            0x13, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xB4, 0x00, 0x00, 0x00,
            0x1B, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x00 
        };
        /*
        private static byte[] sShiftKey = new byte[] {
            0xEC, 0x3F, 0x77, 0xA4, 0x45, 0xD0, 0x71, 0xBF, 0xB7, 0x98, 0x20, 0xFC, 0x4B, 0xE9, 0xB3, 0xE1,
            0x5C, 0x22, 0xF7, 0x0C, 0x44, 0x1B, 0x81, 0xBD, 0x63, 0x8D, 0xD4, 0xC3, 0xF2, 0x10, 0x19, 0xE0,
            0xFB, 0xA1, 0x6E, 0x66, 0xEA, 0xAE, 0xD6, 0xCE, 0x06, 0x18, 0x4E, 0xEB, 0x78, 0x95, 0xDB, 0xBA,
            0xB6, 0x42, 0x7A, 0x2A, 0x83, 0x0B, 0x54, 0x67, 0x6D, 0xE8, 0x65, 0xE7, 0x2F, 0x07, 0xF3, 0xAA,
            0x27, 0x7B, 0x85, 0xB0, 0x26, 0xFD, 0x8B, 0xA9, 0xFA, 0xBE, 0xA8, 0xD7, 0xCB, 0xCC, 0x92, 0xDA,
            0xF9, 0x93, 0x60, 0x2D, 0xDD, 0xD2, 0xA2, 0x9B, 0x39, 0x5F, 0x82, 0x21, 0x4C, 0x69, 0xF8, 0x31,
            0x87, 0xEE, 0x8E, 0xAD, 0x8C, 0x6A, 0xBC, 0xB5, 0x6B, 0x59, 0x13, 0xF1, 0x04, 0x00, 0xF6, 0x5A,
            0x35, 0x79, 0x48, 0x8F, 0x15, 0xCD, 0x97, 0x57, 0x12, 0x3E, 0x37, 0xFF, 0x9D, 0x4F, 0x51, 0xF5,
            0xA3, 0x70, 0xBB, 0x14, 0x75, 0xC2, 0xB8, 0x72, 0xC0, 0xED, 0x7D, 0x68, 0xC9, 0x2E, 0x0D, 0x62,
            0x46, 0x17, 0x11, 0x4D, 0x6C, 0xC4, 0x7E, 0x53, 0xC1, 0x25, 0xC7, 0x9A, 0x1C, 0x88, 0x58, 0x2C,
            0x89, 0xDC, 0x02, 0x64, 0x40, 0x01, 0x5D, 0x38, 0xA5, 0xE2, 0xAF, 0x55, 0xD5, 0xEF, 0x1A, 0x7C,
            0xA7, 0x5B, 0xA6, 0x6F, 0x86, 0x9F, 0x73, 0xE6, 0x0A, 0xDE, 0x2B, 0x99, 0x4A, 0x47, 0x9C, 0xDF,
            0x09, 0x76, 0x9E, 0x30, 0x0E, 0xE4, 0xB2, 0x94, 0xA0, 0x3B, 0x34, 0x1D, 0x28, 0x0F, 0x36, 0xE3,
            0x23, 0xB4, 0x03, 0xD8, 0x90, 0xC8, 0x3C, 0xFE, 0x5E, 0x32, 0x24, 0x50, 0x1F, 0x3A, 0x43, 0x8A,
            0x96, 0x41, 0x74, 0xAC, 0x52, 0x33, 0xF0, 0xD9, 0x29, 0x80, 0xB1, 0x16, 0xD3, 0xAB, 0x91, 0xB9,
            0x84, 0x7F, 0x61, 0x1E, 0xCF, 0xC5, 0xD1, 0x56, 0x3D, 0xCA, 0xF4, 0x05, 0xC6, 0xE5, 0x08, 0x49 
        };*/
        private byte[] mIV = new byte[4];
        private ICryptoTransform mTransformer = null;
        private RijndaelManaged mAES = new RijndaelManaged();

        // locale dic
		 static Dictionary<String, String> locamap=new Dictionary<string, string>()
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
				m_i18ls=new LangString("zh");}
			catch
			{
					MessageBox.Show("Lang File not exist or open error.");
			}
            // init ui
            if (m_i18ls != null)
			{
			Text=m_i18ls.get("title");
            m_extendPage = new ExtendPage(this);
			m_labNetWork.Text=m_i18ls.get("UI1");
			m_lablocale.Text=m_i18ls.get("UI2");
			m_labIp.Text=m_i18ls.get("UI3");
            m_btnSetPath.Text = m_i18ls.get("UI4");
            m_btnCheck.Text = m_i18ls.get("UI5");
            m_btnStop.Text = m_i18ls.get("UI6");
			m_btnCommit.Text=m_i18ls.get("Commit");
            m_extendPage.m_labNoPort.Text = m_i18ls.get("UI7");
            m_extendPage.m_labFixPatch.Text = m_i18ls.get("UI8");
            m_extendPage.m_btnRedirectOnly.Text = m_i18ls.get("UI9");
			m_StatusLabel.Text=m_i18ls.get("StatusLabel_1");
			}
            m_localeBox.Items.Clear();
            // init localebox
            foreach (KeyValuePair<String,String> kvp in locamap)
            {
                m_localeBox.Items.Add(kvp.Key);
            }
			m_localeBox.SelectedIndex=0;
            mAES.Key = sSecretKey;
            mAES.Mode = CipherMode.ECB;
            mAES.Padding = PaddingMode.PKCS7;
            mTransformer = mAES.CreateEncryptor();
			
		}
		void MainFormLoad(object sender, EventArgs e)
		{
            if (m_i18ls == null) Close();
        try
        {
            // try to init dll
            DllInit();
        }
        catch 
        { 
            // close if cannot
            MessageBox.Show(m_i18ls.get("msg/m1"));
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
        m_strFilepath = sb.ToString();
        GetPrivateProfileString("conf", "port", "8484", sb, 100, "./conf.ini");
        m_portBox.Text = sb.ToString();
        GetPrivateProfileString("conf", "noport", "",sb,100, "./conf.ini");
        m_extendPage.m_NoPortBox.Text = sb.ToString();
		int select=0;
            // read adapters
		ManagementClass   mc   =   new   ManagementClass( "Win32_NetworkAdapterConfiguration"); 
		ManagementObjectCollection   moc   =   mc.GetInstances(); 
		foreach(ManagementObject   mo   in   moc)   {
			string[] ips=(string [])(mo["IPAddress"]);
			if (ips!=null)
			{
                if (!m_AdaptorcomboBox.Items.Contains(ips[0]))
				m_AdaptorcomboBox.Items.Add(ips[0]);
			}
			
		}
		m_AdaptorcomboBox.Items.Add("\\DEVICE\\NDISWANIP");
		for (int i=0;i<m_AdaptorcomboBox.Items.Count;i++)
			if(((string)m_AdaptorcomboBox.Items[i]).Equals(dev))select=i;
		m_AdaptorcomboBox.SelectedIndex=select;
		m_ipBox.Text=ip;
        m_callInC = new MyCallBack(InCallBack);
        m_callOutC = new MyCallBack(OutCallBack);
       // opened
        m_bOpened = true;
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
        public string[] CheckServer(string testip,ushort testport)
        {
            byte[] buff=new byte[100];
            int tmp=0;
            Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            sock.ReceiveTimeout = sock.SendTimeout = 1000;
            try {
           TimeoutObject.Reset();
           sock.BeginConnect(testip, (int)testport, TimeOutCallBack, sock);
           if (TimeoutObject.WaitOne(1000,true))
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
        public void TransformAES(byte[] pBuffer)
        {
            int remaining = pBuffer.Length;
            int length = 0x5B0;
            int start = 0;
            byte[] realIV = new byte[mIV.Length * 4];
            while (remaining > 0)
            {
                for (int index = 0; index < realIV.Length; ++index) realIV[index] = mIV[index % 4];

                if (remaining < length) length = remaining;
                for (int index = start; index < (start + length); ++index)
                {
                    if (((index - start) % realIV.Length) == 0)
                    {
                        byte[] tempIV = new byte[realIV.Length];
                        mTransformer.TransformBlock(realIV, 0, realIV.Length, tempIV, 0);
                        Buffer.BlockCopy(tempIV, 0, realIV, 0, realIV.Length);
                        //realIV = mTransformer.TransformFinalBlock(realIV, 0, realIV.Length);
                    }
                    pBuffer[index] ^= realIV[(index - start) % realIV.Length];
                }
                start += length;
                remaining -= length;
                length = 0x5B4;
            }
        }
        // incallback
       unsafe void InCallBack(IntPtr data, UInt32 size)
        {
            if (size != 16 || m_byOrginPatch!=0) return;
            byte* d = (byte *)data.ToPointer();
            if (d[0] != 14) return;
            mIV[0] = d[7]; mIV[1] = d[8]; mIV[2] = d[9]; mIV[3] = d[10];
            m_byOrginPatch = d[6];
            d[6] = (byte)(m_byFixPathch + 0x30);
        }
       unsafe void OutCallBack(IntPtr data, UInt32 size)
       {
           if (size != 11 || m_byOrginPatch==0 ) return;
           byte* unsafe_d = (byte*)data.ToPointer();
           byte[] d = new byte[size-4];
           for (int i = 4; i < size; i++) d[i-4] = unsafe_d[i];
           TransformAES(d);
           d[2 + 3] = (byte)(m_byOrginPatch - 0x30);
           TransformAES(d);
           for (int i = 4; i < size; i++)  unsafe_d[i]=d[i - 4];
           m_byOrginPatch = 0;
       }
		void CommitClick(object sender, EventArgs e)
		{
            ExPage_RedirectOnlyButton_Click();
            //start game
            if (!m_bcallRun) return;
             Process Maple = new Process();
                Maple.StartInfo.FileName = m_strFilepath;
                Maple.StartInfo.Arguments =locamap[m_localeBox.Text]+" "+m_portBox.Text;
                Maple.Start();
          
		/*	*/
		}
		
		void MainFormFormClosed(object sender, FormClosedEventArgs e)
		{
            // if opend wirte conf
            if (m_bOpened)
            {
                WritePrivateProfileString("conf", "ip", m_ipBox.Text, "./conf.ini");
                WritePrivateProfileString("conf", "loc", m_localeBox.Text, "./conf.ini");
                WritePrivateProfileString("conf", "dev", m_AdaptorcomboBox.Text, "./conf.ini");
                WritePrivateProfileString("conf", "path", m_strFilepath, "./conf.ini");
                WritePrivateProfileString("conf", "port", m_portBox.Text, "./conf.ini");
                WritePrivateProfileString("conf", "noport", m_extendPage.m_NoPortBox.Text, "./conf.ini");
            }
           
		}
         
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
             if (m_bOpened)Free(1);
        }
        // set path
        private void SetPath_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "MsMainFile|MapleStory.exe";
            ofd.CheckPathExists = true;
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                m_strFilepath = ofd.FileName;
            }
        }

        private void Check_Click(object sender, EventArgs e)
        {
            ushort ipport;
            if (m_ipBox.Text == "")
            {
                MessageBox.Show(m_i18ls.get("imsg/m1"));
                return;
            }
            if (!ushort.TryParse(m_portBox.Text, out ipport))
            {
                MessageBox.Show(m_i18ls.get("imsg/m5"));
                return;
            }
            // worked and check last ip will fail
            if (m_bWorked&&m_arrLaststatus[0] == m_ipBox.Text)
            {
                MessageBox.Show(m_i18ls.get("imsg/m8"));
                return;
            }
            string[] result = CheckServer(m_ipBox.Text, ipport);
            if (result == null)
            {
                MessageBox.Show(m_i18ls.get("imsg/m6"));
                return;
            }
            // show msg
            MessageBox.Show(string.Format(m_i18ls.get("imsg/m7"), result[0], result[1], result[2]));
        }

        private void Stop_Click(object sender, EventArgs e)
        {
            if (!m_bWorked)
            {
                MessageBox.Show(m_i18ls.get("imsg/m9"));
                return;
            }
            UInt32 ret = Free(0);
            if (ret == 0)
            {
                MessageBox.Show(m_i18ls.get("imsg/m10"));
                m_bWorked = false;
            }
            else
            {
                MessageBox.Show(m_i18ls.get("imsg/m11"));
            }
            m_StatusLabel.Text = m_i18ls.get("StatusLabel_1");
        }

        

        private void NoPortlabel_Click(object sender, EventArgs e)
        {

        }

        private void More_Click(object sender, EventArgs e)
        {
            if (!isExtendPageShow)
            {
                isExtendPageShow = true;
                m_btnMore.Text = "<";
                m_extendPage.Show();
                MainForm_ResizeEnd(sender, e);
            }
            else
            {
                isExtendPageShow = false;
                m_btnMore.Text = ">";
                m_extendPage.Hide();
            }
            
        }

        private void MainForm_ResizeEnd(object sender, EventArgs e)
        {
            if (!isExtendPageShow) return;
            m_extendPage.Top = Top + 10;
            m_extendPage.Height = Height - 20;
            m_extendPage.Left = Right+3 ;
        }
        public void ExPage_RedirectOnlyButton_Click()
        {
            UInt32 ret = 0;
            ushort ipport;
            m_StatusLabel.Text = m_i18ls.get("StatusLabel_1");
            // check ms mainfile exist
           m_bcallRun = false;
            if (!File.Exists(m_strFilepath))
            {
                MessageBox.Show(m_i18ls.get("imsg/m4"));
                return;
            }
            if (m_arrLaststatus[3] != m_extendPage.m_Fixpatchbox.Text)
            {


                if (byte.TryParse(m_extendPage.m_Fixpatchbox.Text, out m_byFixPathch))
                {
                    RegisterInCallBack(m_callInC);
                    RegisterOutCallBack(m_callOutC);
                }
                else
                {
                   RegisterInCallBack(null);
                   RegisterOutCallBack(m_callOutC);
                }
            }

            // ipbox empty
            if (m_ipBox.Text == "")
            {
                MessageBox.Show(m_i18ls.get("imsg/m1"));
                return;
            }
            // port right
            if (!ushort.TryParse(m_portBox.Text, out ipport))
            {
                MessageBox.Show(m_i18ls.get("imsg/m5"));
                return;
            }
            // locamap right
            if (!MainForm.locamap.ContainsKey(m_localeBox.Text))
            {
                MessageBox.Show(m_i18ls.get("imsg/m2"));
                return;
            }
            // if change noport
            if (m_extendPage.m_NoPortBox.Text != "" && m_arrLaststatus[2] != m_extendPage.m_NoPortBox.Text)
            {
                String[] portlist = m_extendPage.m_NoPortBox.Text.Split(',');
                UInt16[] plist = new UInt16[portlist.Length];
                for (int i = 0; i < portlist.Length; i++)
                {
                    UInt16 res;
                    if (UInt16.TryParse(portlist[i], out res))
                        plist[i] = res;
                }
                RegisterNoPort(plist, unchecked((UInt32)plist.Length));
            }
            // if  change  ??
            if (m_arrLaststatus[0] != m_ipBox.Text || m_arrLaststatus[1] != m_AdaptorcomboBox.Text)
            {
                // if work free
                if (m_bWorked)
                {
                    ret = Free(0);
                    if (ret > 0)
                    {
                        MessageBox.Show(ret.ToString() + ":" + m_i18ls.get("msg/m" + ret));
                        m_StatusLabel.Text =m_i18ls.get("StatusLabel_3");
                        return;
                    }
                    m_bWorked = false;
                }
                // check 
                string[] result = CheckServer(m_ipBox.Text, ipport);
                if (result == null)
                {
                    MessageBox.Show(m_i18ls.get("imsg/m6"));
                    return;
                }

            }//end if change
            // if not worked redirect
            if (!m_bWorked)
            {
                ret = redirIp(m_AdaptorcomboBox.Text, MainForm.locamap[m_localeBox.Text],
                       m_ipBox.Text, (byte)1, (UInt16)0);

                if (ret != 0)
                {
                    MessageBox.Show(ret.ToString() + ":" +m_i18ls.get("msg/m" + ret));
                    m_StatusLabel.Text =m_i18ls.get("StatusLabel_3");
                    return;
                }
            }
            //finsh
            m_StatusLabel.Text = m_i18ls.get("StatusLabel_2") + m_ipBox.Text;

            m_bWorked = true;
            // set last status
            m_arrLaststatus[0] = m_ipBox.Text.Clone().ToString();
            m_arrLaststatus[1] = m_AdaptorcomboBox.Text.Clone().ToString();
            m_arrLaststatus[2] = m_extendPage.m_NoPortBox.Text.Clone().ToString();
            m_arrLaststatus[3] = m_extendPage.m_Fixpatchbox.Text.Clone().ToString();
            m_bcallRun = true;
        }

	}
}
