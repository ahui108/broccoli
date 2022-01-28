﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;

namespace logutil
{
    public class CDecrypt
    {
        private bool m_bDecrypt = false;
        private bool m_bShowHelp = false;
        private string m_inkey = "";
        private System.IO.MemoryStream m_inKeyStream;
        private string m_inputFile = "";
        private string m_outputFile = "";
        private System.IO.MemoryStream m_inDataStream;
        private System.IO.MemoryStream m_outDataStream;

        public CDecrypt()
        {
            this.m_inkey = "";
            this.m_inputFile = "";
            this.m_outputFile = "";
        }

        public void SetInputOutputFile(System.IO.MemoryStream inkeyStream, string strInputFile, string strOutputFile)
        {
            m_inKeyStream = inkeyStream;
            m_inputFile = strInputFile;
            m_outputFile = strOutputFile;
        }

        public void SetInputOutputDataStream(System.IO.MemoryStream inkeyStream, System.IO.MemoryStream inDataStream, ref System.IO.MemoryStream outDataStream)
        {
            m_inKeyStream = inkeyStream;
            m_inDataStream = inDataStream;
            m_outDataStream = outDataStream;
        }

        private void Help()
        {
            string msg = @"Usage: logutil.bin [OPTION]
Decrypt file with rsa key pair(pem).

Valid options are:
-help,--help    Display this summary
-decrypt        Decrypt with key file
-inkey keyfile  Input key file
-in infile      Input file
-out outfile    Output file";
            Console.WriteLine(msg);
        }

        public void ParseArgs(string[] args)
        {
            if (0 == args.Length)
            {
                m_bShowHelp = true;
            }
            else
            {
                for (int i = 0; i < args.Length; i++)
                {
                    if (0 == args[i].CompareTo("-decrypt"))
                    {
                        m_bDecrypt = true;
                    }
                    else if (0 == args[i].CompareTo("-inkey"))
                    {
                        i++;
                        if (i < args.Length)
                        {
                            m_inkey = args[i];
                            if (!File.Exists(m_inkey))
                            {
                                m_bDecrypt = false;
                                Console.WriteLine("The key file does not exist.");
                                break;
                            }
                        }
                        else
                        {
                            Console.WriteLine("Argument for '-inkey' is required.\n");
                            m_bShowHelp = true;
                        }
                    }
                    else if (0 == args[i].CompareTo("-in"))
                    {
                        i++;
                        if (i < args.Length)
                        {
                            m_inputFile = args[i];
                            if (!File.Exists(m_inputFile))
                            {
                                m_bDecrypt = false;
                                Console.WriteLine("The input file does not exist.\n");
                                break;
                            }
                        }
                        else
                        {
                            Console.WriteLine("Argument for '-in' is required.\n");
                            m_bShowHelp = true;
                        }

                    }
                    else if (0 == args[i].CompareTo("-out"))
                    {
                        i++;
                        if (i < args.Length)
                        {
                            m_outputFile = args[i];
                        }
                        else 
                        {
                            Console.WriteLine("Argument for '-out' is required.\n");
                            m_bShowHelp = true;
                        }
                    }
                    else if (0 == args[i].CompareTo("-help") || 0 == args[i].CompareTo("--help"))
                    {
                        m_bShowHelp = true;
                        break;
                    }
                }
            }

            if (!m_bDecrypt)
            {
                m_bShowHelp = true;
            }
            else if (!m_bShowHelp)
            {
                if (0 == m_outputFile.Length)
                {
                    Console.WriteLine("Output file is required.\n");
                    m_bShowHelp = true;
                }
                else if (0 == m_inputFile.Length)
                {
                    Console.WriteLine("Input file is required.\n");
                    m_bShowHelp = true;
                }
            }

            if (m_bShowHelp)
            {
                Help();
                m_bDecrypt = false;
            }
        }

        private static int GetIntegerSize(BinaryReader binr)
        {
            int count;
            var bt = binr.ReadByte();
            if (bt != 0x02)        //expect integer
                return 0;
            bt = binr.ReadByte();


            if (bt == 0x81)
                count = binr.ReadByte();    // data size in next byte
            else
                if (bt == 0x82)
                {
                    var highbyte = binr.ReadByte();
                    var lowbyte = binr.ReadByte();
                    byte[] modint = { lowbyte, highbyte, 0x00, 0x00 };
                    count = BitConverter.ToInt32(modint, 0);
                }
                else
                {
                    count = bt;        // we already have the data size
                }


            while (binr.ReadByte() == 0x00)
            {   //remove high order zeros in data
                count -= 1;
            }
            binr.BaseStream.Seek(-1, SeekOrigin.Current);        //last ReadByte wasn't a removed zero, so back up a byte
            return count;
        }

