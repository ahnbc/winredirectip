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
            this.UI1 = new System.Windows.Forms.Label();
            this.AdaptorcomboBox = new System.Windows.Forms.ComboBox();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.StatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.UI2 = new System.Windows.Forms.Label();
            this.localeBox = new System.Windows.Forms.ComboBox();
            this.UI3 = new System.Windows.Forms.Label();
            this.ipBox = new System.Windows.Forms.TextBox();
            this.Commit = new System.Windows.Forms.Button();
            this.SetPath = new System.Windows.Forms.Button();
            this.Check = new System.Windows.Forms.Button();
            this.portBox = new System.Windows.Forms.TextBox();
            this.Stop = new System.Windows.Forms.Button();
            this.Mainpanel = new System.Windows.Forms.Panel();
            this.Expanel = new System.Windows.Forms.Panel();
            this.RedirectOnlyButton = new System.Windows.Forms.Button();
            this.fixpatchbox = new System.Windows.Forms.TextBox();
            this.FixPatchlabel = new System.Windows.Forms.Label();
            this.NoPortBox = new System.Windows.Forms.TextBox();
            this.NoPortlabel = new System.Windows.Forms.Label();
            this.statusStrip.SuspendLayout();
            this.Mainpanel.SuspendLayout();
            this.Expanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // UI1
            // 
            this.UI1.Location = new System.Drawing.Point(3, 3);
            this.UI1.Name = "UI1";
            this.UI1.Size = new System.Drawing.Size(232, 20);
            this.UI1.TabIndex = 0;
            this.UI1.Text = "NetWork Interface:";
            // 
            // AdaptorcomboBox
            // 
            this.AdaptorcomboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.AdaptorcomboBox.FormattingEnabled = true;
            this.AdaptorcomboBox.Location = new System.Drawing.Point(8, 26);
            this.AdaptorcomboBox.Name = "AdaptorcomboBox";
            this.AdaptorcomboBox.Size = new System.Drawing.Size(186, 20);
            this.AdaptorcomboBox.TabIndex = 1;
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.StatusLabel});
            this.statusStrip.Location = new System.Drawing.Point(5, 253);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(455, 22);
            this.statusStrip.SizingGrip = false;
            this.statusStrip.TabIndex = 2;
            this.statusStrip.Text = "statusStrip";
            // 
            // StatusLabel
            // 
            this.StatusLabel.Name = "StatusLabel";
            this.StatusLabel.Size = new System.Drawing.Size(74, 17);
            this.StatusLabel.Text = "StatusLabel";
            // 
            // UI2
            // 
            this.UI2.Location = new System.Drawing.Point(3, 54);
            this.UI2.Name = "UI2";
            this.UI2.Size = new System.Drawing.Size(232, 18);
            this.UI2.TabIndex = 3;
            this.UI2.Text = "MS locale:";
            // 
            // localeBox
            // 
            this.localeBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.localeBox.FormattingEnabled = true;
            this.localeBox.Items.AddRange(new object[] {
            "Chinese"});
            this.localeBox.Location = new System.Drawing.Point(8, 75);
            this.localeBox.Name = "localeBox";
            this.localeBox.Size = new System.Drawing.Size(186, 20);
            this.localeBox.TabIndex = 4;
            // 
            // UI3
            // 
            this.UI3.Location = new System.Drawing.Point(6, 106);
            this.UI3.Name = "UI3";
            this.UI3.Size = new System.Drawing.Size(229, 23);
            this.UI3.TabIndex = 5;
            this.UI3.Text = "Private Server:(IP or domain)";
            // 
            // ipBox
            // 
            this.ipBox.Location = new System.Drawing.Point(8, 132);
            this.ipBox.Name = "ipBox";
            this.ipBox.Size = new System.Drawing.Size(119, 21);
            this.ipBox.TabIndex = 6;
            // 
            // Commit
            // 
            this.Commit.Location = new System.Drawing.Point(5, 159);
            this.Commit.Name = "Commit";
            this.Commit.Size = new System.Drawing.Size(110, 31);
            this.Commit.TabIndex = 7;
            this.Commit.Text = "Commit";
            this.Commit.UseVisualStyleBackColor = true;
            this.Commit.Click += new System.EventHandler(this.CommitClick);
            // 
            // SetPath
            // 
            this.SetPath.Location = new System.Drawing.Point(122, 159);
            this.SetPath.Name = "SetPath";
            this.SetPath.Size = new System.Drawing.Size(91, 31);
            this.SetPath.TabIndex = 8;
            this.SetPath.Text = "SetPath";
            this.SetPath.UseVisualStyleBackColor = true;
            this.SetPath.Click += new System.EventHandler(this.SetPath_Click);
            // 
            // Check
            // 
            this.Check.Location = new System.Drawing.Point(8, 196);
            this.Check.Name = "Check";
            this.Check.Size = new System.Drawing.Size(94, 34);
            this.Check.TabIndex = 9;
            this.Check.Text = "Check";
            this.Check.UseVisualStyleBackColor = true;
            this.Check.Click += new System.EventHandler(this.Check_Click);
            // 
            // portBox
            // 
            this.portBox.Location = new System.Drawing.Point(133, 132);
            this.portBox.Name = "portBox";
            this.portBox.Size = new System.Drawing.Size(80, 21);
            this.portBox.TabIndex = 10;
            // 
            // Stop
            // 
            this.Stop.Location = new System.Drawing.Point(118, 196);
            this.Stop.Name = "Stop";
            this.Stop.Size = new System.Drawing.Size(95, 34);
            this.Stop.TabIndex = 11;
            this.Stop.Text = "Stop Redirect";
            this.Stop.UseVisualStyleBackColor = true;
            this.Stop.Click += new System.EventHandler(this.Stop_Click);
            // 
            // Mainpanel
            // 
            this.Mainpanel.Controls.Add(this.UI1);
            this.Mainpanel.Controls.Add(this.Stop);
            this.Mainpanel.Controls.Add(this.AdaptorcomboBox);
            this.Mainpanel.Controls.Add(this.portBox);
            this.Mainpanel.Controls.Add(this.UI2);
            this.Mainpanel.Controls.Add(this.Check);
            this.Mainpanel.Controls.Add(this.localeBox);
            this.Mainpanel.Controls.Add(this.SetPath);
            this.Mainpanel.Controls.Add(this.UI3);
            this.Mainpanel.Controls.Add(this.Commit);
            this.Mainpanel.Controls.Add(this.ipBox);
            this.Mainpanel.Location = new System.Drawing.Point(8, 3);
            this.Mainpanel.Name = "Mainpanel";
            this.Mainpanel.Size = new System.Drawing.Size(234, 238);
            this.Mainpanel.TabIndex = 12;
            // 
            // Expanel
            // 
            this.Expanel.Controls.Add(this.RedirectOnlyButton);
            this.Expanel.Controls.Add(this.fixpatchbox);
            this.Expanel.Controls.Add(this.FixPatchlabel);
            this.Expanel.Controls.Add(this.NoPortBox);
            this.Expanel.Controls.Add(this.NoPortlabel);
            this.Expanel.Location = new System.Drawing.Point(249, 4);
            this.Expanel.Name = "Expanel";
            this.Expanel.Size = new System.Drawing.Size(181, 237);
            this.Expanel.TabIndex = 13;
            // 
            // RedirectOnlyButton
            // 
            this.RedirectOnlyButton.Location = new System.Drawing.Point(24, 111);
            this.RedirectOnlyButton.Name = "RedirectOnlyButton";
            this.RedirectOnlyButton.Size = new System.Drawing.Size(93, 31);
            this.RedirectOnlyButton.TabIndex = 4;
            this.RedirectOnlyButton.Text = "RedirectOnly";
            this.RedirectOnlyButton.UseVisualStyleBackColor = true;
            this.RedirectOnlyButton.Click += new System.EventHandler(this.RedirectOnlyButton_Click);
            // 
            // fixpatchbox
            // 
            this.fixpatchbox.Location = new System.Drawing.Point(16, 79);
            this.fixpatchbox.Name = "fixpatchbox";
            this.fixpatchbox.Size = new System.Drawing.Size(100, 21);
            this.fixpatchbox.TabIndex = 3;
            // 
            // FixPatchlabel
            // 
            this.FixPatchlabel.AutoSize = true;
            this.FixPatchlabel.Location = new System.Drawing.Point(22, 58);
            this.FixPatchlabel.Name = "FixPatchlabel";
            this.FixPatchlabel.Size = new System.Drawing.Size(59, 12);
            this.FixPatchlabel.TabIndex = 2;
            this.FixPatchlabel.Text = "FixPatch:";
            // 
            // NoPortBox
            // 
            this.NoPortBox.Location = new System.Drawing.Point(13, 25);
            this.NoPortBox.Name = "NoPortBox";
            this.NoPortBox.Size = new System.Drawing.Size(144, 21);
            this.NoPortBox.TabIndex = 1;
            // 
            // NoPortlabel
            // 
            this.NoPortlabel.AutoSize = true;
            this.NoPortlabel.Location = new System.Drawing.Point(20, 9);
            this.NoPortlabel.Name = "NoPortlabel";
            this.NoPortlabel.Size = new System.Drawing.Size(95, 12);
            this.NoPortlabel.TabIndex = 0;
            this.NoPortlabel.Text = "NoRedirectPort:";
            this.NoPortlabel.Click += new System.EventHandler(this.NoPortlabel_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(460, 275);
            this.Controls.Add(this.Expanel);
            this.Controls.Add(this.Mainpanel);
            this.Controls.Add(this.statusStrip);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.Padding = new System.Windows.Forms.Padding(5, 0, 0, 0);
            this.Text = "mslogin";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainFormFormClosed);
            this.Load += new System.EventHandler(this.MainFormLoad);
            this.Layout += new System.Windows.Forms.LayoutEventHandler(this.MainFormLayout);
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.Mainpanel.ResumeLayout(false);
            this.Mainpanel.PerformLayout();
            this.Expanel.ResumeLayout(false);
            this.Expanel.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		private System.Windows.Forms.TextBox ipBox;
		private System.Windows.Forms.Label UI2;
		private System.Windows.Forms.Label UI3;
		private System.Windows.Forms.ComboBox localeBox;
		private System.Windows.Forms.Label UI1;
		private System.Windows.Forms.ToolStripStatusLabel StatusLabel;
		private System.Windows.Forms.Button Commit;
		private System.Windows.Forms.StatusStrip statusStrip;
		private System.Windows.Forms.ComboBox AdaptorcomboBox;
        private System.Windows.Forms.Button SetPath;
        private System.Windows.Forms.Button Check;
        private System.Windows.Forms.TextBox portBox;
        private System.Windows.Forms.Button Stop;
        private System.Windows.Forms.Panel Mainpanel;
        private System.Windows.Forms.Panel Expanel;
        private System.Windows.Forms.Label NoPortlabel;
        private System.Windows.Forms.TextBox NoPortBox;
        private System.Windows.Forms.TextBox fixpatchbox;
        private System.Windows.Forms.Label FixPatchlabel;
        private System.Windows.Forms.Button RedirectOnlyButton;
	}
}
