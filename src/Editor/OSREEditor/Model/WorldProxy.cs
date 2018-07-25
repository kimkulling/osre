using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSREEditor.Model
{

    public class NodeProxy
    {
        public NodeProxy()
        {

        }
    }

    public class StageProxy
    {
        public StageProxy()
        {

        }
    }

    public class WorldProxy
    {
        private IList<StageProxy> _stages;

        public WorldProxy()
        {
            _stages = new List<StageProxy>();
        }
    }
}
