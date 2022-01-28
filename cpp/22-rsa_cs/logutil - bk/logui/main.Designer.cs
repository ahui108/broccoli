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
            this.fileInput = new System.Windows.Forms.Label();
            this.logFile = new System.Windows.Forms.TextBox();
            this.browse = new System.Windows.Forms.Button();
            this.decrypt = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // fileInput
            // 
            this.fileInput.AutoSize = true;
            this.fileInput.Location = new System.Drawing.Point(30, 36);
            this.fileInput.Name = "fileInput";
            this.fileInput.Size = new System.Drawing.Size(29, 12);
            this.fileInput.TabIndex = 0;
            this.fileInput.Text = "File";
            // 
            // logFile
            // 
            this.logFile.Location = new System.Drawing.Point(65, 31);
            this.logFile.Name = "logFile";
            this.logFile.Size = new System.Drawing.Size(342, 21);
            this.logFile.TabIndex = 1;
            // 
            // browse
            // 
            this.browse.Location = new System.Drawing.Point(413, 31);
            this.browse.Name = "browse";
            this.browse.Size = new System.Drawing.Size(75, 23);
            this.browse.TabIndex = 2;
            this.browse.Text = "Browse...";
            this.browse.UseVisualStyleBackColor = true;
            this.browse.Click += new System.EventHandler(this.browse_Click);
            // 
            // decrypt
            // 
            this.decrypt.Location = new System.Drawing.Point(65, 93);
            this.decrypt.Name = "decrypt";
            this.decrypt.Size = new System.Drawing.Size(75, 23);
            this.decrypt.TabIndex = 3;
            this.decrypt.Text = "Decrypt";
            this.decrypt.UseVisualStyleBackColor = true;
            this.decrypt.Click += new System.EventHandler(this.decrypt_Click);
            // 
            // logui
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(500, 262);
            this.Controls.Add(this.decrypt);
            this.Controls.Add(this.browse);
            this.Controls.Add(this.logFile);
            this.Controls.Add(this.fileInput);
            this.Name = "logui";
            this.Text = "logutl";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label fileInput;
        private System.Windows.Forms.TextBox logFile;
        private System.Windows.Forms.Button browse;
        private System.Windows.Forms.Button decrypt;
    }
}

