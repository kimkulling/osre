/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
using OSREEditor.View;
using System;
using System.Windows.Forms;

namespace OSREEditor.Model.Actions
{
    public class NewProjectAction : IAction
    {
        public string ProjectName { get; set; }

        private IntPtr mHandle;
        private Form mMainWindow;
        public Project CurrentProject { get; set; }
        private int _width;
        private int _height;

        public NewProjectAction(IntPtr handle, Form mainWindow, int width, int height)
        {
            mHandle = handle;
            mMainWindow = mainWindow;
            _width = width;
            _height = height;
        }

        public bool Execute()
        {
            if (mHandle == null)
            {
                return false;
            }

            if (ProjectName.Length == 0)
            {
                ProjectName = "New Project";
            }
            if (mMainWindow != null)
            {
                mMainWindow.Text = ProjectName;
            }
            CurrentProject = new Project
            {
                ProjectName = ProjectName
            };
            int retCode = 0;
            retCode = OSREWrapper.CreateEditorApp(mHandle, _width, _height);
            OSREWrapper.NewProject(CurrentProject.ProjectName);

            return 0 == retCode;
        }
    }
}
