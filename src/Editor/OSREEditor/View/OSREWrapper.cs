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
        public unsafe static extern int CreateEditorApp();

        [DllImport(EditorDllName)]
        public unsafe static extern int EditorRequestNextFrame();

        [DllImport(EditorDllName)]
        public unsafe static extern int DestroyEditorApp();

    }
}
 