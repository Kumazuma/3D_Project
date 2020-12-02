using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MapToolCore;
using MapToolRender;
namespace MapTool.View
{
    public partial class ColiiderEditView : UserControl
    {
        private RenderView renderView;
        private Collider collider;
        private HashSet<RenderObject> renderObjects;
        private RenderObject renderObject;
        private BoxColliderMeshObject boxMeshObject;
        private RenderObject sphareMeshObject;

        public ColiiderEditView()
        {
            InitializeComponent();
            comboColliderType.Items.Add("Box");
            comboColliderType.Items.Add("Sphare");
            renderView = new RenderView();
            renderView.Parent = splitContainer1.Panel2;
            splitContainer1.Panel2.Controls.Add(renderView);
            renderView.Dock = System.Windows.Forms.DockStyle.Fill;
            renderObjects = new HashSet<RenderObject>();
            renderView.RenderObjects = renderObjects;
            boxMeshObject = new BoxColliderMeshObject(GraphicsDevice.Instance);
            boxMeshObject.PropertyChanged += BoxMeshObject_PropertyChanged;
        }

        private void BoxMeshObject_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            renderView.Render();
        }

        private void comboColliderType_SelectedIndexChanged(object sender, EventArgs e)
        {
            renderObjects.Remove(boxMeshObject);
            RenderObject colObj = null;
            switch (comboColliderType.SelectedItem as string)
            {
                case "Box":
                    collider = new MapToolCore.BoxCollider();
                    boxMeshObject.Collider = collider as BoxCollider;
                    colObj = boxMeshObject;
                    break;
                case "Sphare":
                    collider = new MapToolCore.SphareCollider();
                    break;
            }
            if(colObj != null)
            {
                propertyGrid1.SelectedObject = colObj;
                renderObjects.Add(colObj);
            }
            renderView.Render();
        }

        private void Collider_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
        }

        private void btnLoadXMesh_Click(object sender, EventArgs e)
        {
            var fileDialog = new OpenFileDialog();
            fileDialog.Filter = "X 파일(*.x)|*.x";
            if (fileDialog.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            var path = fileDialog.FileName;

            var xmesh = Doc.MeshManager.Instance.GetStaticMesh(path);
            xmesh.Name = "XMesh";
            renderObject = xmesh;
            //TODO:콜라이더 메시도 같이 보일 수 있게 해줘야 함.
            renderObjects.Add(xmesh);
            renderView.Render();
        }

        private void btnOpenFile_Click(object sender, EventArgs e)
        {

        }

        private void btnSaveFile_Click(object sender, EventArgs e)
        {

        }

        private void renderView_SizeChanged(object sender, EventArgs e)
        {
            if (renderObject == null)
                return;

        }

    }
}
