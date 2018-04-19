using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using OSREEditor.View;

namespace OSREEditor
{
    public partial class MainEditorWindow : Form
    {
        public MainEditorWindow()
        {
            InitializeComponent();
        }

        private void quitToolStripMenuItem_Quit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void newToolStripMenuItem_New_Click(object sender, EventArgs e)
        {
            OSREWrapper.CreateEditorApp();
        }

        private void openToolStripMenuItem_Open_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.InitialDirectory = "c:\\";
            openFileDialog.Filter = "txt files (*.osa)|*.osa|All files (*.*)|*.*";
            openFileDialog.FilterIndex = 2;
            openFileDialog.RestoreDirectory = true;

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {

            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDlg = new SaveFileDialog();
            saveFileDlg.InitialDirectory = "c:\\";
            saveFileDlg.Filter = "txt files (*.osa)|*.osa|All files (*.*)|*.*";
            saveFileDlg.FilterIndex = 2;
            saveFileDlg.RestoreDirectory = true;

            if (saveFileDlg.ShowDialog() == DialogResult.OK)
            {

            }
        }
    }
}
