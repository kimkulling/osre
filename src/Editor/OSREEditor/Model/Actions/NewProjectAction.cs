using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSREEditor.Model.Actions
{
    public class NewProjectAction : IAction
    {
        public string ProjectName { get; set; }

        public Project CurrentProject { get; set; }

        public NewProjectAction()
        {
            // empty
        }

        public bool Execute()
        {
            if (ProjectName.Length == 0)
            {
                ProjectName = "New Project";
            }
            CurrentProject = new Project();
            CurrentProject.ProjectName = ProjectName;

            return true;
        }
    }
}
