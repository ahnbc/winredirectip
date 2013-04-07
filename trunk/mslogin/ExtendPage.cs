using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
namespace mslogin
{
    public partial class ExtendPage : Form
    {
        

        public ExtendPage(MainForm mainPage)
        {
            InitializeComponent();
            m_MainPage = mainPage;
        }

        private void ExtendPage_Load(object sender, EventArgs e)
        {

        }

        private void RedirectOnly_OnClick(object sender, EventArgs e)
        {
            m_MainPage.ExPage_RedirectOnlyButton_Click();
        }
        
    }
}
