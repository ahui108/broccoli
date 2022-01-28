﻿using System;
using System.IO;
using System.Diagnostics;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace logui
{
        

    public partial class logui : Form
    {
        public logui()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            DisplayNoteMessage();
            DisplayVersionInfo();
        }

        [DllImport("HardInfo.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetBordId();

        [DllImport("HardInfo.dll", CallingConvention = CallingConvention.Cdecl)]
        //public static extern int GetHardInfo([Out, MarshalAs(UnmanagedType.LPArray)] char []szHardInfo);
        public static extern int GetHardInfo([MarshalAs(UnmanagedType.LPStr)] StringBuilder szHardInfo);

        private void decrypt_Click(object sender, EventArgs e)
        {
            decrypt.Enabled = false;
            string strInput = this.logFile.Text;
            if (0 == strInput.Length)
            {
                MessageBox.Show("Log file path could not be empty.");
            }
            else if (!System.IO.File.Exists(strInput))
            {
                MessageBox.Show("Log file does not exist.");
            }
            else
            {
                StringBuilder szHardInfo = new StringBuilder(256);
                GetHardInfo(szHardInfo);
                //char[] szHardInfo = new char[256];
                //GetHardInfo(szHardInfo);
                //MessageBox.Show("'" + szHardInfo.ToString().Trim() + "'");

                if (!File.Exists(@".\\lic.dat"))
                {
                    MessageBox.Show("License file does not exist.");
                }
                else
                {
                    byte[] licFileData = null;
                    try
                    {
                        BinaryReader br;
                        FileStream fs = new FileStream(".\\lic.dat", FileMode.Open, FileAccess.Read);
                        br = new BinaryReader(fs);
                        licFileData = br.ReadBytes((int)fs.Length);
                        br.Close();
                        fs.Close();
                    }
                    catch (IOException ex)
                    {
                        MessageBox.Show("Open license file failed, " + ex.Message);
                    }

                    if (licFileData != null && licFileData.Length != 0)
                    {
                        String strLocalPCID = szHardInfo.ToString();
                        if (ValidatePCID(strLocalPCID, licFileData))
                        {
                            if (DecryptFile(strInput))
                                MessageBox.Show("Completed!");
                        }
                        else
                        {
                            MessageBox.Show("You are not authorized to do this operation.");
                        }
                    }
                }
            }
            decrypt.Enabled = true;
        }

         private void browse_Click(object sender, EventArgs e)
        {
            OpenFileDialog fdlg = new OpenFileDialog();
            fdlg.Title = "Select File";
            fdlg.InitialDirectory = @"c:\";   //@是取消转义字符的意思
            //fdlg.Filter = "All files（*.*）|*.*|All files(*.*)|*.* ";
            fdlg.Filter = "All files（*.*）|*.*";
            /*
             * FilterIndex 属性用于选择了何种文件类型,缺省设置为0,系统取Filter属性设置第一项
             * ,相当于FilterIndex 属性设置为1.如果你编了3个文件类型，当FilterIndex ＝2时是指第2个.
             */
            fdlg.FilterIndex = 2;
            /*
             *如果值为false，那么下一次选择文件的初始目录是上一次你选择的那个目录，
             *不固定；如果值为true，每次打开这个对话框初始目录不随你的选择而改变，是固定的  
             */
            //fdlg.RestoreDirectory = true;
            fdlg.RestoreDirectory = false;
            if (fdlg.ShowDialog() == DialogResult.OK)
            {
                this.logFile.Text = fdlg.FileName;
            }
        }

        private void CreateLic_Click(object sender, EventArgs e)
        {
            string strInput = this.logFile.Text;
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
                        MessageBox.Show("License file created.");
                    }
                    catch (IOException ex)
                    {
                        MessageBox.Show(ex.GetType().Name);
                    }
                }
            }
            
        }

        private void DisplayNoteMessage()
        {
            msglabel.Text = "Note: \r\nThis tool is only used internally.And the license file is required to use this tool. Please request a license file before using this tool.";
        }

        private void DisplayVersionInfo()
        {
            versionlabel.Text = "v1.0.0";
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

        public void DecryptPCID(System.IO.MemoryStream encryptedDataStream, ref System.IO.MemoryStream outDataStream)
        {
            //logdg.key(KEY)
            byte[] strIndgKey = { 0x30, 0x82, 0x02, 0x5D, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xC2, 0x9B, 0xF9, 0xBC, 0xE0, 0xA0, 0x15, 0x7D, 0x0B, 0x2E, 0x71, 0x06, 0x48, 0x06, 0xED, 0x4E, 0x0C, 0x74, 0x2F, 0x5A, 0xA6, 0x5B, 0x0B, 0x34, 0xA4, 0x2F, 0x15, 0x1E, 0xC3, 0xEB, 0xCD, 0x83, 0x00, 0x22, 0x11, 0x3B, 0xDA, 0x78, 0xF6, 0xF9, 0x3A, 0xAB, 0xF2, 0x96, 0x16, 0xAE, 0xE2, 0xEC, 0xF1, 0x83, 0x57, 0xA3, 0xB2, 0xC6, 0x6E, 0xAC, 0xF8, 0x3B, 0xF1, 0x99, 0xF9, 0xE0, 0xFF, 0xB6, 0x57, 0x3D, 0xAF, 0xF1, 0x89, 0x0D, 0xCB, 0x09, 0x1A, 0x0A, 0xA7, 0xEA, 0x2F, 0x06, 0xB2, 0xDB, 0xC8, 0xF6, 0xA6, 0x7F, 0xB0, 0xFF, 0xCC, 0x7E, 0x1A, 0x29, 0xA8, 0x49, 0xBD, 0xAF, 0xF4, 0x2C, 0xD5, 0xD1, 0x9C, 0xC7, 0xC0, 0xA7, 0x20, 0x4B, 0x15, 0xC3, 0xC0, 0x75, 0x40, 0xBD, 0x68, 0xC7, 0xD1, 0x6E, 0xF7, 0xA6, 0x24, 0xE8, 0xC1, 0xBF, 0x27, 0x60, 0x37, 0x5F, 0x55, 0x49, 0x2C, 0x19, 0x02, 0x03, 0x01, 0x00, 0x01, 0x02, 0x81, 0x80, 0x4B, 0xE6, 0x39, 0x48, 0x88, 0xF2, 0xE6, 0x6E, 0x96, 0x65, 0xCE, 0x7E, 0xFC, 0xA8, 0x76, 0xF4, 0xF1, 0xB6, 0x7E, 0xA1, 0x84, 0x30, 0x0D, 0xF6, 0xC1, 0xF1, 0xFA, 0xF2, 0xBC, 0xC6, 0xC2, 0xA8, 0x4C, 0xEC, 0xFC, 0x7B, 0xB3, 0x81, 0x6A, 0x11, 0x59, 0xE8, 0xA5, 0xFC, 0x13, 0x6C, 0xB1, 0xE4, 0x67, 0x6D, 0x2A, 0x49, 0x15, 0xF8, 0x7E, 0x41, 0xC7, 0xAE, 0xD9, 0x53, 0x8C, 0x39, 0x4C, 0x81, 0x49, 0xBD, 0xD2, 0x6A, 0xCA, 0x3A, 0xF3, 0x91, 0x4E, 0x10, 0xD5, 0xCD, 0x2B, 0xA1, 0xA2, 0xDE, 0xEB, 0xEA, 0xAC, 0x08, 0x67, 0x52, 0xBE, 0x99, 0xC5, 0xF0, 0x86, 0xFD, 0x7A, 0xE0, 0x80, 0x6F, 0x7B, 0x75, 0x7E, 0x71, 0x03, 0x40, 0x4B, 0x6F, 0x90, 0xA4, 0x13, 0x51, 0x35, 0xAB, 0x86, 0x3A, 0x95, 0x85, 0x1E, 0x19, 0x0E, 0x1E, 0xC5, 0xE2, 0xC6, 0xF9, 0x50, 0xB8, 0x89, 0x4A, 0x05, 0x81, 0x02, 0x41, 0x00, 0xFB, 0x6D, 0x7D, 0x0D, 0x91, 0x19, 0xC8, 0x8A, 0x37, 0x1F, 0x1F, 0x21, 0x88, 0x8A, 0x03, 0xAD, 0xFB, 0x4E, 0x30, 0x18, 0xC5, 0x54, 0x93, 0xDB, 0x8E, 0xC1, 0xA2, 0x6A, 0x91, 0x04, 0x9E, 0x6E, 0xE2, 0xC7, 0xE0, 0xE3, 0x4B, 0x29, 0x82, 0x39, 0x4D, 0xC8, 0x8C, 0x7A, 0xED, 0x93, 0xD6, 0x0E, 0x76, 0xCB, 0xFA, 0xAC, 0xA3, 0x59, 0x8D, 0x15, 0x24, 0x32, 0xAE, 0xF0, 0x4E, 0x57, 0x38, 0x55, 0x02, 0x41, 0x00, 0xC6, 0x25, 0xF8, 0xA2, 0x23, 0xA4, 0xDE, 0x3C, 0x86, 0xC1, 0x30, 0x81, 0x7C, 0x1C, 0xDE, 0x9B, 0x81, 0xC3, 0x77, 0x17, 0xA4, 0xA5, 0x79, 0x17, 0x29, 0x4F, 0x60, 0x08, 0x57, 0x13, 0x62, 0x66, 0xE4, 0xF7, 0x5D, 0xC2, 0xA1, 0x10, 0xD2, 0x12, 0x76, 0x23, 0x10, 0xD4, 0x7E, 0xF1, 0x86, 0xCD, 0xE8, 0x9F, 0x1B, 0x1E, 0xB8, 0xD8, 0xF5, 0xFD, 0x9D, 0x4C, 0x39, 0x54, 0xE2, 0xDC, 0xF8, 0xB5, 0x02, 0x41, 0x00, 0xCA, 0xAC, 0xA6, 0xBA, 0xB1, 0xF5, 0xC3, 0xC7, 0x4E, 0xC9, 0xC2, 0x91, 0x88, 0x7C, 0x20, 0x9D, 0x35, 0xD0, 0x00, 0x4B, 0x67, 0x7E, 0x57, 0x36, 0x0C, 0x6C, 0xA0, 0x9D, 0x20, 0xCB, 0x45, 0x5F, 0x83, 0x65, 0xB6, 0x2D, 0xE2, 0xDF, 0xD1, 0xFE, 0x99, 0x6A, 0x09, 0x7B, 0xEA, 0x59, 0x5A, 0x9B, 0x0C, 0x3E, 0x3F, 0x92, 0xBC, 0x96, 0xAA, 0x78, 0x99, 0x56, 0x55, 0xD8, 0x96, 0x7E, 0x0E, 0x85, 0x02, 0x40, 0x77, 0x91, 0x94, 0x3E, 0x6B, 0x19, 0xE2, 0xCD, 0x4D, 0x57, 0xA8, 0xA8, 0x8A, 0x37, 0x75, 0x91, 0x0B, 0x35, 0xE3, 0x58, 0x8E, 0x0F, 0x18, 0x4A, 0x04, 0xB0, 0x94, 0x37, 0x94, 0x2B, 0xEB, 0x56, 0x41, 0xD6, 0x8F, 0x6A, 0x91, 0xFC, 0xEF, 0x77, 0x1A, 0x78, 0xC0, 0x80, 0x29, 0xF6, 0x1F, 0x2D, 0x83, 0xB0, 0xF7, 0x11, 0x8B, 0xE3, 0xC8, 0x54, 0xF9, 0x25, 0x74, 0xF7, 0x27, 0x4A, 0xC6, 0x7D, 0x02, 0x41, 0x00, 0xAD, 0x13, 0x27, 0x35, 0xC8, 0xEE, 0xDB, 0x67, 0x20, 0x81, 0x08, 0x93, 0x31, 0x85, 0x81, 0x51, 0x95, 0xDE, 0xD3, 0x08, 0xA2, 0xE2, 0x8F, 0xB7, 0xE1, 0x2C, 0x7A, 0x76, 0x23, 0x59, 0x2C, 0xD4, 0xA1, 0xAF, 0xFE, 0x60, 0xC0, 0x85, 0xB7, 0xBC, 0xF0, 0xBF, 0x3B, 0x39, 0x2D, 0x14, 0x9A, 0x46, 0x5E, 0x2F, 0x0F, 0xA5, 0x7A, 0x2E, 0xD0, 0x07, 0x08, 0xF7, 0x6E, 0x62, 0x2E, 0xBE, 0x1D, 0x15 };
            System.IO.MemoryStream bufferStreamInDgKey = new System.IO.MemoryStream(strIndgKey);
            logutil.CDecrypt decDiag = new logutil.CDecrypt();
            decDiag.SetInputOutputDataStream(bufferStreamInDgKey, encryptedDataStream, ref outDataStream);
            decDiag.DecryptDataStream();
        }

        public bool ValidatePCID(string strLocalPCID, byte[] szEncryptedPCID)
        {
            System.IO.MemoryStream outDataStream = new System.IO.MemoryStream();
            System.IO.MemoryStream encryptedPCID = new System.IO.MemoryStream(szEncryptedPCID);
            DecryptPCID(encryptedPCID, ref outDataStream);
            byte[] outDataArray = outDataStream.ToArray();
            if (outDataArray.Length != 0)
            {
                string strOutData = System.Text.Encoding.UTF8.GetString(outDataArray, 0, outDataArray.Length);
                return strOutData.Trim().Equals(strLocalPCID);
            }
            else
            {
                return false;
            }
        }

        public bool DecryptFile(string strInputFilePath)
        {
            string strInputFile = strInputFilePath;
            string strDir = System.IO.Path.GetDirectoryName(strInputFile);
            if (!strDir[strDir.Length - 1].Equals(Path.DirectorySeparatorChar))
            {
                strDir += Path.DirectorySeparatorChar;
            }
            string strInputFileName = System.IO.Path.GetFileName(strInputFile);
            string strOutputFile = strDir + "ARMLog.tbz";
            string strExecPath = System.Windows.Forms.Application.StartupPath;
            string str7zPath = strExecPath + "\\7z.exe";
            Execute("rmdir /S/Q " + strDir + "ARMLOG", 0);
            Execute("mkdir " + strDir + "ARMLOG", 0);

            //log.key(KEY)
            byte[] szInKey = { 0x30, 0x82, 0x02, 0x5B, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xB1, 0x37, 0x16, 0x14, 0x8B, 0xB1, 0x97, 0x39, 0x68, 0x13, 0xCE, 0xF7, 0xDA, 0x3A, 0xC4, 0x30, 0x0B, 0xB7, 0xFC, 0xAF, 0x63, 0xE4, 0x45, 0x60, 0xEC, 0x04, 0x0D, 0xF2, 0xE8, 0x6E, 0x26, 0xE2, 0xFE, 0x94, 0xF1, 0x4C, 0x4F, 0xA1, 0xD7, 0x34, 0xAF, 0x69, 0x78, 0xF9, 0xDC, 0x2F, 0x73, 0xA9, 0xB6, 0x7C, 0x22, 0x57, 0x01, 0x27, 0x7E, 0xB7, 0x97, 0x4B, 0x27, 0x48, 0x5E, 0xF7, 0x64, 0x05, 0x99, 0xCA, 0x79, 0xB2, 0xEB, 0x98, 0x60, 0xA1, 0x84, 0xBE, 0x32, 0x36, 0xB3, 0x70, 0x85, 0xE5, 0x91, 0xC2, 0xC0, 0x30, 0x8E, 0x5B, 0x9B, 0xB9, 0x0F, 0xE2, 0xC7, 0x75, 0xA9, 0xAD, 0xD1, 0x66, 0x51, 0x8F, 0xF3, 0xB7, 0xB3, 0xFE, 0x2B, 0x25, 0x6D, 0xCB, 0xE6, 0x0E, 0xE9, 0xCD, 0xA2, 0xF5, 0x57, 0x8F, 0x99, 0x48, 0x6A, 0x4F, 0xBA, 0x16, 0x5A, 0xA0, 0x4C, 0xAC, 0x14, 0xEC, 0x5D, 0x19, 0x02, 0x03, 0x01, 0x00, 0x01, 0x02, 0x81, 0x80, 0x02, 0x61, 0x2A, 0xFA, 0xBB, 0xA5, 0x76, 0xD2, 0x5E, 0x83, 0x65, 0x94, 0x0E, 0x6A, 0x1B, 0x98, 0x49, 0x8F, 0xFE, 0xE4, 0x9C, 0x02, 0xC4, 0xAB, 0xB8, 0x7F, 0x95, 0x70, 0x5D, 0x8E, 0x9D, 0x17, 0x0F, 0x34, 0x10, 0xC9, 0x65, 0x88, 0x56, 0xCB, 0x6E, 0x4F, 0x8C, 0x40, 0x4E, 0xA4, 0x94, 0x79, 0x02, 0xEA, 0x47, 0xCB, 0xC3, 0x48, 0x34, 0x7B, 0x62, 0x9C, 0xAF, 0xBA, 0x59, 0x0C, 0x19, 0x5B, 0xB4, 0x27, 0x69, 0x78, 0x8B, 0x81, 0x5B, 0xA8, 0xB3, 0x55, 0x60, 0xD5, 0x5E, 0xCD, 0x2F, 0x2B, 0x5F, 0x05, 0xE0, 0xDE, 0xBC, 0xF0, 0x62, 0x1D, 0xF8, 0x92, 0x8E, 0x95, 0x2B, 0xF6, 0x6C, 0x0E, 0x24, 0x14, 0x80, 0x06, 0xE3, 0x78, 0x76, 0x12, 0x2F, 0xCA, 0x6B, 0xF9, 0x5E, 0xD1, 0xB8, 0xAF, 0x29, 0x42, 0x85, 0xC9, 0x12, 0xA8, 0xFD, 0x5F, 0x90, 0xF7, 0xDB, 0xC8, 0x97, 0xF1, 0x08, 0x01, 0x02, 0x41, 0x00, 0xE7, 0xB6, 0x10, 0x05, 0x27, 0xED, 0xFF, 0x3D, 0x9D, 0x0D, 0xF0, 0xFA, 0x9B, 0x1B, 0x3B, 0x46, 0x99, 0x7E, 0x7D, 0x8D, 0xF1, 0xFC, 0x75, 0x1A, 0x21, 0xD1, 0x2C, 0x62, 0x6B, 0x0A, 0x1C, 0xE8, 0x19, 0x20, 0x60, 0x87, 0xAC, 0x89, 0x5C, 0x57, 0xC7, 0xC2, 0xC0, 0x97, 0x11, 0xCC, 0x91, 0x16, 0x6F, 0x37, 0x20, 0xA8, 0x47, 0x6E, 0x36, 0x4D, 0x04, 0x61, 0xB6, 0xA5, 0xB9, 0x8F, 0x64, 0x31, 0x02, 0x41, 0x00, 0xC3, 0xCA, 0xA1, 0x8B, 0xBC, 0x21, 0x50, 0x80, 0x20, 0x1A, 0x18, 0xDF, 0x1C, 0x3E, 0x9B, 0xE9, 0xF7, 0xE7, 0x33, 0x54, 0x1E, 0xF4, 0x5E, 0xF4, 0x41, 0xB2, 0x6E, 0xEF, 0x98, 0x5B, 0x8D, 0xED, 0xE3, 0xE2, 0xBE, 0x6B, 0x85, 0x51, 0x73, 0x44, 0xF6, 0x26, 0x4E, 0x41, 0x80, 0xFE, 0xE3, 0x11, 0x8A, 0xC3, 0xCD, 0xB0, 0x4C, 0x2B, 0xFA, 0x50, 0xCD, 0x1A, 0x4B, 0x6B, 0xAC, 0x58, 0x55, 0x69, 0x02, 0x40, 0x68, 0xE5, 0xA6, 0x28, 0xFF, 0x63, 0xA2, 0x2A, 0x5D, 0x80, 0x63, 0x38, 0xC0, 0xA1, 0x3A, 0x5B, 0x82, 0xBF, 0x9D, 0xCD, 0x75, 0xE0, 0x69, 0x8F, 0x14, 0x02, 0x23, 0x0A, 0xC7, 0x14, 0x83, 0xF3, 0x9A, 0xF4, 0x6A, 0x5E, 0xE1, 0x80, 0x29, 0xDA, 0xDF, 0x67, 0x4E, 0x52, 0xD5, 0xCE, 0xFB, 0x5B, 0xE1, 0xAB, 0x5E, 0x50, 0x70, 0x4D, 0xF7, 0xAD, 0x34, 0x2F, 0x9C, 0x45, 0x38, 0x55, 0x2E, 0xF1, 0x02, 0x40, 0x35, 0x79, 0x23, 0x13, 0xD1, 0xE6, 0xDB, 0x60, 0x92, 0xE3, 0xED, 0xC5, 0x09, 0xA6, 0xC3, 0xD8, 0x6B, 0xBF, 0x76, 0x52, 0x54, 0x3D, 0xF3, 0xAD, 0x0C, 0x40, 0x5A, 0x72, 0x0F, 0x8F, 0x79, 0xBC, 0xEE, 0x0F, 0x60, 0x68, 0x67, 0x39, 0xEE, 0x9C, 0x78, 0x09, 0xD0, 0x37, 0xAD, 0xAE, 0x6F, 0x7F, 0xBC, 0x01, 0x6C, 0x2C, 0xE9, 0x30, 0xDD, 0xA5, 0xB1, 0xCE, 0xAB, 0xA8, 0x95, 0xA2, 0x6E, 0x79, 0x02, 0x40, 0x3A, 0x2C, 0x77, 0x35, 0xCC, 0x08, 0x0E, 0xCA, 0x18, 0x2A, 0x56, 0x84, 0x25, 0x10, 0x3D, 0xE9, 0x35, 0x90, 0x8B, 0xAB, 0x85, 0xA4, 0xED, 0x8F, 0xC0, 0xEC, 0x10, 0xE7, 0xB4, 0x98, 0x6B, 0x90, 0x93, 0x32, 0xFB, 0xFF, 0xB3, 0x37, 0xAE, 0x3B, 0x08, 0x4E, 0x29, 0x79, 0x0A, 0x84, 0xF6, 0xC5, 0x4E, 0x5C, 0x94, 0x46, 0x58, 0x1E, 0x15, 0x12, 0xE1, 0x24, 0xF7, 0x26, 0x34, 0xBD, 0xA8, 0x21 };
            System.IO.MemoryStream bufferStreamInKey = new System.IO.MemoryStream(szInKey);
            logutil.CDecrypt decLog = new logutil.CDecrypt();
            decLog.SetInputOutputFile(bufferStreamInKey, strInputFile, strOutputFile);
            decLog.DecryptFile(); //-> ARMLog.tbz
            if (!File.Exists(strOutputFile))
            {
                MessageBox.Show("Decrypt failed!");
                return false;
            }
            Execute(str7zPath + " e -tgzip " + strDir + "ARMLog.tbz -o"+ strDir + "ARMLOG", 0); //->ARMLOG\ARMLog
            Execute(str7zPath + " e -ttar " + strDir + "ARMLOG\\ARMLog -o" + strDir + "ARMLOG", 0); //->ARMLOG\diags.log

            strInputFile = strDir + "ARMLOG\\diags.log";
            strOutputFile = strDir + "ARMLOG\\diags.tbz";
            if (!File.Exists(strInputFile))
            {
                MessageBox.Show("Decrypt failed!");
                return false;
            }
            //logdg.key(KEY)
            byte[] strIndgKey = { 0x30, 0x82, 0x02, 0x5D, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xC2, 0x9B, 0xF9, 0xBC, 0xE0, 0xA0, 0x15, 0x7D, 0x0B, 0x2E, 0x71, 0x06, 0x48, 0x06, 0xED, 0x4E, 0x0C, 0x74, 0x2F, 0x5A, 0xA6, 0x5B, 0x0B, 0x34, 0xA4, 0x2F, 0x15, 0x1E, 0xC3, 0xEB, 0xCD, 0x83, 0x00, 0x22, 0x11, 0x3B, 0xDA, 0x78, 0xF6, 0xF9, 0x3A, 0xAB, 0xF2, 0x96, 0x16, 0xAE, 0xE2, 0xEC, 0xF1, 0x83, 0x57, 0xA3, 0xB2, 0xC6, 0x6E, 0xAC, 0xF8, 0x3B, 0xF1, 0x99, 0xF9, 0xE0, 0xFF, 0xB6, 0x57, 0x3D, 0xAF, 0xF1, 0x89, 0x0D, 0xCB, 0x09, 0x1A, 0x0A, 0xA7, 0xEA, 0x2F, 0x06, 0xB2, 0xDB, 0xC8, 0xF6, 0xA6, 0x7F, 0xB0, 0xFF, 0xCC, 0x7E, 0x1A, 0x29, 0xA8, 0x49, 0xBD, 0xAF, 0xF4, 0x2C, 0xD5, 0xD1, 0x9C, 0xC7, 0xC0, 0xA7, 0x20, 0x4B, 0x15, 0xC3, 0xC0, 0x75, 0x40, 0xBD, 0x68, 0xC7, 0xD1, 0x6E, 0xF7, 0xA6, 0x24, 0xE8, 0xC1, 0xBF, 0x27, 0x60, 0x37, 0x5F, 0x55, 0x49, 0x2C, 0x19, 0x02, 0x03, 0x01, 0x00, 0x01, 0x02, 0x81, 0x80, 0x4B, 0xE6, 0x39, 0x48, 0x88, 0xF2, 0xE6, 0x6E, 0x96, 0x65, 0xCE, 0x7E, 0xFC, 0xA8, 0x76, 0xF4, 0xF1, 0xB6, 0x7E, 0xA1, 0x84, 0x30, 0x0D, 0xF6, 0xC1, 0xF1, 0xFA, 0xF2, 0xBC, 0xC6, 0xC2, 0xA8, 0x4C, 0xEC, 0xFC, 0x7B, 0xB3, 0x81, 0x6A, 0x11, 0x59, 0xE8, 0xA5, 0xFC, 0x13, 0x6C, 0xB1, 0xE4, 0x67, 0x6D, 0x2A, 0x49, 0x15, 0xF8, 0x7E, 0x41, 0xC7, 0xAE, 0xD9, 0x53, 0x8C, 0x39, 0x4C, 0x81, 0x49, 0xBD, 0xD2, 0x6A, 0xCA, 0x3A, 0xF3, 0x91, 0x4E, 0x10, 0xD5, 0xCD, 0x2B, 0xA1, 0xA2, 0xDE, 0xEB, 0xEA, 0xAC, 0x08, 0x67, 0x52, 0xBE, 0x99, 0xC5, 0xF0, 0x86, 0xFD, 0x7A, 0xE0, 0x80, 0x6F, 0x7B, 0x75, 0x7E, 0x71, 0x03, 0x40, 0x4B, 0x6F, 0x90, 0xA4, 0x13, 0x51, 0x35, 0xAB, 0x86, 0x3A, 0x95, 0x85, 0x1E, 0x19, 0x0E, 0x1E, 0xC5, 0xE2, 0xC6, 0xF9, 0x50, 0xB8, 0x89, 0x4A, 0x05, 0x81, 0x02, 0x41, 0x00, 0xFB, 0x6D, 0x7D, 0x0D, 0x91, 0x19, 0xC8, 0x8A, 0x37, 0x1F, 0x1F, 0x21, 0x88, 0x8A, 0x03, 0xAD, 0xFB, 0x4E, 0x30, 0x18, 0xC5, 0x54, 0x93, 0xDB, 0x8E, 0xC1, 0xA2, 0x6A, 0x91, 0x04, 0x9E, 0x6E, 0xE2, 0xC7, 0xE0, 0xE3, 0x4B, 0x29, 0x82, 0x39, 0x4D, 0xC8, 0x8C, 0x7A, 0xED, 0x93, 0xD6, 0x0E, 0x76, 0xCB, 0xFA, 0xAC, 0xA3, 0x59, 0x8D, 0x15, 0x24, 0x32, 0xAE, 0xF0, 0x4E, 0x57, 0x38, 0x55, 0x02, 0x41, 0x00, 0xC6, 0x25, 0xF8, 0xA2, 0x23, 0xA4, 0xDE, 0x3C, 0x86, 0xC1, 0x30, 0x81, 0x7C, 0x1C, 0xDE, 0x9B, 0x81, 0xC3, 0x77, 0x17, 0xA4, 0xA5, 0x79, 0x17, 0x29, 0x4F, 0x60, 0x08, 0x57, 0x13, 0x62, 0x66, 0xE4, 0xF7, 0x5D, 0xC2, 0xA1, 0x10, 0xD2, 0x12, 0x76, 0x23, 0x10, 0xD4, 0x7E, 0xF1, 0x86, 0xCD, 0xE8, 0x9F, 0x1B, 0x1E, 0xB8, 0xD8, 0xF5, 0xFD, 0x9D, 0x4C, 0x39, 0x54, 0xE2, 0xDC, 0xF8, 0xB5, 0x02, 0x41, 0x00, 0xCA, 0xAC, 0xA6, 0xBA, 0xB1, 0xF5, 0xC3, 0xC7, 0x4E, 0xC9, 0xC2, 0x91, 0x88, 0x7C, 0x20, 0x9D, 0x35, 0xD0, 0x00, 0x4B, 0x67, 0x7E, 0x57, 0x36, 0x0C, 0x6C, 0xA0, 0x9D, 0x20, 0xCB, 0x45, 0x5F, 0x83, 0x65, 0xB6, 0x2D, 0xE2, 0xDF, 0xD1, 0xFE, 0x99, 0x6A, 0x09, 0x7B, 0xEA, 0x59, 0x5A, 0x9B, 0x0C, 0x3E, 0x3F, 0x92, 0xBC, 0x96, 0xAA, 0x78, 0x99, 0x56, 0x55, 0xD8, 0x96, 0x7E, 0x0E, 0x85, 0x02, 0x40, 0x77, 0x91, 0x94, 0x3E, 0x6B, 0x19, 0xE2, 0xCD, 0x4D, 0x57, 0xA8, 0xA8, 0x8A, 0x37, 0x75, 0x91, 0x0B, 0x35, 0xE3, 0x58, 0x8E, 0x0F, 0x18, 0x4A, 0x04, 0xB0, 0x94, 0x37, 0x94, 0x2B, 0xEB, 0x56, 0x41, 0xD6, 0x8F, 0x6A, 0x91, 0xFC, 0xEF, 0x77, 0x1A, 0x78, 0xC0, 0x80, 0x29, 0xF6, 0x1F, 0x2D, 0x83, 0xB0, 0xF7, 0x11, 0x8B, 0xE3, 0xC8, 0x54, 0xF9, 0x25, 0x74, 0xF7, 0x27, 0x4A, 0xC6, 0x7D, 0x02, 0x41, 0x00, 0xAD, 0x13, 0x27, 0x35, 0xC8, 0xEE, 0xDB, 0x67, 0x20, 0x81, 0x08, 0x93, 0x31, 0x85, 0x81, 0x51, 0x95, 0xDE, 0xD3, 0x08, 0xA2, 0xE2, 0x8F, 0xB7, 0xE1, 0x2C, 0x7A, 0x76, 0x23, 0x59, 0x2C, 0xD4, 0xA1, 0xAF, 0xFE, 0x60, 0xC0, 0x85, 0xB7, 0xBC, 0xF0, 0xBF, 0x3B, 0x39, 0x2D, 0x14, 0x9A, 0x46, 0x5E, 0x2F, 0x0F, 0xA5, 0x7A, 0x2E, 0xD0, 0x07, 0x08, 0xF7, 0x6E, 0x62, 0x2E, 0xBE, 0x1D, 0x15 };
            System.IO.MemoryStream bufferStreamInDgKey = new System.IO.MemoryStream(strIndgKey);
            logutil.CDecrypt decDiag = new logutil.CDecrypt();
            decDiag.SetInputOutputFile(bufferStreamInDgKey, strInputFile, strOutputFile);
            decDiag.DecryptFile();//-> ARMLOG\diags.tbz
            if (!File.Exists(strOutputFile))
            {
                MessageBox.Show("Decrypt failed!");
                return false;
            }

            Execute("mkdir " + strDir + "ARMLOG\\diags_log", 0);
            Execute(str7zPath + " e -tgzip " + strDir + "ARMLOG\\diags.tbz -o" + strDir + "ARMLOG\\diags_log", 0);//->ARMLOG\diags_log\diags
            Execute(str7zPath + " e -ttar " + strDir + "ARMLOG\\diags_log\\diags -o" + strDir + "ARMLOG\\diags_log", 0);

            Execute("del /Q " + strDir + "ARMLog.tbz", 0);
            Execute("del /Q " + strDir + "ARMLOG\\ARMLog", 0);
            Execute("del /Q " + strDir + "ARMLOG\\diags.log", 0);
            Execute("del /Q " + strDir + "ARMLOG\\diags.tbz", 0);
            Execute("del /Q/S " + strDir + "ARMLOG\\diags_log\\diags", 0);
            return true;
        }

        public static string Execute(string command, int seconds)
        {
         string output = ""; //输出字符串
         //MessageBox.Show(command);
         if (command != null && !command.Equals(""))
         {
            Process process = new Process();//创建进程对象  
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.FileName = "cmd.exe";//设定需要执行的命令  
            startInfo.Arguments = "/C " + command;//“/C”表示执行完命令后马上退出  
            startInfo.UseShellExecute = false;//不使用系统外壳程序启动 
            startInfo.RedirectStandardInput = false;//不重定向输入  
            startInfo.RedirectStandardOutput = true; //重定向输出  
            startInfo.CreateNoWindow = true;//不创建窗口  
            process.StartInfo = startInfo;
            try
            {
               if (process.Start())//开始进程  
               {
                  if (seconds == 0)
                  {
                     process.WaitForExit();//这里无限等待进程结束  
                  }
                  else
                  {
                     process.WaitForExit(seconds); //等待进程结束，等待时间为指定的毫秒  
                  }
                  output = process.StandardOutput.ReadToEnd();//读取进程的输出 
                  //MessageBox.Show(output);
               }
            }
            catch(Exception ex)
            {
               Console.WriteLine(ex.Message);//捕获异常，输出异常信息
            }
            finally
            {
               if (process != null)
                  process.Close();
            }
         }
         return output;
       }
    }
}
