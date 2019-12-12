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
using OSREEditor.View;
using System.Collections.Generic;

namespace OSREEditor.Model
{

    /// <summary>
    /// The c# proxy class for node instances.
    /// </summary>
    public class NodeProxy
    {
        public string Name { get; set; }

        /// <summary>
        /// The class constructor.
        /// </summary>
        public NodeProxy()
        {
            // empty
        }
    }

    /// <summary>
    /// The c# proxy class for stage instances.
    /// </summary>
    public class StageProxy
    {
        public string Name { get; set; }

        /// <summary>
        /// The class constructor.
        /// </summary>
        public StageProxy()
        {
            // empty
        }
    }

    /// <summary>
    /// The c# proxy class for world instances.
    /// </summary>
    public class WorldProxy
    {
        private IList<StageProxy> _stages;

        /// <summary>
        /// The class constructor.
        /// </summary>
        public WorldProxy()
        {
            _stages = new List<StageProxy>();
        }

        /// <summary>
        /// Will load an existing project file.
        /// </summary>
        /// <param name="name">The project-file</param>
        /// <param name="flags">Flags.</param>
        /// <returns></returns>
        public bool LoadWorld(string name, int flags)
        {
            return OSREWrapper.LoadProject(name, flags) == 0;
        }

        /// <summary>
        /// Will save the current project into a file.
        /// </summary>
        /// <param name="name">The project-file</param>
        /// <param name="flags">Flags.</param>
        /// <returns></returns>
        public bool SaveWorld(string name, int flags)
        {
            return OSREWrapper.SaveProject(name, flags) == 0;
        }

        /// <summary>
        /// Will add a new stage.
        /// </summary>
        /// <param name="newStage"></param>
        public void AddStage( StageProxy newStage ) 
        {
            if ( GetStage(newStage.Name)==null)
            {
                _stages.Add(newStage);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        public StageProxy GetStage(string name) 
        {
            foreach ( var currentStage in _stages) 
            {
                if ( currentStage.Name == name )
                {
                    return currentStage;
                }
            }
            return null;
        }
    }
}
