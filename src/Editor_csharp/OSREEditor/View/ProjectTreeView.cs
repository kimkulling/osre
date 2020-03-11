using System.Windows.Forms;

namespace OSREEditor.View
{
    /// <summary>
    /// The project tree-view implementation.
    /// </summary>
    public class ProjectTreeView
    {
        #region Private attributes 

        private TreeView projectTreeView;

        #endregion

        #region Public API

        /// <summary>
        /// The class constructor with the parameters.
        /// </summary>
        /// <param name="treeView">The treeview isntance</param>
        public ProjectTreeView(ref TreeView treeView)
        {
            this.projectTreeView = treeView;
        }

        /// <summary>
        /// The class constructor.
        /// </summary>
        public ProjectTreeView()
        {
            this.projectTreeView = new TreeView();
        }

        /// <summary>
        /// Will create a new project view.
        /// </summary>
        /// <param name="name"></param>
        public void NewProjectView(string name)
        {
            if (projectTreeView.Nodes.Count > 0)
            {
                this.projectTreeView.Nodes.Clear();
            }

            var newNode = new TreeNode(name);

            this.projectTreeView.BeginUpdate();
            this.projectTreeView.Nodes.Add(newNode);
            this.projectTreeView.EndUpdate();
        }

        /// <summary>
        /// Will add a new node.
        /// </summary>
        /// <param name="name">The node name</param>
        public void AddNode(string name)
        {
            if (name.Length == 0)
            {
                return;
            }
            this.projectTreeView.Nodes.Add(new TreeNode(name));
        }

        #endregion
    }
}
