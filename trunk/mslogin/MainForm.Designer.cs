/*
 * Created by SharpDevelop.
 * User: nowind
 * Date: 2013/3/7
 * Time: 10:14
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
namespace mslogin
{
	partial class MainForm
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		
		/// <summary>
		/// Disposes resources used by the form.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing) {
				if (components != null) {
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}
		
		/// <summary>
		/// This method is required for Windows Forms designer support.
		/// Do not change the method contents inside the source code editor. The Forms designer might
		/// not be able to load this method if it was changed manually.
		/// </summary>
		private void InitializeComponent()
		{
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.m_labNetWork = new System.Windows.Forms.Label();
            this.m_AdaptorcomboBox = new System.Windows.Forms.ComboBox();
            this.m_statusStrip = new System.Windows.Forms.StatusStrip();
            this.m_StatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.m_lablocale = new System.Windows.Forms.Label();
            this.m_localeBox = new System.Windows.Forms.ComboBox();
            this.m_labIp = new System.Windows.Forms.Label();
            this.m_ipBox = new System.Windows.Forms.TextBox();
            this.m_btnCommit = new System.Windows.Forms.Button();
            this.m_btnSetPath = new System.Windows.Forms.Button();
            this.m_btnCheck = new System.Windows.Forms.Button();
            this.m_portBox = new System.Windows.Forms.TextBox();
            this.m_btnStop = new System.Windows.Forms.Button();
            this.Mainpanel = new System.Windows.Forms.Panel();
            this.m_btnMore = new System.Windows.Forms.Button();
            this.m_statusStrip.SuspendLayout();
            this.Mainpanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_labNetWork
            // 
            this.m_labNetWork.Location = new System.Drawing.Point(3, 3);
            this.m_labNetWork.Name = "m_labNetWork";
            this.m_labNetWork.Size = new System.Drawing.Size(232, 20);
            this.m_labNetWork.TabIndex = 0;
            this.m_labNetWork.Text = "NetWork Interface:";
            // 
            // m_AdaptorcomboBox
            // 
            this.m_AdaptorcomboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_AdaptorcomboBox.FormattingEnabled = true;
            this.m_AdaptorcomboBox.Location = new System.Drawing.Point(8, 26);
            this.m_AdaptorcomboBox.Name = "m_AdaptorcomboBox";
            this.m_AdaptorcomboBox.Size = new System.Drawing.Size(186, 20);
            this.m_AdaptorcomboBox.TabIndex = 1;
            // 
            // m_statusStrip
            // 
            this.m_statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.m_StatusLabel});
            this.m_statusStrip.Location = new System.Drawing.Point(5, 246);
            this.m_statusStrip.Name = "m_statusStrip";
            this.m_statusStrip.Size = new System.Drawing.Size(246, 22);
            this.m_statusStrip.SizingGrip = false;
            this.m_statusStrip.TabIndex = 2;
            this.m_statusStrip.Text = "statusStrip";
            // 
            // m_StatusLabel
            // 
            this.m_StatusLabel.Name = "m_StatusLabel";
            this.m_StatusLabel.Size = new System.Drawing.Size(73, 17);
            this.m_StatusLabel.Text = "StatusLabel";
            // 
            // m_lablocale
            // 
            this.m_lablocale.Location = new System.Drawing.Point(3, 54);
            this.m_lablocale.Name = "m_lablocale";
            this.m_lablocale.Size = new System.Drawing.Size(232, 18);
            this.m_lablocale.TabIndex = 3;
            this.m_lablocale.Text = "MS locale:";
            // 
            // m_localeBox
            // 
            this.m_localeBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.m_localeBox.FormattingEnabled = true;
            this.m_localeBox.Items.AddRange(new object[] {
            "Chinese"});
            this.m_localeBox.Location = new System.Drawing.Point(8, 75);
            this.m_localeBox.Name = "m_localeBox";
            this.m_localeBox.Size = new System.Drawing.Size(186, 20);
            this.m_localeBox.TabIndex = 4;
            // 
            // m_labIp
            // 
            this.m_labIp.Location = new System.Drawing.Point(6, 106);
            this.m_labIp.Name = "m_labIp";
            this.m_labIp.Size = new System.Drawing.Size(229, 23);
            this.m_labIp.TabIndex = 5;
            this.m_labIp.Text = "Private Server:(IP or domain)";
            // 
            // m_ipBox
            // 
            this.m_ipBox.Location = new System.Drawing.Point(8, 132);
            this.m_ipBox.Name = "m_ipBox";
            this.m_ipBox.Size = new System.Drawing.Size(119, 21);
            this.m_ipBox.TabIndex = 6;
            // 
            // m_btnCommit
            // 
            this.m_btnCommit.Location = new System.Drawing.Point(5, 159);
            this.m_btnCommit.Name = "m_btnCommit";
            this.m_btnCommit.Size = new System.Drawing.Size(110, 31);
            this.m_btnCommit.TabIndex = 7;
            this.m_btnCommit.Text = "Commit";
            this.m_btnCommit.UseVisualStyleBackColor = true;
            this.m_btnCommit.Click += new System.EventHandler(this.CommitClick);
            // 
            // m_btnSetPath
            // 
            this.m_btnSetPath.Location = new System.Drawing.Point(122, 159);
            this.m_btnSetPath.Name = "m_btnSetPath";
            this.m_btnSetPath.Size = new System.Drawing.Size(91, 31);
            this.m_btnSetPath.TabIndex = 8;
            this.m_btnSetPath.Text = "SetPath";
            this.m_btnSetPath.UseVisualStyleBackColor = true;
            this.m_btnSetPath.Click += new System.EventHandler(this.SetPath_Click);
            // 
            // m_btnCheck
            // 
            this.m_btnCheck.Location = new System.Drawing.Point(8, 196);
            this.m_btnCheck.Name = "m_btnCheck";
            this.m_btnCheck.Size = new System.Drawing.Size(94, 34);
            this.m_btnCheck.TabIndex = 9;
            this.m_btnCheck.Text = "Check";
            this.m_btnCheck.UseVisualStyleBackColor = true;
            this.m_btnCheck.Click += new System.EventHandler(this.Check_Click);
            // 
            // m_portBox
            // 
            this.m_portBox.Location = new System.Drawing.Point(133, 132);
            this.m_portBox.Name = "m_portBox";
            this.m_portBox.Size = new System.Drawing.Size(80, 21);
            this.m_portBox.TabIndex = 10;
            // 
            // m_btnStop
            // 
            this.m_btnStop.Location = new System.Drawing.Point(118, 196);
            this.m_btnStop.Name = "m_btnStop";
            this.m_btnStop.Size = new System.Drawing.Size(95, 34);
            this.m_btnStop.TabIndex = 11;
            this.m_btnStop.Text = "Stop Redirect";
            this.m_btnStop.UseVisualStyleBackColor = true;
            this.m_btnStop.Click += new System.EventHandler(this.Stop_Click);
            // 
            // Mainpanel
            // 
            this.Mainpanel.Controls.Add(this.m_btnMore);
            this.Mainpanel.Controls.Add(this.m_labNetWork);
            this.Mainpanel.Controls.Add(this.m_btnStop);
            this.Mainpanel.Controls.Add(this.m_AdaptorcomboBox);
            this.Mainpanel.Controls.Add(this.m_portBox);
            this.Mainpanel.Controls.Add(this.m_lablocale);
            this.Mainpanel.Controls.Add(this.m_btnCheck);
            this.Mainpanel.Controls.Add(this.m_localeBox);
            this.Mainpanel.Controls.Add(this.m_btnSetPath);
            this.Mainpanel.Controls.Add(this.m_labIp);
            this.Mainpanel.Controls.Add(this.m_btnCommit);
            this.Mainpanel.Controls.Add(this.m_ipBox);
            this.Mainpanel.Location = new System.Drawing.Point(8, 3);
            this.Mainpanel.Name = "Mainpanel";
            this.Mainpanel.Size = new System.Drawing.Size(234, 238);
            this.Mainpanel.TabIndex = 12;
            // 
            // m_btnMore
            // 
            this.m_btnMore.Location = new System.Drawing.Point(212, 91);
            this.m_btnMore.Name = "m_btnMore";
            this.m_btnMore.Size = new System.Drawing.Size(21, 35);
            this.m_btnMore.TabIndex = 12;
            this.m_btnMore.Text = ">";
            this.m_btnMore.UseVisualStyleBackColor = true;
            this.m_btnMore.Click += new System.EventHandler(this.More_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(251, 268);
            this.Controls.Add(this.Mainpanel);
            this.Controls.Add(this.m_statusStrip);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Padding = new System.Windows.Forms.Padding(5, 0, 0, 0);
            this.Text = "mslogin";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainFormFormClosed);
            this.Load += new System.EventHandler(this.MainFormLoad);
            this.ResizeEnd += new System.EventHandler(this.MainForm_ResizeEnd);
            this.Layout += new System.Windows.Forms.LayoutEventHandler(this.MainFormLayout);
            this.m_statusStrip.ResumeLayout(false);
            this.m_statusStrip.PerformLayout();
            this.Mainpanel.ResumeLayout(false);
            this.Mainpanel.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
        private System.Windows.Forms.TextBox m_ipBox;
		private System.Windows.Forms.Label m_lablocale;
		private System.Windows.Forms.Label m_labIp;
        private System.Windows.Forms.ComboBox m_localeBox;
		private System.Windows.Forms.Label m_labNetWork;
        private System.Windows.Forms.ToolStripStatusLabel m_StatusLabel;
		private System.Windows.Forms.Button m_btnCommit;
		private System.Windows.Forms.StatusStrip m_statusStrip;
        private System.Windows.Forms.ComboBox m_AdaptorcomboBox;
        private System.Windows.Forms.Button m_btnSetPath;
        private System.Windows.Forms.Button m_btnCheck;
        private System.Windows.Forms.TextBox m_portBox;
        private System.Windows.Forms.Button m_btnStop;
        private System.Windows.Forms.Panel Mainpanel;
        private ExtendPage m_extendPage;
        private System.Windows.Forms.Button m_btnMore;
	}
}
