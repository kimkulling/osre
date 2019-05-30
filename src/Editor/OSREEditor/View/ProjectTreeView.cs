using System.Windows.Forms;

namespace OSREEditor.View
{
    public class ProjectTreeView
    {

        private TreeView projectTreeView;

        public ProjectTreeView(ref TreeView treeView)
        {
            this.projectTreeView = treeView;
        }

        public ProjectTreeView()
        {
            this.projectTreeView = new TreeView();
        }

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

        public void AddNode(string name)
        {
            if (name.Length == 0)
            {
                return;
            }
            this.projectTreeView.Nodes.Add(new TreeNode(name));
        }
    }
}
