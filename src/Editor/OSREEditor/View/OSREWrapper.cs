using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace OSREEditor.View
{
    public class OSREWrapper
    {
        const string EditorDllName = "osre_nativeeditor.dll";

        [DllImport(EditorDllName)]
        public static extern int CreateEditorApp(IntPtr handle);

        [DllImport(EditorDllName)]
        public static extern int EditorRequestNextFrame();

        [DllImport(EditorDllName)]
        public static extern int DestroyEditorApp();

        [DllImport(EditorDllName)]
        public static extern int NewProject([MarshalAs(UnmanagedType.LPStr)] string name);

        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int ImportAsset([MarshalAs(UnmanagedType.LPStr)] string code, int flags);

        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int LoadWorld([MarshalAs(UnmanagedType.LPStr)] string code, int flags);

        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int SaveWorld([MarshalAs(UnmanagedType.LPStr)] string code, int flags);

        // 
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void LogCallbackDelegate(int severity, string message);

        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        private static extern void RegisterLogCallback(IntPtr fc);

        private LogCallbackDelegate _logCallback;
        private static TextBox _logger;
        public OSREWrapper(TextBox logWindow)
        {
            _logCallback = null;
            _logger = logWindow;
        }

        delegate void LogDelegate(int severity, String msg);

        public void InitCSharpModules()
        {
            _logCallback = Log;
            var ipLog = Marshal.GetFunctionPointerForDelegate(_logCallback);
            if (ipLog != null)
            {
                RegisterLogCallback(ipLog);
            }
        }

        private static void Log(int severity, string message) {
            if (_logger.InvokeRequired) {
                LogDelegate delegateLog = new LogDelegate(Log);
                _logger.BeginInvoke(delegateLog, severity, message);
            } else {
                _logger?.AppendText(message + Environment.NewLine);
                //_logger.Text += message;
            }
        }
    }
}
 