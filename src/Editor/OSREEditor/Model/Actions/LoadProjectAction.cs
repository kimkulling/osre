using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSREEditor.Model.Actions {

    /// <summary>
    /// WIll implement the loading of a project
    /// </summary>
    class LoadProjectAction : IAction {
        private Project _project;

        public LoadProjectAction( Project project)
        {
            _project = project;
        }

        public string Filename { get; set; }

        public int LoaderFlags { get; set; }

        public bool Execute() {
            if ( null == _project ) {
                return false;
            }

            return _project.LoadProject(Filename, LoaderFlags);
       }
    }
}
