using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WebLogin
{
    public class API
    {
        [DllImport("WebLoginDLL.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetInfo( int nLevel,  int nSumFightCount,  int nGold, string pwszNiceName);

        [DllImport("WebLoginDLL.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetInfo(ref int nAreId, StringBuilder pwszQQUser, StringBuilder pwszQQPass, StringBuilder pwszdamaUser, StringBuilder pwszdamaPass);

    }
    static class WebCleaner
    {
        /*
         * 7 个静态函数
         * 私有函数
         * private bool FileDelete()    : 删除文件
         * private void FolderClear()   : 清除文件夹内的所有文件
         * private void RunCmd()        : 运行内部命令
         * 
         * 公有函数
         * public void CleanCookie()    : 删除Cookie
         * public void CleanHistory()   : 删除历史记录
         * public void CleanTempFiles() : 删除临时文件
         * public void CleanAll()       : 删除所有
         * 
         * 
         * 
         * */
        //private
        /// 
        /// 删除一个文件，System.IO.File.Delete()函数不可以删除只读文件，这个函数可以强行把只读文件删除。
        /// 
        /// 文件路径
        /// 是否被删除
        static bool FileDelete(string path)
        {
            //first set the File\'s ReadOnly to 0
            //if EXP, restore its Attributes
            System.IO.FileInfo file = new System.IO.FileInfo(path);
            System.IO.FileAttributes att = 0;
            bool attModified = false;
            try
            {
                //### ATT_GETnSET
                att = file.Attributes;
                file.Attributes &= (~System.IO.FileAttributes.ReadOnly);
                attModified = true;
                file.Delete();
            }
            catch (Exception e)
            {
                if (attModified)
                    file.Attributes = att;
                return false;
            }
            return true;
        }
        //public 
        /// 
        /// 清除文件夹
        /// 
        /// 文件夹路径
        static void FolderClear(string path)
        {
            System.IO.DirectoryInfo diPath = new System.IO.DirectoryInfo(path);
            foreach (System.IO.FileInfo fiCurrFile in diPath.GetFiles())
            {
                FileDelete(fiCurrFile.FullName);
            }
            foreach (System.IO.DirectoryInfo diSubFolder in diPath.GetDirectories())
            {
                FolderClear(diSubFolder.FullName); // Call recursively for all subfolders
            }
        }
        static void RunCmd(string cmd)
        {
            System.Diagnostics.Process.Start("\"cmd.exe\"", "\"/c \"" + cmd);
        }
        /// 
        /// 删除历史记录
        /// 
        public static void CleanHistory()
        {
            string[] theFiles = System.IO.Directory.GetFiles(Environment.GetFolderPath(Environment.SpecialFolder.History), "\"*\"", System.IO.SearchOption.AllDirectories);
            foreach (string s in theFiles)
                FileDelete(s);
            RunCmd("\"RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 1\"");
        }
        /// 
        /// 删除临时文件
        /// 
        public static void CleanTempFiles()
        {
            FolderClear(Environment.GetFolderPath(Environment.SpecialFolder.InternetCache));
            RunCmd("\"RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 8\"");
        }
        /// 
        /// 删除Cookie
        /// 
        public static void CleanCookie()
        {
            string[] theFiles = System.IO.Directory.GetFiles(Environment.GetFolderPath(Environment.SpecialFolder.Cookies), "*", System.IO.SearchOption.AllDirectories);
            foreach (string s in theFiles)
                FileDelete(s);
            RunCmd("RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 2");
        }
        /// 
        /// 删除全部
        /// 
        public static void CleanAll()
        {
            CleanHistory();
            CleanCookie();
            CleanTempFiles();
        }
    }
}
