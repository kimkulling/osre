using System;
using System.Windows.Forms;
using OSREEditor.View;
using OSREEditor.Model;

namespace OSREEditor
{
    public partial class MainEditorWindow : Form
    {
        Project _project;

        public MainEditorWindow()
        {
            InitializeComponent();

            //this.MouseWheel += Window_MouseWheel;
            this.MouseClick += Window_MouseClick;

            _project = new Project();
        }

        private void quitToolStripMenuItem_Quit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void newToolStripMenuItem_New_Click(object sender, EventArgs e)
        {
            IntPtr windowsHandle = this.Handle;
            OSREWrapper.CreateEditorApp(windowsHandle);
            _project.ProjectName = "New Project";

            OSREWrapper.NewProject(_project.ProjectName);
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

        private void importAssetToolStripMenuItem_Click(object sender, EventArgs e) {
            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.InitialDirectory = "c:\\";
            openFileDialog.Filter = "txt files (*.osa)|*.osa|All files (*.*)|*.*";
            openFileDialog.FilterIndex = 2;
            openFileDialog.RestoreDirectory = true;

            if (openFileDialog.ShowDialog() == DialogResult.OK) {
                var filename = openFileDialog.FileName;
                var retValue = OSREWrapper.ImportAsset(filename, 0);
                if ( 0 == retValue ) {
                    int numItems = OSREWrapper.GetNumItems();
                    if ( numItems == 0 )
                    {
                        return;
                    }

                    OSREWrapper.NativeStreeItem[] items = new OSREWrapper.NativeStreeItem[numItems];
                    OSREWrapper.GetNodeHierarchy(numItems, items);
                    this.treeView1.BeginUpdate();
                    TreeNode newNode = new TreeNode("root");
                    treeView1.Nodes.Add(newNode);
                    foreach ( var item in items )
                    {
                        newNode = new TreeNode(item.m_name);
                        treeView1.Nodes[0].Nodes.Add(newNode);
                    }
                    this.treeView1.EndUpdate();                
                }
            }
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

            OSREWrapper.EditorRequestNextFrame();
        }

        private void Window_MouseClick(object sender, MouseEventArgs e)
        {
            bool isDown = IsDown(e.Clicks);
            OSREWrapper.EditorRequestNextFrame();
            /*int clicked = 0;
            if (isDown) {
                clicked = 1;
            }

            OSREWrapper.CSharpEvent ev;
            ev.x = e.X;
            ev.y = e.Y;*/
        }
    }
}
