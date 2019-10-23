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
    /// <summary>
    /// The enum to describe the project type.
    /// </summary>
    public enum ProjectType
    {
        /// <summary>2D-project </summary>
        Project2D,
        /// <summary>3D-project </summary>
        Project3D
    }

    /// <summary>
    /// This class stores all project-relevant data.
    /// </summary>
    public class Project
    {
        private WorldProxy _world;

        public static Project Instance { get; set; }

        /// <summary>
        /// The class constructor.
        /// </summary>
        public Project()
        {
            ProjectName = "untitled";
            _world = new WorldProxy();
        }

        /// <summary>
        /// The project-name property access.
        /// </summary>
        public string ProjectName { get; set; }

        /// <summary>
        /// The project-author property access.
        /// </summary>
        public string ProjectAuthor { get; set; }

        /// <summary>
        /// The major-version property access.
        /// </summary>
        public int MajorVersion { get; set; }

        /// <summary>
        /// The minor-version property access.
        /// </summary>
        public int MinorVersion { get; set; }

        /// <summary>
        /// The project-type property access.
        /// </summary>
        public ProjectType CurrentProjectType { get; set; }

        /// <summary>
        /// Will load an existing project file.
        /// </summary>
        /// <param name="name">The project-file</param>
        /// <param name="flags">Flags.</param>
        /// <returns></returns>
        public bool LoadProject(string name, int flags)
        {
            if (_world == null)
            {
                return true;
            }

            return _world.LoadWorld(name, flags);
        }

        /// <summary>
        /// Will save the current project into a file.
        /// </summary>
        /// <param name="name">The project-file</param>
        /// <param name="flags">Flags.</param>
        /// <returns></returns>
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
