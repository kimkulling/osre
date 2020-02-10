using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace OSREEditor.View 
{
    /// <summary>
    /// The world struct
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct OSREWorld 
    {
        public int NumStages;   
    }

    /// <summary>
    /// The stage struct
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct OSREStage 
    {

    }

    /// <summary>
    /// The node struct
    /// </summary>
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct OSRENode 
    {

    }

    /// <summary>
    /// The OSRE native wrapper, contains prototypes for calling native methods from OSRE.
    /// </summary>
    public class OSREWrapper : IDisposable {
        
        #region Private Attributes

#if DEBUG
        private const string EditorDllName = "osre_nativeeditord.dll";
#else
        private const string EditorDllName = "osre_nativeeditor.dll";
#endif
        private static TextBox _logger;

#endregion

        #region Native Interface Wrapper

        /// <summary>
        /// Will create the editor native application instance.
        /// </summary>
        /// <param name="handle"></param>
        /// <returns>0 for successful, != 0 in case of an error.</returns>
        [DllImport(EditorDllName)]
        public static extern int CreateEditorApp(IntPtr handle);

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        [DllImport(EditorDllName)]
        public static extern int EditorUpdate();

        /// <summary>
        /// Will request the next frame to render
        /// </summary>
        /// <returns>0 for successful, != 0 in case of an error.</returns>
        [DllImport(EditorDllName)]
        public static extern int EditorRequestNextFrame();

        /// <summary>
        /// Will destroy the editor app.
        /// </summary>
        /// <returns>0 for successful, != 0 in case of an error.</returns>
        [DllImport(EditorDllName)]
        public static extern int DestroyEditorApp();

        /// <summary>
        /// Will open a new project.
        /// </summary>
        /// <param name="name">The name for the new project</param>
        /// <returns>0 for successful, != 0 in case of an error.</returns>
        [DllImport(EditorDllName)]
        public static extern int NewProject([MarshalAs(UnmanagedType.LPStr)] string name);

        /// <summary>
        /// Will perform an import of an asset.
        /// </summary>
        /// <param name="name">The asset name</param>
        /// <param name="flags">The importer flags</param>
        /// <returns>0 for successful, != 0 in case of an error.</returns>
        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int ImportAsset([MarshalAs(UnmanagedType.LPStr)] string name, int flags);

        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int openWorldAccess([MarshalAs(UnmanagedType.LPStr)] string name);

        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int openStageAccess([MarshalAs(UnmanagedType.LPStr)] string name );

        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int openNodeAccess([MarshalAs(UnmanagedType.LPStr)] string name);
        
        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int createNode([MarshalAs(UnmanagedType.LPStr)] string name, [MarshalAs(UnmanagedType.LPStr)] string parent );
        
        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int closeNodeAccess();

        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int closeStageAccess();

        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int closeWorldAccess();

        /// <summary>
        /// todo!
        /// </summary>
        /// <param name="world"></param>
        /// <returns></returns>
        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern unsafe int OpenWorldAccess( string name );

        /// <summary>
        /// todo!
        /// </summary>
        /// <param name="world"></param>
        /// <returns></returns>
        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern unsafe int closeWorldAccess(OSREWorld* world);

        /// <summary>
        /// Will load a project.
        /// </summary>
        /// <param name="name">The project name</param>
        /// <param name="flags">Load flags, currently unused</param>
        /// <returns>0 for successful, != 0 in case of an error.</returns>
        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int LoadProject([MarshalAs(UnmanagedType.LPStr)] string name, int flags);

        /// <summary>
        /// Will save the current project.
        /// </summary>
        /// <param name="name">The project name</param>
        /// <param name="flags">Save flags, currently unused</param>
        /// <returns>0 for successful, != 0 in case of an error.</returns>
        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int SaveProject([MarshalAs(UnmanagedType.LPStr)] string name, int flags);

        /// <summary>
        /// Delegate for the log system.
        /// </summary>
        /// <param name="severity">The log severity</param>
        /// <param name="message">The log message</param>
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void LogCallbackDelegate(int severity, string message);

        /// <summary>
        /// Will send a left button clicked event to OSRE
        /// </summary>
        /// <param name="x">The x-pos of the mouse cursor</param>
        /// <param name="y">The y-pos of the mouse cursor</param>
        /// <param name="pressed">true for pressed</param>
        /// <returns>!= 0 in case of an error</returns>
        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int LeftMousePressed(int x, int y, bool pressed);

        /// <summary>
        /// Will send a middle button clicked event to OSRE
        /// </summary>
        /// <param name="x">The x-pos of the mouse cursor</param>
        /// <param name="y">The y-pos of the mouse cursor</param>
        /// <param name="pressed">true for pressed</param>
        /// <returns>!= 0 in case of an error</returns>
        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int MiddleMousePressed(int x, int y, bool pressed);

        /// <summary>
        /// Will send a right button clicked event to OSRE
        /// </summary>
        /// <param name="x">The x-pos of the mouse cursor</param>
        /// <param name="y">The y-pos of the mouse cursor</param>
        /// <param name="pressed">true for pressed</param>
        /// <returns>!= 0 in case of an error</returns>
        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int RightMousePressed(int x, int y, bool pressed);

        /// <summary>
        /// The resize callback.
        /// </summary>
        /// <param name="x">New x position</param>
        /// <param name="y">New y position</param>
        /// <param name="w">New width</param>
        /// <param name="h">New height</param>
        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern void EditorResize(int x, int y, int w, int h);

        #endregion

        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        private static extern void RegisterLogCallback(IntPtr fc);

        /// <summary>
        /// The class constructor.
        /// </summary>
        /// <param name="logWindow">The window to log into.</param>
        public OSREWrapper(TextBox logWindow) {
            _logger = logWindow;
        }

        delegate void LogDelegate(int severity, String msg);

        /// <summary>
        /// Will do the handshake between c++ and c# for delegates and function pointers.
        /// </summary>
        public void InitCSharpModules() {
            LogCallbackDelegate logCallback = Log;
            var dir = System.IO.Directory.GetCurrentDirectory();
            var ipLog = Marshal.GetFunctionPointerForDelegate(logCallback);
            if (ipLog != null) {
                RegisterLogCallback(ipLog);
            }
        }

        /// <summary>
        /// The dispose implementation.
        /// </summary>
        public void Dispose() {
            DestroyEditorApp();
        }

        private static void Log(int severity, string message) {
            if (_logger.InvokeRequired) {
                LogDelegate delegateLog = new LogDelegate(Log);
                _logger?.BeginInvoke(delegateLog, severity, message);
            } else {
                _logger?.AppendText(message + Environment.NewLine);
            }
        }
    }
}
