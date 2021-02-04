using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MaptoolNightbuild
{
    public partial class Form1 : Form
    {
        MaptoolRenderer.PersCamera camera = new MaptoolRenderer.PersCamera();
        MaptoolRenderer.OBJMesh chino;
        List<MaptoolRenderer.IRenderable> renderables = new List<MaptoolRenderer.IRenderable>();
        public Form1()
        {
            InitializeComponent();
            MaptoolRenderer.GraphicsDevice.Initalize(panel1, 1920, 1080);
            camera.Angle = 45.0f;
            camera.Far = 1000.0f;
            camera.Near = 0.1f;
            chino = new MaptoolRenderer.OBJMesh("./chino/gctitm001.obj");
            var objMeshObject = new MaptoolRenderer.OBJObject(chino);
            objMeshObject.Transform.Position = new MapToolCore.Position(0f, -1f, 2f);
            renderables.Add(objMeshObject);
            propertyGrid1.SelectedObject = objMeshObject;
            objMeshObject.PropertyChanged += ObjMeshObject_PropertyChanged;
        }

        private void ObjMeshObject_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            MaptoolRenderer.GraphicsDevice.Instance.Render(panel1, renderables, camera);
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
        }

        private void splitContainer1_Panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            MaptoolRenderer.GraphicsDevice.Instance.Render(panel1, renderables, camera);
        }
    }
}
