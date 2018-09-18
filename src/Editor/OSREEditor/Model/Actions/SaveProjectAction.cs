using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSREEditor.Model.Actions {

    /// <summary>
    /// Will save the project.
    /// </summary>
    public class SaveProjectAction : IAction {

        private Project _project;

        /// <summary>
        /// The class constructor with the project instance.
        /// </summary>
        /// <param name="project">The project instance.</param>
        public SaveProjectAction( Project project ) {
            _project = project;
            Filename = "none";
            SaveFlags = 0;
        }

        public string Filename { get; set; }

        public int SaveFlags { get; set; }

        public bool Execute()
        {
            if (null == _project) {
                return false;
            }

            return _project.SaveProject(Filename, SaveFlags);
        }
    }
}
