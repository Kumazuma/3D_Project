using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;

namespace MaptoolNightbuild
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            MaptoolRenderer.GraphicsDevice.Initalize(this, 1920, 1080);
            
            var editorView = new MapEditor.MainFrame();
            tabPage1.Controls.Add(editorView);
            editorView.Dock = DockStyle.Fill;
            var metaEditorView = new CharacterMetaEditor.MainFrame();
            tabPage2.Controls.Add(metaEditorView);
            metaEditorView.Dock = DockStyle.Fill;
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
        }

        private void splitContainer1_Panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            //MaptoolRenderer.GraphicsDevice.Instance.Render(panel1, renderables, camera);
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            //propertyGrid1.SelectedObject = comboBox1.SelectedItem;

        }
    }
}
