using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace LanCopyFile
{
    class Program
    {
        /// <summary>
        /// 下载局域网文件
        /// </summary>
        /// <param name="path">局域网文件路径，如：\\192.168.10.1\app\app\123.zip</param>
        /// <param name="username">计算机名称</param>
        /// <param name="password">计算机密码</param>
        static void RequestWindowsShared(string path, string username, string password)
        {
            System.Net.FileWebRequest request = null;
            System.Net.FileWebResponse response = null;
            System.IO.Stream stream = null;
            System.IO.FileStream fileStream = null;
            try
            {
                Uri uri = new Uri(path);
                request = (System.Net.FileWebRequest)System.Net.FileWebRequest.Create(uri);
                System.Net.ICredentials ic = new System.Net.NetworkCredential(username, password);
                request.Credentials = ic;
                response = (System.Net.FileWebResponse)request.GetResponse();
                stream = response.GetResponseStream();
                byte[] bytes = new byte[stream.Length];
                stream.Read(bytes, 0, bytes.Length);
                string filename = System.Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\\" + System.IO.Path.GetFileName(path);
                fileStream = new FileStream(filename, System.IO.FileMode.Create, System.IO.FileAccess.Write, System.IO.FileShare.ReadWrite);
                fileStream.Write(bytes, 0, bytes.Length);
                fileStream.Flush();

                Console.WriteLine("下载成功!");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            finally
            {

                fileStream.Close();
                fileStream.Dispose();
                stream.Close();
                stream.Dispose();
            }
        }

        /// <summary>
        /// 上传文件到共享目录
        /// </summary>
        /// <param name="path">共享目录路径+文件名称</param>
        /// <param name="local">本地路径</param>
        /// <param name="username">用户名</param>
        /// <param name="password">密码</param>
        static void ResponseWindowsShared(string path, string local, string username, string password)
        {
            System.Net.FileWebRequest request = null;
            System.IO.Stream stream = null;
            try
            {
                //时间戳 
                string strBoundary = "----------" + DateTime.Now.Ticks.ToString("x");
                Uri uri = new Uri(path);
                byte[] bytes = System.IO.File.ReadAllBytes(local);
                request = (System.Net.FileWebRequest)System.Net.FileWebRequest.Create(uri);
                request.Method = "POST";
                //设置获得响应的超时时间（300秒） 
                request.Timeout = 300000;
                request.ContentType = "multipart/form-data; boundary=" + strBoundary;
                request.ContentLength = bytes.Length;

                System.Net.ICredentials ic = new System.Net.NetworkCredential(username, password);
                request.Credentials = ic;
                stream = request.GetRequestStream();
                stream.Seek(0, SeekOrigin.Begin);
                stream.Write(bytes, 0, bytes.Length);
                Console.WriteLine("上传成功!");
                
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            finally
            {
                stream.Close();
                stream.Dispose();
            }
        }

        static void AutoCopyLanFile(string strFileName)
        {
            string[] strIpArray = new string[] { "192.168.199.212", "192.168.199.219", "192.168.199.119", "192.168.199.232" };
            for (int i = 0; i < strIpArray.Length; i++)
            {
                string strLanFilePath = @"\\" + strIpArray[i] + @"\Log\New folder\\";
                string strCurFilePath = @"D:\Visual Studio 2010\Projects\LOLProjects\Release\";

                string strTmpLanFilePath = strLanFilePath + strFileName;
                string strTmpCurFilePath = strCurFilePath + strFileName;
                if (!File.Exists(strTmpCurFilePath))
                    break;

                ResponseWindowsShared(strTmpLanFilePath, strTmpCurFilePath, "administrator", "");
            }
        }

        static void Main(string[] args)
        {
            AutoCopyLanFile("AutoUpdateFile.exe");
            Console.ReadLine();
        }
    }
}
