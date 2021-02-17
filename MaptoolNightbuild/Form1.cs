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
        MaptoolRenderer.PersCamera camera = new MaptoolRenderer.PersCamera();

        MaptoolRenderer.SkinnedXMesh xMesh;
        MaptoolRenderer.SkinnedXMeshObject xMeshObject;
        Timer renderTimer = new Timer();
        //MaptoolRenderer.BoxMesh boxMesh;
        List<MaptoolRenderer.IRenderable> renderables = new List<MaptoolRenderer.IRenderable>();
        public Form1()
        {
            InitializeComponent();
            MaptoolRenderer.GraphicsDevice.Initalize(this, 1920, 1080);

            var editorView = new View.MapEditorView();
            tabPage1.Controls.Add(editorView);
            editorView.Dock = DockStyle.Fill;
            var metaEditorView = new View.CharacterMetaEditor();
            tabPage2.Controls.Add(metaEditorView);
            metaEditorView.Dock = DockStyle.Fill;
            //camera.Angle = 45.0f;
            //camera.Far = 1000.0f;
            //camera.Near = 0.1f;
            //chino = new MaptoolRenderer.OBJMesh("./chino/gctitm001.obj");
            //xMesh = new MaptoolRenderer.SkinnedXMesh("./player/player.x");
            //xMeshObject = new MaptoolRenderer.SkinnedXMeshObject(xMesh);
            ////boxMesh = new MaptoolRenderer.BoxMesh();
            //var colliderObject = new MaptoolRenderer.ColliderObject();
            //colliderObject.ParentObject = xMeshObject;
            //var objMeshObject = MaptoolRenderer.MeshObject.Create(chino);
            //objMeshObject.Transform.Position = new MapToolCore.Position(0f, -1f, 2f);
            //renderables.Add(xMeshObject);
            //renderables.Add(colliderObject);
            //propertyGrid1.SelectedObject = colliderObject;
            //colliderObject.PropertyChanged += ObjMeshObject_PropertyChanged;
            //xMeshObject.PropertyChanged += ObjMeshObject_PropertyChanged;
            //comboBox1.Items.Add(xMeshObject);
            //comboBox1.Items.Add(colliderObject);
            //comboBox1.SelectedItem = colliderObject;
            //renderTimer.Tick += RenderTimer_Tick;
            //renderTimer.Interval = 1;
        }

        private void RenderTimer_Tick(object sender, EventArgs e)
        {
            lock (renderTimer)
            {
               // propertyGrid1.SelectedObject = comboBox1.SelectedItem;
                //MaptoolRenderer.GraphicsDevice.Instance.Render(panel1, renderables, camera);
                //renderTimer.Stop();
            }
        }

        private void ObjMeshObject_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            lock(renderTimer)
            {
                if (!renderTimer.Enabled)
                {
                    renderTimer.Start();
                }
            }
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
