using OSREEditor.Model;
using OSREEditor.Model.Actions;
using OSREEditor.View;
using System;
using System.Windows.Forms;

namespace OSREEditor
{
    public partial class MainEditorWindow : Form
    {
        private Project _project;

        private ProjectTreeView _projectTreeView;

        private readonly OSREWrapper _osreWrapper;

        public MainEditorWindow()
        {
            InitializeComponent();

            this.MouseClick += Window_MouseClick;

            _project = new Project();
            _projectTreeView = new ProjectTreeView(ref this.treeView1);
            _osreWrapper = new OSREWrapper(this.logWindow);
            _osreWrapper.InitCSharpModules();
        }

        private void quitToolStripMenuItem_Quit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void newToolStripMenuItem_New_Click(object sender, EventArgs e)
        {
            IntPtr windowsHandle = this.panel3d.Handle;
            if ( windowsHandle == null )
            {
                return;
            }

            NewProject npDialog = new NewProject( windowsHandle, this );
            if (npDialog.ShowDialog() == DialogResult.OK)
            {
                _project = Project.Instance;
                if (_project != null)
                {
                }
            }
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
                LoadProjectAction action = new LoadProjectAction(_project);
                action.Filename = openFileDialog.FileName;
                action.LoaderFlags = 0;
                if (!action.Execute())
                {
                    System.Console.WriteLine("Error while loading project.");
                }
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
                SaveProjectAction action = new SaveProjectAction(_project);
                action.Filename = saveFileDlg.FileName;
                action.SaveFlags = 0;
                if (!action.Execute())
                {
                    System.Console.WriteLine("Error while saving project.");
                }
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
                if (0 == retValue)
                {
                    string[] sepFolders = filename.Split('\\');
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

        private void versionInfoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            InfoDialog infoDialog = new InfoDialog();
            infoDialog.ShowDialog();
        }
    }
}
