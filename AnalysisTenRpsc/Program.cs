using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AnalysisTenRpsc
{
    class Program
    {
        static string GetText()
        {
            string strText = string.Empty;
            using (StreamReader SR = new StreamReader("1.txt", Encoding.ASCII))
            {
                strText = SR.ReadToEnd();
                SR.Close();
            }

            return strText;
        }
        static void Main(string[] args)
        {
            string strText = GetText();
            while (strText.IndexOf("\r\n") != -1)
            {
                string strLine = strText.Substring(0, strText.IndexOf("\r\n"));
                if (strLine.IndexOf("PUSHFD") == -1)
                {
                    strText = strText.Substring(strText.IndexOf("\r\n") + 2);
                    continue;
                }

                string strAddr = strLine.Substring(0, 8);
                int nAddr = Convert.ToInt32(strAddr, 16);
                nAddr -= 0xA190000;

                Console.WriteLine("0x" + nAddr.ToString("X") + ",");
                strText = strText.Substring(strText.IndexOf("\r\n") + 2);
            }

            Console.ReadLine();
        }
    }
}
