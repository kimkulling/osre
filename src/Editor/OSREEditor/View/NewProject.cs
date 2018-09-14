using OSREEditor.Model;
using System;
using System.Windows.Forms;
using OSREEditor.Model.Actions;

namespace OSREEditor.View {
    public partial class NewProject : Form {
        IntPtr mHandle;

        public NewProject(IntPtr handle) {
            InitializeComponent();
            mHandle = handle;
            this.ProjectType.Select(0, 1);
        }

        private void NewProjectButton_Click(object sender, EventArgs e)
        {
            NewProjectAction projectAction = new NewProjectAction(mHandle);
            projectAction.ProjectName = this.ProjectName.Text;
            if (projectAction.Execute()) {
                Project.Instance = projectAction.CurrentProject;
                Project.Instance.ProjectAuthor = this.ProjectAuthor.Text;
                Project.Instance.MajorVersion = Convert.ToInt32( this.ProjectMajorVersion.Text );
                Project.Instance.MinorVersion = Convert.ToInt32( this.ProjectMinorVersion.Text );
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
