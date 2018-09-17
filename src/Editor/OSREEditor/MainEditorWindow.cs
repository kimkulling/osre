using System;
using System.Linq;
using System.Windows.Forms;
using OSREEditor.View;
using OSREEditor.Model;
using System.Runtime.InteropServices;

namespace OSREEditor
{
    public partial class MainEditorWindow : Form
    {
        private Project _project;

        private ProjectTreeView _projectTreeView;

        public MainEditorWindow()
        {
            InitializeComponent();

            this.MouseClick += Window_MouseClick;

            _project = new Project();
            _projectTreeView = new ProjectTreeView(ref this.treeView1);
        }

        private void quitToolStripMenuItem_Quit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void newToolStripMenuItem_New_Click(object sender, EventArgs e)
        {
            IntPtr windowsHandle = this.Handle;
            NewProject npDialog = new NewProject(windowsHandle, this);
            if ( npDialog.ShowDialog() == DialogResult.OK ) {
                _project = Project.Instance;
                if (_project != null) {
                }
            }
        }

        private void openToolStripMenuItem_Open_Click(object sender, EventArgs e) {
            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.InitialDirectory = "c:\\";
            openFileDialog.Filter = "txt files (*.osa)|*.osa|All files (*.*)|*.*";
            openFileDialog.FilterIndex = 2;
            openFileDialog.RestoreDirectory = true;

            if (openFileDialog.ShowDialog() == DialogResult.OK) {

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
                if (0 == retValue) {
                    string[] sepFolders  = filename.Split('\\');
                    string name = sepFolders[sepFolders.Length - 1];
                    _projectTreeView.NewProjectView(name);
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

        private void versionInfoToolStripMenuItem_Click( object sender, EventArgs e )
        {
            InfoDialog infoDialog = new InfoDialog();
            infoDialog.ShowDialog();
        }
    }
}
