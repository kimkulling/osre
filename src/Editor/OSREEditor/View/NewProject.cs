using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace OSREEditor.View
{
    public partial class NewProject : Form
    {
        IntPtr mHandle;

        public NewProject(IntPtr handle)
        {
            InitializeComponent();
            mHandle = handle;
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }
    }
}