        public RSACryptoServiceProvider DecodeRsaPrivateKey(string filePath)
        {

            byte[] MODULUS, E, D, P, Q, DP, DQ, IQ;

            //FileStream fs = new FileStream(filePath, FileMode.Open, FileAccess.Read);
            BinaryReader binr = new BinaryReader(m_inKeyStream);    //wrap Memory Stream with BinaryReader for easy reading
            try
            {
                var twobytes = binr.ReadUInt16();
                if (twobytes == 0x8130)     //data read as little endian order (actual data order for Sequence is 30 81)
                    binr.ReadByte();        //advance 1 byte
                else if (twobytes == 0x8230)
                    binr.ReadInt16();       //advance 2 bytes
                else
                    return null;


                twobytes = binr.ReadUInt16();
                if (twobytes != 0x0102) //version number
                    return null;
                var bt = binr.ReadByte();
                if (bt != 0x00)
                    return null;

                var elems = GetIntegerSize(binr);
                MODULUS = binr.ReadBytes(elems);


                elems = GetIntegerSize(binr);
                E = binr.ReadBytes(elems);


                elems = GetIntegerSize(binr);
                D = binr.ReadBytes(elems);


                elems = GetIntegerSize(binr);
                P = binr.ReadBytes(elems);


                elems = GetIntegerSize(binr);
                Q = binr.ReadBytes(elems);


                elems = GetIntegerSize(binr);
                DP = binr.ReadBytes(elems);


                elems = GetIntegerSize(binr);
                DQ = binr.ReadBytes(elems);


                elems = GetIntegerSize(binr);
                IQ = binr.ReadBytes(elems);
                // ------- create RSACryptoServiceProvider instance and initialize with public key -----
                RSACryptoServiceProvider rsa = new RSACryptoServiceProvider();
                RSAParameters rsAparams = new RSAParameters();
                rsAparams.Modulus = MODULUS;
                rsAparams.Exponent = E;
                rsAparams.D = D;
                rsAparams.P = P;
                rsAparams.Q = Q;
                rsAparams.DP = DP;
                rsAparams.DQ = DQ;
                rsAparams.InverseQ = IQ;
                rsa.ImportParameters(rsAparams);
                return rsa;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message + e.StackTrace);
                return null;
            }
            finally
            {
                binr.Close();
            }
        }

        public string PrivateKeyDecXml(string strDerKey)
        {
            RSACryptoServiceProvider rsaProvider = DecodeRsaPrivateKey(strDerKey);
            var privateKey = rsaProvider.ToXmlString(true);
            return privateKey;
        }

        private static RSAParameters ConvertFromPublicKey(string pemFileConent)
        {

            if (string.IsNullOrEmpty(pemFileConent))
            {
                throw new ArgumentNullException("pemFileConent", "This arg cann't be empty.");
            }
            pemFileConent = pemFileConent.Replace("-----BEGIN PUBLIC KEY-----", "").Replace("-----END PUBLIC KEY-----", "").Replace("\n", "").Replace("\r", "");
            byte[] keyData = Convert.FromBase64String(pemFileConent);
            bool keySize1024 = (keyData.Length == 162);
            bool keySize2048 = (keyData.Length == 294);
            if (!(keySize1024 || keySize2048))
            {
                throw new ArgumentException("pem file content is incorrect, Only support the key size is 1024 or 2048");
            }
            byte[] pemModulus = (keySize1024 ? new byte[128] : new byte[256]);
            byte[] pemPublicExponent = new byte[3];
            Array.Copy(keyData, (keySize1024 ? 29 : 33), pemModulus, 0, (keySize1024 ? 128 : 256));
            Array.Copy(keyData, (keySize1024 ? 159 : 291), pemPublicExponent, 0, 3);
            RSAParameters para = new RSAParameters();
            para.Modulus = pemModulus;
            para.Exponent = pemPublicExponent;
            return para;
        }

        public void EncryptDataStream()
        {
            byte []outData;
            byte[] b = m_inKeyStream.ToArray();
            string strPemKey = System.Text.Encoding.UTF8.GetString(b, 0, b.Length);
            outData = RSAEncrypt(m_inDataStream.ToArray(), strPemKey, false);
            new System.IO.MemoryStream(outData).CopyTo(m_outDataStream);
        }

        public void DecryptDataStream()
        {
            byte[] outData;
            string keyxml = PrivateKeyDecXml(m_inkey);
            outData = RSADecrypt(m_inDataStream.ToArray(), keyxml, false);
            if (outData != null && outData.Length != 0)
                new System.IO.MemoryStream(outData).CopyTo(m_outDataStream);
        }

