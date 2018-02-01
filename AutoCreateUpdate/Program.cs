using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace AutoCreateUpdate
{
    class Program
    {
        static void Main(string[] args)
        {
            string strUpdateListPath = System.Environment.CurrentDirectory + @"\Update\UpdateList.ini";

            StringBuilder sbServerVersion = new StringBuilder(32);
            GetPrivateProfileString("Config", "Version", "0", sbServerVersion, 32, strUpdateListPath);
            if (sbServerVersion.ToString() == "0")
            {
                Console.WriteLine("无法读取版本号!");
                Console.ReadLine();
                return;
            }

            string strServerVersion = sbServerVersion.ToString();
            strServerVersion = strServerVersion.Substring(1);
            Console.WriteLine("当前版本是:" + strServerVersion);
            strServerVersion = "V" + (float.Parse(strServerVersion) + 0.01).ToString("F2");
            Console.WriteLine("生成的版本号是:" + strServerVersion);

            

            // 遍历目录CopyFile下的文件
            DirectoryInfo DirInfo = new DirectoryInfo(System.Environment.CurrentDirectory + @"\CopyFile");
            FileInfo[] FileInfoArray = DirInfo.GetFiles();

            for (int i = 0; i < FileInfoArray.Length; i++)
            {
                string strFileName = FileInfoArray[i].Name;
                if (strFileName.IndexOf(".dll") == -1 && strFileName.IndexOf(".exe") == -1 && strFileName.IndexOf(".bmp") == -1)
                    continue;

                string strMd5 = GetMD5HashFromFile(System.Environment.CurrentDirectory + @"\CopyFile\" + strFileName);

                try
                {
                    if (File.Exists(System.Environment.CurrentDirectory + @"\Update\" + strFileName))
                        File.Delete(System.Environment.CurrentDirectory + @"\Update\" + strFileName);

                    File.Copy(System.Environment.CurrentDirectory + @"\CopyFile\" + strFileName,System.Environment.CurrentDirectory + @"\Update\" + strFileName);
                }
                catch (Exception ex)
                {
                    Console.WriteLine("拷贝文件出现异常:" + ex.Message);
                    Console.ReadLine();
                    System.Environment.Exit(0);
                }

                WritePrivateProfileString("Config", "File" + (i + 1).ToString(), strFileName, strUpdateListPath);
                WritePrivateProfileString("Config", "File" + (i + 1).ToString() + "Md5", strMd5, strUpdateListPath);
            }

            WritePrivateProfileString("Config", "FileCount", FileInfoArray.Length.ToString(), strUpdateListPath);
            WritePrivateProfileString("Config", "Version", strServerVersion, strUpdateListPath);
        }

        public static string GetMD5HashFromFile(string fileName)
        {
            try
            {
                Console.WriteLine("FileName:" + fileName);
                FileStream file = new FileStream(fileName, System.IO.FileMode.Open);
                MD5 md5 = new MD5CryptoServiceProvider();
                byte[] retVal = md5.ComputeHash(file);
                file.Close();
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < retVal.Length; i++)
                {
                    sb.Append(retVal[i].ToString("x2"));
                }
                return sb.ToString();
            }
            catch (Exception ex)
            {
                Console.WriteLine("计算文件md5错误:" + "GetMD5HashFromFile() fail,error:" + ex.Message);
            }
            return "";
        }  

        [DllImport("kernel32")]
        public static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

        [DllImport("kernel32")]
        public static extern int GetPrivateProfileString(string section, string key, string def, System.Text.StringBuilder retVal, int size, string filePath);
    }
}
