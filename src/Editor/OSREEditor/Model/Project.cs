using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSREEditor.Model
{
    public class Project
    {
        string _projectName;

        WorldProxy  _world;

        public Project() {
            _projectName = "untitled";
            _world = new WorldProxy();
        }

        public string ProjectName {
            get {
                return _projectName;
            }

            set {
                if (_projectName != value)
                {
                    _projectName = value;
                }
            }
        }
    }
}