        public void DecryptFile()
        {
            FileStream inFileStream = new FileStream(m_inputFile, FileMode.Open, FileAccess.Read);
            byte[] buffer = new byte[128];
            int len = inFileStream.Read(buffer, 0, 128);
            //Console.WriteLine("read input file length:{0}", len);

            byte[] decryptedData;
            string keyxml = PrivateKeyDecXml(m_inkey);
            decryptedData = RSADecrypt(buffer, keyxml, false);
            
            if (decryptedData != null)
            { 
                FileStream outFileStream = new FileStream(m_outputFile, FileMode.Create, FileAccess.Write);
                outFileStream.Write(decryptedData, 0, decryptedData.Length);
                long length = inFileStream.Length;
                if (length > 128)
                {
                    inFileStream.CopyTo(outFileStream, (int)(length - 128));
                }
                outFileStream.Close();
            }
            inFileStream.Close();
        }

        public byte[] RSADecrypt(byte[] DataToDecrypt, string strKeyXML, bool DoOAEPPadding)
        {
            byte[] decryptedData;
            try
            {
                //Create a new instance of RSACryptoServiceProvider.
                using (RSACryptoServiceProvider RSA = new RSACryptoServiceProvider())
                {
                    //Import the RSA Key information. This needs to include the private key information.
                    //RSA.ImportParameters(RSAKeyInfo);
                    RSA.FromXmlString(strKeyXML);

                    //Decrypt the passed byte array and specify OAEP padding.  
                    //OAEP padding is only available on Microsoft Windows XP or later.  
                    decryptedData = RSA.Decrypt(DataToDecrypt, DoOAEPPadding);
                }
                return decryptedData;
            }
            //Catch and display a CryptographicException to the console.
            catch (CryptographicException e)
            {
                Console.WriteLine(e.ToString());

                return null;
            }
        }

        public byte[] RSAEncrypt(byte[] DataToEncrypt, string strPemKey, bool DoOAEPPadding)
        {
            try
            {
                byte[] encryptedData;
                //Create a new instance of RSACryptoServiceProvider.
                using (RSACryptoServiceProvider RSA = new RSACryptoServiceProvider())
                {

                    //Import the RSA Key information. This only needs
                    //toinclude the public key information.
                    //RSA.ImportParameters(RSAKeyInfo);
                    //RSA.FromXmlString(strKeyXML);
                    //var pub = Convert.FromBase64String(strPemKey);
                    //RSAParameters myRSAParameters = RSA.ExportParameters(false);
                   
                    RSAParameters myRSAParameters = ConvertFromPublicKey(strPemKey);
                    RSA.ImportParameters(myRSAParameters);

                    //Encrypt the passed byte array and specify OAEP padding.  
                    //OAEP padding is only available on Microsoft Windows XP or
                    //later.  
                    encryptedData = RSA.Encrypt(DataToEncrypt, DoOAEPPadding);
                }
                return encryptedData;
            }
            //Catch and display a CryptographicException  
            //to the console.
            catch (CryptographicException e)
            {
                Console.WriteLine(e.Message);

                return null;
            }
        }

        public byte[] RSAEncrypt(byte[] DataToEncrypt, RSAParameters RSAKeyInfo, bool DoOAEPPadding)
        {
            try
            {
                byte[] encryptedData;
                //Create a new instance of RSACryptoServiceProvider.
                using (RSACryptoServiceProvider RSA = new RSACryptoServiceProvider())
                {

                    //Import the RSA Key information. This only needs
                    //toinclude the public key information.
                    RSA.ImportParameters(RSAKeyInfo);

                    //Encrypt the passed byte array and specify OAEP padding.  
                    //OAEP padding is only available on Microsoft Windows XP or
                    //later.  
                    encryptedData = RSA.Encrypt(DataToEncrypt, DoOAEPPadding);
                }
                return encryptedData;
            }
            //Catch and display a CryptographicException  
            //to the console.
            catch (CryptographicException e)
            {
                Console.WriteLine(e.Message);

                return null;
            }

        }

        public byte[] RSADecrypt(byte[] DataToDecrypt, RSAParameters RSAKeyInfo, bool DoOAEPPadding)
        {
            try
            {
                byte[] decryptedData;
                //Create a new instance of RSACryptoServiceProvider.
                using (RSACryptoServiceProvider RSA = new RSACryptoServiceProvider())
                {
                    //Import the RSA Key information. This needs to include the private key information.
                    RSA.ImportParameters(RSAKeyInfo);

                    //Decrypt the passed byte array and specify OAEP padding.  
                    //OAEP padding is only available on Microsoft Windows XP or later.  
                    decryptedData = RSA.Decrypt(DataToDecrypt, DoOAEPPadding);
                }
                return decryptedData;
            }
            //Catch and display a CryptographicException to the console.
            catch (CryptographicException e)
            {
                Console.WriteLine(e.ToString());

                return null;
            }
        }

        void start(String[] args)
        {
            ParseArgs(args);
            if (m_bDecrypt)
            {
                Console.Write("Decrypting... ");
                DecryptFile();
                Console.WriteLine("Done");
            }
        }
/*
        static void Main(string[] args)
        {
            CDecrypt program = new CDecrypt();
            program.start(args);
        }
*/
    }
}
