using OSREEditor.Model;
using OSREEditor.Model.Actions;
using OSREEditor.View;
using System;
using System.Windows.Forms;

namespace OSREEditor
{
    /// <summary>
    /// This class implements the main form.
    /// </summary>
    public partial class MainEditorWindow : Form, IDisposable
    {
        private Project _project;

        private ProjectTreeView _projectTreeView;

        private readonly OSREWrapper _osreWrapper;

        /// <summary>
        /// The class constructor.
        /// </summary>
        public MainEditorWindow()
        {
            InitializeComponent();

            this.panel3d.MouseClick += Window_MouseClick;
            this.panel3d.MouseMove += Window_MouseMove;

            this.MouseWheel += Window_MouseWheel;
            this.Resize += OnResize;
            
            _project = new Project();
            _projectTreeView = new ProjectTreeView(ref this.treeView1);
            _osreWrapper = new OSREWrapper(this.logWindow);
            _osreWrapper.InitCSharpModules();
        }

        /// <summary>
        /// The class destructor.
        /// </summary>
        ~MainEditorWindow()
        {
            this.MouseWheel -= Window_MouseWheel;
            this.panel3d.MouseClick -= Window_MouseClick;
            this.panel3d.MouseMove -= Window_MouseMove;
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
            }
        }

        private void openToolStripMenuItem_Open_Click(object sender, EventArgs e) {
            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.InitialDirectory = "c:\\";
            openFileDialog.Filter = "txt files (*.osa)|*.osa|All files (*.*)|*.*";
            openFileDialog.FilterIndex = 2;
            openFileDialog.RestoreDirectory = true;

            if (openFileDialog.ShowDialog() == DialogResult.OK) {
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

        private void importAssetToolStripMenuItem_Click(object sender, EventArgs e) {
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
            OSREWrapper.EditorRequestNextFrame();
        }

        private bool IsDown(int numClicks)
        {
            bool down = true;
            if (numClicks % 2 == 0)
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
            OSREWrapper.LeftMousePressed(e.X, e.Y, isDown);
            OSREWrapper.EditorRequestNextFrame();
        }

        private void Window_MouseMove(object sender, MouseEventArgs e) 
        {
            bool isDown = IsDown(e.Clicks);

        }

        private void OnResize(object sender, System.EventArgs e) {
            MainEditorWindow mainWindow = (MainEditorWindow) sender;
            if ( mainWindow != null) 
            {
                var parentSize = mainWindow.panel3d.Size;
                OSREWrapper.EditorResize(0, 0, parentSize.Width, parentSize.Height);
            }
        }

        private void versionInfoToolStripMenuItem_Click(object sender, EventArgs e) 
        {
            InfoDialog infoDialog = new InfoDialog();
            infoDialog.ShowDialog();
        }
    }
}
