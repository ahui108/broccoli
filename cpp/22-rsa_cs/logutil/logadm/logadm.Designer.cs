namespace logadm
{
    partial class logadm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(logadm));
            this.CreateLic = new System.Windows.Forms.Button();
            this.pcid = new System.Windows.Forms.TextBox();
            this.fileInput = new System.Windows.Forms.Label();
            this.versionlabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // CreateLic
            // 
            this.CreateLic.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.CreateLic.Location = new System.Drawing.Point(162, 78);
            this.CreateLic.Name = "CreateLic";
            this.CreateLic.Size = new System.Drawing.Size(104, 38);
            this.CreateLic.TabIndex = 7;
            this.CreateLic.Text = "Create";
            this.CreateLic.UseVisualStyleBackColor = true;
            this.CreateLic.Click += new System.EventHandler(this.CreateLic_Click);
            // 
            // pcid
            // 
            this.pcid.Location = new System.Drawing.Point(71, 28);
            this.pcid.MaxLength = 1024;
            this.pcid.Name = "pcid";
            this.pcid.Size = new System.Drawing.Size(342, 21);
            this.pcid.TabIndex = 6;
            // 
            // fileInput
            // 
            this.fileInput.AutoSize = true;
            this.fileInput.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.fileInput.Location = new System.Drawing.Point(35, 31);
            this.fileInput.Name = "fileInput";
            this.fileInput.Size = new System.Drawing.Size(35, 12);
            this.fileInput.TabIndex = 5;
            this.fileInput.Text = "PCID:";
            // 
            // versionlabel
            // 
            this.versionlabel.AutoSize = true;
            this.versionlabel.Location = new System.Drawing.Point(393, 126);
            this.versionlabel.Name = "versionlabel";
            this.versionlabel.Size = new System.Drawing.Size(41, 12);
            this.versionlabel.TabIndex = 8;
            this.versionlabel.Text = "v1.0.0";
            // 
            // logadm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(446, 147);
            this.Controls.Add(this.versionlabel);
            this.Controls.Add(this.CreateLic);
            this.Controls.Add(this.pcid);
            this.Controls.Add(this.fileInput);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "logadm";
            this.Text = "logadm";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button CreateLic;
        private System.Windows.Forms.TextBox pcid;
        private System.Windows.Forms.Label fileInput;
        private System.Windows.Forms.Label versionlabel;
    }
}

