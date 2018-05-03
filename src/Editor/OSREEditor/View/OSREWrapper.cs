using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;  

namespace OSREEditor.View
{
    public static class OSREWrapper
    {
        const string EditorDllName = "osre_nativeeditor.dll";

        [DllImport(EditorDllName)]
        public static extern int CreateEditorApp( IntPtr handle );

        [DllImport(EditorDllName)]
        public static extern int EditorRequestNextFrame();

        [DllImport(EditorDllName)]
        public static extern int DestroyEditorApp();

        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int ImportAsset([MarshalAs(UnmanagedType.LPStr)] string code, int flags );
    }
}
 