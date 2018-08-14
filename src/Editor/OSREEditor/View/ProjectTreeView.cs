using System.Windows.Forms;

namespace OSREEditor.View
{
    public class ProjectTreeView {

        private TreeView _projectTreeView;

        public ProjectTreeView(ref TreeView treeView) {
            _projectTreeView = treeView;
        }

        public void NewProjectView(string name) {
            if (_projectTreeView.Nodes.Count > 0) {
                _projectTreeView.Nodes.Clear();
            }

            _projectTreeView.BeginUpdate();
            TreeNode newNode = new TreeNode(name);
            _projectTreeView.Nodes.Add(newNode);
            _projectTreeView.EndUpdate();
        }
    }
}
