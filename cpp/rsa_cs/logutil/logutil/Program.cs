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
    class Program
    {
        private bool m_bDecrypt = false;
        private bool m_bShowHelp = false;
        private string m_inkey = "";
        private string m_inputFile = "";
        private string m_outputFile = "";

        private void Help()
        {
            string msg = @"Usage: logutil.bin [OPTION]\n
Encrypt or decrypt file with rsa key pair.\n
\n
Valid options are:\n
-help,--help    Display this summary\n
-decrypt        Decrypt with key file\n
-inkey keyfile  Input key file\n
-in infile      Input file\n
-out outfile    Output file\n";
            Console.WriteLine(msg);
        }

        public void ParseArgs(string[] args)
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
                    m_inkey = args[i];
                    if (!File.Exists(m_inkey))
                    {
                        m_bDecrypt = false;
                        Console.WriteLine("The key file does not exist.");
                        break;
                    }
                }
                else if (0 == args[i].CompareTo("-in"))
                {
                    i++;
                    m_inputFile = args[i];
                    if (!File.Exists(m_inputFile))
                    {
                        m_bDecrypt = false;
                        Console.WriteLine("The input file does not exist.");
                        break;
                    }

                }
                else if (0 == args[i].CompareTo("-out"))
                {
                    i++;
                    m_outputFile = args[i];
                }
                else if (0 == args[i].CompareTo("-help") || 0 == args[i].CompareTo("--help"))
                {
                    m_bShowHelp = true;
                    break;
                }
            }

            if (m_bShowHelp)
            {
                m_bDecrypt = false;
                Help();
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
            {    //remove high order zeros in data
                count -= 1;
            }
            binr.BaseStream.Seek(-1, SeekOrigin.Current);        //last ReadByte wasn't a removed zero, so back up a byte
            return count;
        }

        public static RSACryptoServiceProvider DecodeRsaPrivateKey(string filePath)
        {

            byte[] MODULUS, E, D, P, Q, DP, DQ, IQ;

            FileStream fs = new FileStream(filePath, FileMode.Open, FileAccess.Read);

            BinaryReader binr = new BinaryReader(fs);    //wrap Memory Stream with BinaryReader for easy reading
            try
            {
                var twobytes = binr.ReadUInt16();
                if (twobytes == 0x8130) //data read as little endian order (actual data order for Sequence is 30 81)
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

        public static string PrivateKeyDecXml(string strDerKey)
        {
            RSACryptoServiceProvider rsaProvider = DecodeRsaPrivateKey(strDerKey);
            var privateKey = rsaProvider.ToXmlString(true);
            return privateKey;
        }

        public void DecryptFile()
        {
            FileStream inFileStream = new FileStream(m_inputFile, FileMode.Open, FileAccess.Read);
            byte[] buffer = new byte[128];
            inFileStream.Read(buffer, 0, 128);

            byte[] decryptedData;
            string keyxml = PrivateKeyDecXml(m_inkey);
            decryptedData = RSADecrypt(buffer, keyxml, false);
            
            if (decryptedData != null)
            { 
                FileStream outFileStream = new FileStream(m_outputFile, FileMode.Create, FileAccess.Write);
                outFileStream.Write(decryptedData, 0, 128);
                long length = inFileStream.Length;
                inFileStream.CopyTo(outFileStream, (int)(length - 128));
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
                DecryptFile();
            }
        }
        static void Main(string[] args)
        {
            Program program = new Program();
            program.start(args);
        }
    }
}
