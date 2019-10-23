using OSREEditor.Model;
using OSREEditor.Model.Actions;
using System;
using System.Windows.Forms;

namespace OSREEditor.View
{
    /// <summary>
    /// this class implements the dialog to create a new project.
    /// </summary>
    public partial class NewProject : Form
    {
        private readonly IntPtr _handle;

        private readonly Form _parent;

        /// <summary>
        /// The class constructor with the parameters.
        /// </summary>
        /// <param name="handle">The windows handle of the engine surface.</param>
        /// <param name="parent">The parent form.</param>
        public NewProject(IntPtr handle, Form parent )
        {
            InitializeComponent();
            _handle = handle;
            _parent = parent;
            this.ProjectType.Select(0, 1);
        }

        private void NewProjectButton_Click(object sender, EventArgs e)
        {
            NewProjectAction projectAction = new NewProjectAction(_handle, _parent);
            projectAction.ProjectName = this.ProjectName.Text;
            if (projectAction.Execute())
            {
                Project.Instance = projectAction.CurrentProject;
                Project.Instance.ProjectAuthor = this.ProjectAuthor.Text;
                Project.Instance.MajorVersion = Convert.ToInt32(this.ProjectMajorVersion.Text);
                Project.Instance.MinorVersion = Convert.ToInt32(this.ProjectMinorVersion.Text);
                this.DialogResult = DialogResult.OK;
            }
            else
            {
                this.DialogResult = DialogResult.Cancel;
            }
            this.Close();
        }

        private void CancelProjectButton_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
