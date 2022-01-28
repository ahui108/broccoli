namespace logui
{
    partial class logui
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(logui));
            this.fileInput = new System.Windows.Forms.Label();
            this.logFile = new System.Windows.Forms.TextBox();
            this.browse = new System.Windows.Forms.Button();
            this.decrypt = new System.Windows.Forms.Button();
            this.msglabel = new System.Windows.Forms.Label();
            this.versionlabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // fileInput
            // 
            this.fileInput.AutoSize = true;
            this.fileInput.Location = new System.Drawing.Point(8, 30);
            this.fileInput.Name = "fileInput";
            this.fileInput.Size = new System.Drawing.Size(59, 12);
            this.fileInput.TabIndex = 0;
            this.fileInput.Text = "Log File:";
            // 
            // logFile
            // 
            this.logFile.Location = new System.Drawing.Point(65, 27);
            this.logFile.MaxLength = 1024;
            this.logFile.Name = "logFile";
            this.logFile.Size = new System.Drawing.Size(342, 21);
            this.logFile.TabIndex = 1;
            // 
            // browse
            // 
            this.browse.Location = new System.Drawing.Point(410, 25);
            this.browse.Name = "browse";
            this.browse.Size = new System.Drawing.Size(75, 23);
            this.browse.TabIndex = 2;
            this.browse.Text = "Browse...";
            this.browse.UseVisualStyleBackColor = true;
            this.browse.Click += new System.EventHandler(this.browse_Click);
            // 
            // decrypt
            // 
            this.decrypt.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.decrypt.Location = new System.Drawing.Point(184, 83);
            this.decrypt.Name = "decrypt";
            this.decrypt.Size = new System.Drawing.Size(118, 37);
            this.decrypt.TabIndex = 3;
            this.decrypt.Text = "Decrypt";
            this.decrypt.UseVisualStyleBackColor = true;
            this.decrypt.Click += new System.EventHandler(this.decrypt_Click);
            // 
            // msglabel
            // 
            this.msglabel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.msglabel.Location = new System.Drawing.Point(10, 160);
            this.msglabel.Margin = new System.Windows.Forms.Padding(3);
            this.msglabel.Name = "msglabel";
            this.msglabel.Padding = new System.Windows.Forms.Padding(5);
            this.msglabel.Size = new System.Drawing.Size(478, 50);
            this.msglabel.TabIndex = 4;
            this.msglabel.Text = "Note:";
            // 
            // versionlabel
            // 
            this.versionlabel.AutoSize = true;
            this.versionlabel.Location = new System.Drawing.Point(444, 143);
            this.versionlabel.Name = "versionlabel";
            this.versionlabel.Size = new System.Drawing.Size(41, 12);
            this.versionlabel.TabIndex = 5;
            this.versionlabel.Text = "v1.0.0";
            // 
            // logui
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(500, 224);
            this.Controls.Add(this.versionlabel);
            this.Controls.Add(this.msglabel);
            this.Controls.Add(this.decrypt);
            this.Controls.Add(this.browse);
            this.Controls.Add(this.logFile);
            this.Controls.Add(this.fileInput);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "logui";
            this.Text = "logutilx";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label fileInput;
        private System.Windows.Forms.TextBox logFile;
        private System.Windows.Forms.Button browse;
        private System.Windows.Forms.Button decrypt;
        private System.Windows.Forms.Label msglabel;
        private System.Windows.Forms.Label versionlabel;
    }
}

