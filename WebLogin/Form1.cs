using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using mshtml;

namespace WebLogin
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        int g_nAreaId = 2;
        string strQQUser = "";
        string strQQPass = "";
        string strdamaUser = "";
        string strdamaPass = "";
        Thread t = null;


        private void ReadInfo()
        {
            //public static extern int GetInfo(ref int nAreId, StringBuilder pwszQQUser, StringBuilder pwszQQPass, StringBuilder pwszdamaUser, StringBuilder pwszdamaPass);
            try
            {
                StringBuilder pwszQQUser = new StringBuilder(64);
                StringBuilder pwszQQPass = new StringBuilder(64);
                StringBuilder pwszdamaUser = new StringBuilder(64);
                StringBuilder pwszdamaPass = new StringBuilder(64);
                 int nRetCode = API.GetInfo(ref g_nAreaId, pwszQQUser, pwszQQPass, pwszdamaUser, pwszdamaPass);
                if (nRetCode == 0)
                {
                    MessageBox.Show("无法读取到配置信息! 程序即将退出!");
                    Application.Exit();
                    return;
                }

                strQQUser = pwszQQUser.ToString();
                strQQPass = pwszQQPass.ToString();
                strdamaUser = pwszdamaUser.ToString();
                strdamaPass = pwszdamaPass.ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show("ReadInfo:" + ex.Message);
            }
        }
        static void RunCmd(string cmd)
        {
            System.Diagnostics.Process p = new System.Diagnostics.Process();
            p.StartInfo.FileName = "cmd.exe";
            // 关闭Shell的使用
            p.StartInfo.UseShellExecute = false;
            // 重定向标准输入
            p.StartInfo.RedirectStandardInput = true;
            // 重定向标准输出
            p.StartInfo.RedirectStandardOutput = true;
            //重定向错误输出
            p.StartInfo.RedirectStandardError = true;
            p.StartInfo.CreateNoWindow = true;
            p.Start();
            p.StandardInput.WriteLine(cmd);
            p.StandardInput.WriteLine("exit");
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            ReadInfo();
            if (strQQUser != "")
            {
                //RunCmd("RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 8");
               // RunCmd("RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 2");
               // RunCmd("RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 16");
              //  RunCmd("RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 32");
              //  RunCmd("RunDll32.exe InetCpl.cpl,ClearMyTracksByProcess 255");
              //  lbMsg.Text = "StartLogin";
               // t = new Thread(RefushWebThread);
               // t.Start();   
            }

           /* this.ShowInTaskbar = false;
            this.FormBorderStyle = FormBorderStyle.None;*/
        }

        private void HandleServer()
        {
            Invoke_Navigate("http://lol.qq.com/web201310/personal.shtml");

            // 强制重新选服务器!
            Invoke(new MethodInvoker(delegate()
            {
                lbMsg.Text = "ReRefushPage……";
                //webBrowser1.Document.InvokeScript("javascript:LW201310_Userinfo.userLogin(); void 0;");
            }));
            Thread.Sleep(3 * 1000);
            // 选服务器 selectedIndex=区ID
            string str = string.Format("javascript:document.getElementById('areaContentId_lol').selectedIndex={0};document.getElementById('areaContentId_lol').onchange();void 0;", g_nAreaId.ToString());
            Invoke_Navigate(str);

            // 要等Ajax 3s, 状态判断不准!
            Thread.Sleep(3 * 1000);

            // 点确定
            Invoke_Navigate("javascript:document.getElementById('confirmButtonId_lol').click();void 0;");
            

            Thread.Sleep(1 * 1000);

            // 要刷新一次, 不然有可能会显示登录失败
            Invoke(new MethodInvoker(delegate()
            {
                lbMsg.Text = "Refush3……";
                webBrowser1.Refresh();
            }));

            Thread.Sleep(5 * 1000);

            // 调用 获取金币, Level, PlayerName
            bool bRet = false;
            Invoke(new MethodInvoker(delegate()
            {
                try
                {
                    string strLevel = webBrowser1.Document.GetElementById("jUserLoginedDiv").Children[0].Children[1].Children[0].InnerText;
                    string strName = webBrowser1.Document.GetElementById("jUserLoginedDiv").Children[1].Children[0].InnerText;
                    string strSumFight = webBrowser1.Document.GetElementById("jUserLoginedDiv").Children[2].Children[0].Children[0].InnerText;
                    string strMoney = webBrowser1.Document.GetElementById("jUserLoginedDiv").Children[2].Children[2].Children[0].InnerText;
                    lbGold.Text = strMoney;
                    lbLevel.Text = strLevel;
                    lbNiceName.Text = strName;
                    lbSumFightCount.Text = strSumFight;
                    if (strLevel != "")
                    {
                        lbMsg.Text = "Done……";
                        strLevel = strLevel.Substring(3);
                        int nLevel = int.Parse(strLevel);
                        int nGold = int.Parse(strMoney);
                        int nSumWarCount = int.Parse(strSumFight);

                        API.SetInfo(nLevel, nSumWarCount, nGold, strName);
                        Thread.Sleep(1000);
                    }
                    else
                    {
                        lbMsg.Text = "Error……";
                        bRet = true;
                    }
                }
                catch (Exception)
                {
                    lbMsg.Text = "Error……";
                    bRet = true;
                    API.SetInfo(-1, 0, 0, "");
                    Thread.Sleep(1000);
                }
            }));

            // 每6个小时查一下
           // if (!bRet)
                Thread.Sleep(6 * 60 * 60 * 1000);
        }

        private void RefushWebThread()
        {
            try
            {
                while (true)
                {
                    // QQ MainIndex
                    int nCount = 0;
                    Random rd = new Random();
                lb_ReStart: ;
                    Invoke_Navigate("http://xui.ptlogin2.qq.com/cgi-bin/xlogin?proxy_url=http://game.qq.com/comm-htdocs/milo/proxy.html&appid=21000501&target=top&s_url=http%3A%2F%2Flol.qq.com%2Fweb201310%2Fpersonal.shtml&daid=8");
                    Thread.Sleep(3000);
                    //Invoke_Navigate("javascript:LW201310_Userinfo.userLogout(); void 0;");
                    //Invoke_Navigate("http://xui.ptlogin2.qq.com/cgi-bin/xlogin?proxy_url=http://game.qq.com/comm-htdocs/milo/proxy.html&appid=21000501&target=top&s_url=http%3A%2F%2Flol.qq.com%2Fweb201310%2Fpersonal.shtml&daid=8");
                    Thread.Sleep(2000);

                    if (IsLoginSucc())
                        HandleServer();

                    // 输入QQ && Pass
                    string str = string.Format("javascript:document.getElementById('u').value = '{0}';document.getElementById('p').value='{1}';document.getElementById('login_button').click();void 0;", strQQUser, strQQPass);
                    Invoke_Navigate(str);
                    Thread.Sleep(2000);
                    // 登录2次, 免得失败
                  //  str = string.Format("javascript:document.getElementById('u').value = '{0}';document.getElementById('p').value='{1}';document.getElementById('login_button').click();void 0;", strQQUser, strQQPass);
                   // Invoke_Navigate(str);

                    // 判断是不是出验证码了!
                    if (IsHasVerCode())
                        GetVerCodeSrc();

                    Invoke(new MethodInvoker(delegate()
                    {
                        lbMsg.Text = "Refush1……";
                        //webBrowser1.Refresh();
                    }));
                    Thread.Sleep(5 * 1000);

                    // 判断是否登录成功了!
                    if (nCount++ > 2)
                    {
                        MessageBox.Show("2次登录失败");
                        return;
                    }

                    if (!IsLoginSucc())
                        goto lb_ReStart;

                    HandleServer();
                }
            }
            catch (Exception)
            {
                Application.Exit();
            }
            
        }

        private void Invoke_Navigate(string strUrl)
        {
            try
            {
                Invoke(new MethodInvoker(delegate()
                {
                    webBrowser1.Navigate(strUrl);
                }));
                WaitToLoad();
            }
            catch (Exception)
            {
                Application.Exit();
            }
        }

        private void WaitToLoad()
        {
            for (int i = 0; i < 3; ++i)
            {
                Thread.Sleep(1000);
                bool bComplete = false;
                Invoke(new MethodInvoker(delegate()
                {
                    bComplete = (webBrowser1.ReadyState == WebBrowserReadyState.Complete);
                }));

                if (bComplete)
                    break;

            }
        }

        private bool IsChooseServer()
        {
            bool bRetCode = false;
            Invoke(new MethodInvoker(delegate()
            {
                lbMsg.Text = "IsChooseServer";
                bRetCode = webBrowser1.Document.GetElementById("roleselecterlol") != null;
            }));
            return bRetCode;
        }

        private bool IsLoginSucc()
        {
            bool bRetCode = false;
            Invoke(new MethodInvoker(delegate()
            {
                lbMsg.Text = "IsLoginSucc";
                bRetCode = webBrowser1.Document.GetElementsByTagName("form").Count == 1;
            }));
            return !bRetCode;
        }

        private bool IsHasVerCode()
        {
            bool bRetCode = false;
            Invoke(new MethodInvoker(delegate()
            {
                lbMsg.Text = "IsHasVerCode";
                /*if (webBrowser1.Document.GetElementsByTagName("iframe") == null)
                {
                    bRetCode = false;
                }
                else
                {
                    bRetCode = webBrowser1.Document.GetElementsByTagName("iframe").Count > 0 ? true : false;
                }*/
                bRetCode = webBrowser1.Document.GetElementById("newVcodeArea") != null ? true : false;
            }));
            return bRetCode;
        }

        private Image GetWebImage(HTMLDocument doc, IHTMLControlElement ImgeTag)
        {
            HTMLBody body = (HTMLBody)doc.body;
            IHTMLControlRange rang = (IHTMLControlRange)body.createControlRange();
            IHTMLControlElement Img = ImgeTag; //图片地址

            Image oldImage = Clipboard.GetImage();
            rang.add(Img);
            rang.execCommand("Copy", false, null);  //拷贝到内存
            Image numImage = Clipboard.GetImage();
            try
            {
                if (oldImage != null)
                    Clipboard.SetImage(oldImage);
            }
            catch(Exception ex)
            {
               // MessageBox.Show("GetWebImage:" + ex.Message);
            }

            return numImage;
        }

        public string GetVerCodeSrc()
        {
            Invoke(new MethodInvoker(delegate()
            {
                try
                {
                    lbMsg.Text = "GetVerCodeSrc";
                    var Doc = webBrowser1.Document;
                    var frames = Doc.Window.Frames;
                    if (frames != null && frames.Count > 0)
                    {
                        IHTMLDocument3 framesDoc = CorssDomainHelper.GetDocumentFromWindow(frames[0].DomWindow as IHTMLWindow2);
                        if (framesDoc != null)
                        {
                            var ImgeTag = framesDoc.getElementById("CAPTCHA_img") as IHTMLControlElement;
                            if (ImgeTag != null)
                            {
                                Image numPic = GetWebImage(framesDoc as HTMLDocument, ImgeTag); // 得到验证码图片
                                numPic.Save("C:\\1.jpg");
                                
                                StringBuilder VCodeText = new StringBuilder(100);
                                int ret = Dama2.D2File(
                                    "f2e0d5bfad94c9e325e1a7b707f3fcc9", //softawre key (software id)
                                    strdamaUser,    //user name
                                    strdamaPass,     //password
                                    "C:\\1.jpg",
                                    30,
                                    101,
                                    VCodeText);

                                if (ret < 0)
                                {
                                    MessageBox.Show("打码失败! 可能是帐号没钱了吧? 也可能是断网了!Err" + ret.ToString());
                                }

                                framesDoc.getElementById("capAns").setAttribute("value", VCodeText.ToString());
                                var a = framesDoc.getElementsByTagName("a");
                                a.item(3).click();
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    //MessageBox.Show("GetVerCodeSrc" + ex.Message);
                    lbMsg.Text = ex.Message;
                }

            }));
            Thread.Sleep(3 * 1000);
            return "";
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Close Thread
            if (t != null)
            {
                t.Abort();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string strLevel = txtLevel.Text.Trim();
            string strName = txtNiceName.Text.Trim();
            string strSumFight = txtSumCount.Text.Trim();
            string strMoney = txtGold.Text.Trim();
            lbGold.Text = strMoney;
            lbLevel.Text = strLevel;
            lbNiceName.Text = strName;
            lbSumFightCount.Text = strSumFight;
            if (strLevel != "")
            {
                lbMsg.Text = "Done……";
                int nLevel = int.Parse(strLevel);
                int nGold = int.Parse(strMoney);
                int nSumWarCount = int.Parse(strSumFight);

                API.SetInfo(nLevel, nSumWarCount, nGold, strName);
            }
        }

    }
}
