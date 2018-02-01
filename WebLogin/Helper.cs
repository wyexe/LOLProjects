using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;
using mshtml;
using System.Threading.Tasks;

namespace WebLogin
{
    [ComImport(), ComVisible(true), Guid("6D5140C1-7436-11CE-8034-00AA006009FA"),
    InterfaceTypeAttribute(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IServiceProvider
    {
        [return: MarshalAs(UnmanagedType.I4)]
        [PreserveSig]
        int QueryService(ref Guid guidService, ref Guid riid, [MarshalAs(UnmanagedType.Interface)] out object ppvObject);
    }

    public enum OLECMDF
    {
        OLECMDF_DEFHIDEONCTXTMENU = 0x20,
        OLECMDF_ENABLED = 2,
        OLECMDF_INVISIBLE = 0x10,
        OLECMDF_LATCHED = 4,
        OLECMDF_NINCHED = 8,
        OLECMDF_SUPPORTED = 1
    }

    public enum OLECMDID
    {
        OLECMDID_PAGESETUP = 8,
        OLECMDID_PRINT = 6,
        OLECMDID_PRINTPREVIEW = 7,
        OLECMDID_PROPERTIES = 10,
        OLECMDID_SAVEAS = 4
    }

    public enum OLECMDEXECOPT
    {
        OLECMDEXECOPT_DODEFAULT,
        OLECMDEXECOPT_PROMPTUSER,
        OLECMDEXECOPT_DONTPROMPTUSER,
        OLECMDEXECOPT_SHOWHELP
    }

    [ComImport, Guid("D30C1661-CDAF-11d0-8A3E-00C04FC9E26E"), TypeLibType(TypeLibTypeFlags.FOleAutomation | TypeLibTypeFlags.FDual | TypeLibTypeFlags.FHidden)]
    public interface IWebBrowser2
    {
        [DispId(100)]
        void GoBack();
        [DispId(0x65)]
        void GoForward();
        [DispId(0x66)]
        void GoHome();
        [DispId(0x67)]
        void GoSearch();
        [DispId(0x68)]
        void Navigate([In] string Url, [In] ref object flags, [In] ref object targetFrameName, [In] ref object postData, [In] ref object headers);
        [DispId(-550)]
        void Refresh();
        [DispId(0x69)]
        void Refresh2([In] ref object level);
        [DispId(0x6a)]
        void Stop();
        [DispId(200)]
        object Application { [return: MarshalAs(UnmanagedType.IDispatch)] get; }
        [DispId(0xc9)]
        object Parent { [return: MarshalAs(UnmanagedType.IDispatch)] get; }
        [DispId(0xca)]
        object Container { [return: MarshalAs(UnmanagedType.IDispatch)] get; }
        [DispId(0xcb)]
        object Document { [return: MarshalAs(UnmanagedType.IDispatch)] get; }
        [DispId(0xcc)]
        bool TopLevelContainer { get; }
        [DispId(0xcd)]
        string Type { get; }
        [DispId(0xce)]
        int Left { get; set; }
        [DispId(0xcf)]
        int Top { get; set; }
        [DispId(0xd0)]
        int Width { get; set; }
        [DispId(0xd1)]
        int Height { get; set; }
        [DispId(210)]
        string LocationName { get; }
        [DispId(0xd3)]
        string LocationURL { get; }
        [DispId(0xd4)]
        bool Busy { get; }
        [DispId(300)]
        void Quit();
        [DispId(0x12d)]
        void ClientToWindow(out int pcx, out int pcy);
        [DispId(0x12e)]
        void PutProperty([In] string property, [In] object vtValue);
        [DispId(0x12f)]
        object GetProperty([In] string property);
        [DispId(0)]
        string Name { get; }
        [DispId(-515)]
        int HWND { get; }
        [DispId(400)]
        string FullName { get; }
        [DispId(0x191)]
        string Path { get; }
        [DispId(0x192)]
        bool Visible { get; set; }
        [DispId(0x193)]
        bool StatusBar { get; set; }
        [DispId(0x194)]
        string StatusText { get; set; }
        [DispId(0x195)]
        int ToolBar { get; set; }
        [DispId(0x196)]
        bool MenuBar { get; set; }
        [DispId(0x197)]
        bool FullScreen { get; set; }
        [DispId(500)]
        void Navigate2([In] ref object URL, [In] ref object flags, [In] ref object targetFrameName, [In] ref object postData, [In] ref object headers);

        [DispId(0x1f5)]
        OLECMDF QueryStatusWB([In] OLECMDID cmdID);

        [DispId(0x1f6)]
        void ExecWB([In] OLECMDID cmdID, [In] OLECMDEXECOPT cmdexecopt, ref object pvaIn, IntPtr pvaOut);

        [DispId(0x1f7)]
        void ShowBrowserBar([In] ref object pvaClsid, [In] ref object pvarShow, [In] ref object pvarSize);
        [DispId(-525)]
        WebBrowserReadyState ReadyState { get; }
        [DispId(550)]
        bool Offline { get; set; }
        [DispId(0x227)]
        bool Silent { get; set; }
        [DispId(0x228)]
        bool RegisterAsBrowser { get; set; }
        [DispId(0x229)]
        bool RegisterAsDropTarget { get; set; }
        [DispId(0x22a)]
        bool TheaterMode { get; set; }
        [DispId(0x22b)]
        bool AddressBar { get; set; }
        [DispId(0x22c)]
        bool Resizable { get; set; }
    }

    class CorssDomainHelper
    {
        private static Guid IID_IWebBrowserApp = new Guid("0002DF05-0000-0000-C000-000000000046");
        private static Guid IID_IWebBrowser2 = new Guid("D30C1661-CDAF-11D0-8A3E-00C04FC9E26E");

        // Utility for IE cross domain access
        // Returns null in case of failure.
        public static IHTMLDocument3 GetDocumentFromWindow(IHTMLWindow2 htmlWindow)
        {

            if (htmlWindow == null)
            {
                return null;
            }

            // First try the usual way to get the document.
            try
            {
                IHTMLDocument2 doc = htmlWindow.document;
                return (IHTMLDocument3)doc;
            }
            catch (COMException comEx)
            {

                // I think COMException won't be ever fired but just to be sure ...

            }
            catch (UnauthorizedAccessException)
            {

            }
            catch (Exception ex)
            {

                return null;
            }

            // At this point the error was E_ACCESSDENIED because the frame contains a document from another domain.
            // IE tries to prevent a cross frame scripting security issue.
            try
            {
                // Convert IHTMLWindow2 to IWebBrowser2 using IServiceProvider.
                IServiceProvider sp = (IServiceProvider)htmlWindow;

                // Use IServiceProvider.QueryService to get IWebBrowser2 object.
                Object brws = null;
                sp.QueryService(ref IID_IWebBrowserApp, ref IID_IWebBrowser2, out brws);

                // Get the document from IWebBrowser2.
                IWebBrowser2 browser = (IWebBrowser2)(brws);

                return (IHTMLDocument3)browser.Document;
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }

            return null;
        }
    }
}
