using OSREEditor.Model;
using System;
using System.Windows.Forms;
using OSREEditor.Model.Actions;

namespace OSREEditor.View
{
    public partial class NewProject : Form
    {
        IntPtr mHandle;

        public NewProject(IntPtr handle)
        {
            InitializeComponent();
            mHandle = handle;
        }

        private void NewProjectButton_Click(object sender, EventArgs e)
        {
            NewProjectAction projectAction = new NewProjectAction();
            projectAction.ProjectName = this.ProjectName.Text;
            if (projectAction.Execute())
            {
                Project.Instance = projectAction.CurrentProject;
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
