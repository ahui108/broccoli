using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace logadm
{
    public partial class logadm : Form
    {
        public logadm()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            DisplayVersionInfo();
        }

        private void CreateLic_Click(object sender, EventArgs e)
        {
            CreateLic.Enabled = false;
            string strInput = this.pcid.Text;
            if (0 == strInput.Length)
            {
                MessageBox.Show("PCID could not be empty.");
            }
            else
            {
                System.IO.MemoryStream encryptedPCIDData = new System.IO.MemoryStream();
                EncryptPCID(strInput, ref encryptedPCIDData);
                if (encryptedPCIDData.Length != 0)
                {
                    BinaryWriter binWriter = new BinaryWriter(new FileStream(".\\lic.dat", FileMode.Create));
                    try
                    {
                        binWriter.Write(encryptedPCIDData.ToArray());
                        binWriter.Flush();
                        binWriter.Close();
                        MessageBox.Show("License file created!");
                    }
                    catch (IOException ex)
                    {
                        MessageBox.Show(ex.GetType().Name);
                    }
                }
            }
            CreateLic.Enabled = true;
        }

        public void EncryptPCID(string strLocalPCID, ref System.IO.MemoryStream outDataStream)
        {
            //armdgpub.ky(PEM)
            String strIndgKey = "-----BEGIN PUBLIC KEY-----\r\n" +
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDCm/m84KAVfQsucQZIBu1ODHQv\r\n" +
"WqZbCzSkLxUew+vNgwAiETvaePb5Oqvylhau4uzxg1ejssZurPg78Zn54P+2Vz2v\r\n" +
"8YkNywkaCqfqLway28j2pn+w/8x+GimoSb2v9CzV0ZzHwKcgSxXDwHVAvWjH0W73\r\n" +
"piTowb8nYDdfVUksGQIDAQAB\r\n" +
"-----END PUBLIC KEY-----\r\n";
            logutil.CDecrypt decDiag = new logutil.CDecrypt();
            System.IO.MemoryStream inDataStream = new System.IO.MemoryStream(System.Text.Encoding.ASCII.GetBytes(strLocalPCID));
            System.IO.MemoryStream indgKeyStream = new System.IO.MemoryStream(System.Text.Encoding.ASCII.GetBytes(strIndgKey));
            decDiag.SetInputOutputDataStream(indgKeyStream, inDataStream, ref outDataStream);
            decDiag.EncryptDataStream();
        }

        private void DisplayVersionInfo()
        {
            versionlabel.Text = "v1.0.0";
        }
    }
}
