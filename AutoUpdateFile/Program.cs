using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace AutoUpdateFile
{
    class Program
    {
        private static FtpWebRequest GetRequest(string URI, string username, string password)
        {
            //根据服务器信息FtpWebRequest创建类的对象
            FtpWebRequest result = (FtpWebRequest)FtpWebRequest.Create(URI);
            //提供身份验证信息
            result.Credentials = new System.Net.NetworkCredential(username, password);
            //设置请求完成之后是否保持到FTP服务器的控制连接，默认值为true
            result.KeepAlive = false;
            return result;
        }

        private static void KillProc(string strProcName)
        {
            Process[] ProcArray = Process.GetProcessesByName(strProcName);
            for (int i = 0; i < ProcArray.Length; i++)
            {
                Console.WriteLine("正在结束进程:" + ProcArray[i].ProcessName);
                ProcArray[i].Kill();
            }
        }

        private static void CopyFileToSysWoW64()
        {
            try
            {
                if (File.Exists(System.Environment.CurrentDirectory + @"\mfc120u\.dll"))
                {
                    File.Move(System.Environment.CurrentDirectory + @"\mfc120u\.dll", @"C:\Windows\SysWOW64\mfc120u.dll");
                    File.Move(System.Environment.CurrentDirectory + @"\mfc120ud\.dll", @"C:\Windows\SysWOW64\mfc120ud.dll");
                    File.Move(System.Environment.CurrentDirectory + @"\msvcp120\.dll", @"C:\Windows\SysWOW64\msvcp120.dll");
                    File.Move(System.Environment.CurrentDirectory + @"\msvcp120d\.dll", @"C:\Windows\SysWOW64\msvcp120d.dll");
                    File.Move(System.Environment.CurrentDirectory + @"\msvcr120\.dll", @"C:\Windows\SysWOW64\msvcr120.dll");
                    File.Move(System.Environment.CurrentDirectory + @"\msvcr120d\.dll", @"C:\Windows\SysWOW64\msvcr120d.dll");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("CopyFileToSysWoW64:" + ex.Message);
                Console.ReadLine();
            }
        }

        public static void DownloadFile(string localDir, string FtpDir, string FtpFile, string hostname, string username, string password)
        {
            string URI = "FTP://" + hostname + "/" + FtpDir + "/" + FtpFile;
            string tmpname = Guid.NewGuid().ToString();
            string localfile = localDir + @"\" + tmpname;

            bool bSucc = false;
            while (!bSucc)
            {
                try
                {
                    System.Net.FtpWebRequest ftp = GetRequest(URI, username, password);
                    ftp.Timeout = 5 * 1000;
                    ftp.ReadWriteTimeout = 5 * 1000;
                    ftp.Method = System.Net.WebRequestMethods.Ftp.DownloadFile;
                    ftp.UseBinary = true;
                    ftp.UsePassive = false;

                    using (FtpWebResponse response = (FtpWebResponse)ftp.GetResponse())
                    {
                        using (Stream responseStream = response.GetResponseStream())
                        {
                            //loop to read & write to file
                            using (FileStream fs = new FileStream(localfile, FileMode.CreateNew))
                            {
                                try
                                {
                                    byte[] buffer = new byte[2048];
                                    int read = 0;
                                    do
                                    {
                                        read = responseStream.Read(buffer, 0, buffer.Length);
                                        fs.Write(buffer, 0, read);
                                    } while (!(read == 0));
                                    responseStream.Close();
                                    fs.Flush();
                                    fs.Close();
                                }
                                catch (Exception)
                                {
                                    //catch error and delete file only partially downloaded
                                    fs.Close();
                                    //delete target file as it's incomplete
                                    File.Delete(localfile);
                                    throw;
                                }
                            }

                            responseStream.Close();
                        }

                        response.Close();
                    }
                    bSucc = true;
                }
                catch (Exception ex)
                {
                    Console.WriteLine("访问ftp异常:" + ex.Message);
                    Thread.Sleep(10 * 1000);
                    bSucc = false;
                }
            }   



            try
            {
                File.Delete(localDir + @"\" + FtpFile);
                File.Move(localfile, localDir + @"\" + FtpFile);


               // ftp = null;
                //ftp = GetRequest(URI, username, password);
                //ftp.Method = System.Net.WebRequestMethods.Ftp.DeleteFile;
                //ftp.GetResponse();

            }
            catch (Exception ex)
            {
                File.Delete(localfile);
                Console.WriteLine("转换文件发生异常:" + ex.Message);
                Console.Read();
                System.Environment.Exit(0);
            }

            // 记录日志 "从" + URI.ToString() + "下载到" + localDir + @"\" + FtpFile + "成功." );
           // ftp = null;
        }
        static void Main(string[] args)
        {
            CopyFileToSysWoW64();
            StringBuilder sbServerIp = new StringBuilder(32);
            GetPrivateProfileString("Login", "ServerIp", "0", sbServerIp, 32, System.Environment.CurrentDirectory + @"\Config.ini");
            if (sbServerIp.ToString() == "0")
            {
                Console.WriteLine("无法读取配置文件!Config.ini下的[Config]里木有ServerIp");
                Console.Read();
                System.Environment.Exit(0);
            }

            StringBuilder sbVersion = new StringBuilder(32);
            GetPrivateProfileString("Config", "Version", "0", sbVersion, 32, System.Environment.CurrentDirectory + @"\Config.ini");
            if (sbVersion.ToString() == "0")
            {
                Console.WriteLine("无法读取配置文件!Config.ini下的[Config]里木有Version=版本号");
                Console.Read();
                System.Environment.Exit(0);
            }

            if (!Directory.Exists(System.Environment.CurrentDirectory + @"\TempFile"))
            {
                Directory.CreateDirectory(System.Environment.CurrentDirectory + @"\TempFile");
            }

            string strVersion = sbVersion.ToString();
            // V1.01
            Console.WriteLine("当前客户端版本:" + strVersion);

            // 下载更新配置文件到本地
            Console.WriteLine("正在下载配置文件……");
            DownloadFile(System.Environment.CurrentDirectory + @"\TempFile", "Update", "UpdateList.ini", sbServerIp.ToString(), "ftpUser", "qq8159465");
            Console.WriteLine("下载完毕!");

            // 读取配置是否要更新
            string strServerVersion = string.Empty;
            if (!IsUpdate(strVersion, ref strServerVersion))
            {
                Console.WriteLine("不需要更新!当前已经是最新的版本了:" + strVersion);
                RunConsole();
                Thread.Sleep(3000);
                return;
            }

            Console.WriteLine("当前版本是:" + strVersion + ",服务器最新版本是:" + strServerVersion + ",需要更新文件!");
            Console.WriteLine("服务器文件列表如下:");
            List<string> DownLoadList = PrintServerList();
            for (int i = 0; i < DownLoadList.Count; i++)
            {
                Console.WriteLine("正在下载第" + (i + 1).ToString() + "个文件:" + DownLoadList[i]);
                DownloadFile(System.Environment.CurrentDirectory + @"\TempFile", "Update", DownLoadList[i], sbServerIp.ToString(), "ftpUser", "qq8159465");
            }

            for (int i = 0; i < DownLoadList.Count; i++)
            {
                Console.WriteLine("正在删除旧文件:" + DownLoadList[i]);
                if (DownLoadList[i].IndexOf("WebLogin") != -1)
                {
                     KillProc("WebLogin");
                     Thread.Sleep(1000);
                }

                string strFileName = "";
                if (DownLoadList[i].IndexOf(".bmp") != -1)
                    strFileName = @"Bmp\" + DownLoadList[i];
                else
                    strFileName = DownLoadList[i];

                if (File.Exists(System.Environment.CurrentDirectory + @"\" + strFileName) && !ExistProcess(DownLoadList[i]))
                    File.Delete(System.Environment.CurrentDirectory + @"\" + strFileName);
            }

            for (int i = 0; i < DownLoadList.Count; i++)
            {
                Console.WriteLine("正在生成新文件:" + DownLoadList[i]);

                string strFileName = "";
                if (DownLoadList[i].IndexOf(".bmp") != -1)
                    strFileName = @"\Bmp\" + DownLoadList[i];
                else
                    strFileName = @"\" + DownLoadList[i];

                if (File.Exists(System.Environment.CurrentDirectory + @"\TempFile\" + DownLoadList[i]))
                {
                    File.Move(System.Environment.CurrentDirectory + @"\TempFile\" + DownLoadList[i], System.Environment.CurrentDirectory + strFileName);
                    File.Delete(System.Environment.CurrentDirectory + @"\TempFile\" + DownLoadList[i]);
                }
            }

            if(File.Exists(System.Environment.CurrentDirectory + @"\TempFile\UpdateList.ini"))
                File.Delete(System.Environment.CurrentDirectory + @"\TempFile\UpdateList.ini");

            WritePrivateProfileString("Config","Version", strServerVersion, System.Environment.CurrentDirectory + @"\Config.ini");

           // if (File.Exists(System.Environment.CurrentDirectory + @"\AutoUpdateFile.exe"))
               // File.Delete(System.Environment.CurrentDirectory + @"\AutoUpdateFile.exe");

            Console.WriteLine("更新完毕!");
            RunConsole();
            Thread.Sleep(3000);
            return;
        }

        static void RunConsole()
        {
            Process.Start(System.Environment.CurrentDirectory + @"\LolConsole.exe", "-start");
        }

        static bool IsUpdate(string strClientVersion, ref string strServerVersion)
        {
            StringBuilder sbServerVersion = new StringBuilder(32);
            GetPrivateProfileString("Config", "Version", "0", sbServerVersion, 32, System.Environment.CurrentDirectory + @"\TempFile\UpdateList.ini");
            if (sbServerVersion.ToString() == "0")
            {
                Console.WriteLine("无法读取服务器下载的配置文件!");
                return false;
            }
            strServerVersion = sbServerVersion.ToString();
            return strClientVersion != sbServerVersion.ToString();
        }

        public static string GetMD5HashFromFile(string fileName)
        {
            try
            {
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

        public static bool CompFileMd5(string fileName, string strServerFileMd5)
        {
            string strClientMd5 = GetMD5HashFromFile(System.Environment.CurrentDirectory + @"\" + fileName);
            return strClientMd5 == strServerFileMd5;
        }

        public static bool ExistProcess(string strFileName)
        {
            Process[] ProcArray = Process.GetProcessesByName(strFileName);
            return ProcArray.Length > 0;
        }

        static List<string> PrintServerList()
        {
            List<string> lst = new List<string>();

            string strPath = System.Environment.CurrentDirectory + @"\TempFile\UpdateList.ini";
            if(!File.Exists(strPath))
                return lst;

            StringBuilder sbFileCount = new StringBuilder(32);
            GetPrivateProfileString("Config", "FileCount", "0", sbFileCount, 32, strPath);
            if (sbFileCount.ToString() == "0")
                return lst;

            int nFileCount = int.Parse(sbFileCount.ToString());
            for (int i = 0; i < nFileCount; i++)
            {
                StringBuilder sbFileName = new StringBuilder(64);
                StringBuilder sbFileMd5 = new StringBuilder(1024);

                GetPrivateProfileString("Config", "File" + (i + 1).ToString(), "0", sbFileName, 64, strPath);
                GetPrivateProfileString("Config", "File" + (i + 1).ToString() + "Md5", "0", sbFileMd5, 64, strPath);
                if (sbFileName.ToString() == "0" || sbFileMd5.ToString() == "0")
                {
                    Console.WriteLine("读取文件出错:" + sbFileName.ToString() + ",MD5:" + sbFileMd5.ToString());
                    Console.Read();
                    System.Environment.Exit(0);
                }

                string strFileName = "";

                if (sbFileName.ToString().IndexOf(".bmp") != -1)
                    strFileName = @"Bmp\" + sbFileName.ToString();
                else
                    strFileName = sbFileName.ToString();

                // 判断该文件是否存在, 如果不存在, 那就直接更新该文件
                if (!File.Exists(System.Environment.CurrentDirectory + @"\" + strFileName))
                {
                    Console.WriteLine("文件:" + strFileName + ",不存在,直接下载!");
                    lst.Add(sbFileName.ToString());
                    continue;
                }

                // 判断文件的MD5

                if (!ExistProcess(sbFileName.ToString()) && !CompFileMd5(strFileName.ToString(), sbFileMd5.ToString()))
                {
                    Console.WriteLine("文件:" + strFileName.ToString() + ",的MD5与服务器上的不对, 要更新此文件!");
                    lst.Add(sbFileName.ToString());
                    continue;
                }
                else
                {
                    Console.WriteLine("文件:" + strFileName.ToString() + ",不用更新");
                }
            }


            return lst;
        }

        [DllImport("kernel32")]
        public static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

        [DllImport("kernel32")]
        public static extern int GetPrivateProfileString(string section, string key, string def, System.Text.StringBuilder retVal, int size, string filePath);
    }
}
