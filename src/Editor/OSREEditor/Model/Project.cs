/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/

namespace OSREEditor.Model
{
    public enum ProjectType
    {
        Project2D,
        Project3D
    }

    public class Project
    {
        private string _projectName;

        private WorldProxy _world;

        public static Project Instance { get; set; }

        public Project()
        {
            _projectName = "untitled";
            _world = new WorldProxy();
        }

        public string ProjectName { get; set; }

        public string ProjectAuthor { get; set; }

        public int MajorVersion { get; set; }

        public int MinorVersion { get; set; }

        public ProjectType CurrentProjectType { get; set; }

        public bool LoadProject(string name, int flags)
        {
            if (_world == null)
            {
                return true;
            }

            return _world.LoadWorld(name, flags);
        }

        public bool SaveProject(string name, int flags)
        {
            if (_world == null)
            {
                return true;
            }

            return _world.SaveWorld(name, flags);

        }
    }
}
