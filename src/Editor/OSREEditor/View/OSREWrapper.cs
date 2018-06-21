using System;
using System.Runtime.InteropServices;

namespace OSREEditor.View
{
    public static class OSREWrapper
    {
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public struct NativeStreeItem {
            [MarshalAs(UnmanagedType.LPStr)]
            public String m_name;
            [MarshalAs(UnmanagedType.I4)]
            public int m_parentId;
            [MarshalAs(UnmanagedType.I4)]
            public int m_numChildren;
            [MarshalAs(UnmanagedType.ByValArray)]
            public int[] m_childrenIds;
        }

        public const int MouseDown = 1;

        public const int MouseUp = 2;

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi, Pack = 1)]
        public struct CSharpEvent {
            [MarshalAs(UnmanagedType.SysInt)]
            public int type;
            [MarshalAs(UnmanagedType.SysInt)]
            public int x;
            [MarshalAs(UnmanagedType.SysInt)]
            public int y;
            [MarshalAs(UnmanagedType.SysInt)]
            public int mouseBtnState;
        }
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
        public static extern int EnqueueEvent(CSharpEvent ev);

        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int GetNumItems();

        [DllImport(EditorDllName, CharSet = CharSet.Auto)]
        public static extern int GetNodeHierarchy(int numItems, [MarshalAs(UnmanagedType.LPArray)] NativeStreeItem[] items);
    }
}
 