namespace mslogin
{
    partial class ExtendPage
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.m_btnRedirectOnly = new System.Windows.Forms.Button();
            this.m_Fixpatchbox = new System.Windows.Forms.TextBox();
            this.m_labFixPatch = new System.Windows.Forms.Label();
            this.m_NoPortBox = new System.Windows.Forms.TextBox();
            this.m_labNoPort = new System.Windows.Forms.Label();
            this.Expanel = new System.Windows.Forms.Panel();
            this.Expanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // m_btnRedirectOnly
            // 
            this.m_btnRedirectOnly.Location = new System.Drawing.Point(24, 111);
            this.m_btnRedirectOnly.Name = "m_btnRedirectOnly";
            this.m_btnRedirectOnly.Size = new System.Drawing.Size(93, 31);
            this.m_btnRedirectOnly.TabIndex = 4;
            this.m_btnRedirectOnly.Text = "RedirectOnly";
            this.m_btnRedirectOnly.UseVisualStyleBackColor = true;
            this.m_btnRedirectOnly.Click += new System.EventHandler(this.RedirectOnly_OnClick);
            // 
            // m_Fixpatchbox
            // 
            this.m_Fixpatchbox.Location = new System.Drawing.Point(16, 79);
            this.m_Fixpatchbox.Name = "m_Fixpatchbox";
            this.m_Fixpatchbox.Size = new System.Drawing.Size(100, 21);
            this.m_Fixpatchbox.TabIndex = 3;
            // 
            // m_labFixPatch
            // 
            this.m_labFixPatch.AutoSize = true;
            this.m_labFixPatch.Location = new System.Drawing.Point(22, 58);
            this.m_labFixPatch.Name = "m_labFixPatch";
            this.m_labFixPatch.Size = new System.Drawing.Size(59, 12);
            this.m_labFixPatch.TabIndex = 2;
            this.m_labFixPatch.Text = "FixPatch:";
            // 
            // m_NoPortBox
            // 
            this.m_NoPortBox.Location = new System.Drawing.Point(13, 25);
            this.m_NoPortBox.Name = "m_NoPortBox";
            this.m_NoPortBox.Size = new System.Drawing.Size(144, 21);
            this.m_NoPortBox.TabIndex = 1;
            // 
            // m_labNoPort
            // 
            this.m_labNoPort.AutoSize = true;
            this.m_labNoPort.Location = new System.Drawing.Point(20, 9);
            this.m_labNoPort.Name = "m_labNoPort";
            this.m_labNoPort.Size = new System.Drawing.Size(95, 12);
            this.m_labNoPort.TabIndex = 0;
            this.m_labNoPort.Text = "NoRedirectPort:";
            // 
            // Expanel
            // 
            this.Expanel.Controls.Add(this.m_btnRedirectOnly);
            this.Expanel.Controls.Add(this.m_Fixpatchbox);
            this.Expanel.Controls.Add(this.m_labFixPatch);
            this.Expanel.Controls.Add(this.m_NoPortBox);
            this.Expanel.Controls.Add(this.m_labNoPort);
            this.Expanel.Location = new System.Drawing.Point(12, 12);
            this.Expanel.Name = "Expanel";
            this.Expanel.Size = new System.Drawing.Size(171, 237);
            this.Expanel.TabIndex = 14;
            // 
            // ExtendPage
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(201, 258);
            this.ControlBox = false;
            this.Controls.Add(this.Expanel);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "ExtendPage";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "ExtendPage";
            this.Load += new System.EventHandler(this.ExtendPage_Load);
            this.Expanel.ResumeLayout(false);
            this.Expanel.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        public System.Windows.Forms.Button m_btnRedirectOnly;
        public System.Windows.Forms.TextBox m_Fixpatchbox;
        public System.Windows.Forms.Label m_labFixPatch;
        public System.Windows.Forms.TextBox m_NoPortBox;
        public System.Windows.Forms.Label m_labNoPort;
        private System.Windows.Forms.Panel Expanel;
        private MainForm m_MainPage;
    }
}