using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using OSREEditor.View;

namespace OSREEditor
{
    public partial class MainEditorWindow : Form
    {
        public MainEditorWindow()
        {
            InitializeComponent();
            this.MouseMove += Window_MouseMove;
            this.MouseWheel += Window_MouseWheel;
            this.MouseDown += Window_MouseClick;
            this.MouseUp += Window_MouseClick;
        }

        private void quitToolStripMenuItem_Quit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void newToolStripMenuItem_New_Click(object sender, EventArgs e)
        {
            IntPtr windowsHandle = this.Handle;
            OSREWrapper.CreateEditorApp(windowsHandle);
        }

        private void openToolStripMenuItem_Open_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.InitialDirectory = "c:\\";
            openFileDialog.Filter = "txt files (*.osa)|*.osa|All files (*.*)|*.*";
            openFileDialog.FilterIndex = 2;
            openFileDialog.RestoreDirectory = true;

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {

            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDlg = new SaveFileDialog();
            saveFileDlg.InitialDirectory = "c:\\";
            saveFileDlg.Filter = "txt files (*.osa)|*.osa|All files (*.*)|*.*";
            saveFileDlg.FilterIndex = 2;
            saveFileDlg.RestoreDirectory = true;

            if (saveFileDlg.ShowDialog() == DialogResult.OK)
            {

            }
        }

        private void importAssetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.InitialDirectory = "c:\\";
            openFileDialog.Filter = "txt files (*.osa)|*.osa|All files (*.*)|*.*";
            openFileDialog.FilterIndex = 2;
            openFileDialog.RestoreDirectory = true;

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                var filename = openFileDialog.FileName;
                var retValue = OSREWrapper.ImportAsset(filename, 0);
                if ( 0 == retValue )
                {
                    int numItems = OSREWrapper.GetNumItems();
                    if ( numItems == 0 )
                    {
                        return;
                    }

                    OSREWrapper.NativeStreeItem[] items = new OSREWrapper.NativeStreeItem[numItems];
                    OSREWrapper.GetNodeHierarchy(numItems, items);
                    this.treeView1.BeginUpdate();
                    TreeNode newNode = new TreeNode();
                    foreach ( var item in items )
                    {
                        var name = item.m_name;
                    }
                    this.treeView1.EndUpdate();

                //    this.treeView1.SelectedNode.
                
                }
            }
        }

        private void Window_MouseMove(object sender, MouseEventArgs e)
        {

        }

        private bool IsDown(int numClicks)
        {
            bool down = true;
            if (numClicks % 2 != 0)
            {
                down = false;
            }
            return down;
        }

        private void Window_MouseWheel(object sender, MouseEventArgs e)
        {
            bool isDown = IsDown(e.Clicks);
        }

        private void Window_MouseClick(object sender, MouseEventArgs e)
        {
            bool isDown = IsDown(e.Clicks);
            int clicked = 0;
            if (isDown)
            {
                clicked = 1;
            }

            int x = e.X;
            int y = e.Y;

        }
    }
}
