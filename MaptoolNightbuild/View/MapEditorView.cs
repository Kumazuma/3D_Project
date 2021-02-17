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

namespace MaptoolNightbuild.View
{
    public partial class MapEditorView : UserControl
    {
        View.DockView<RenderView> renderView;
        View.DockView<ListBox> listbox;
        View.DockView<PropertyGrid> propertyView;
        MaptoolRenderer.OBJMesh chino;
        List<MaptoolRenderer.IRenderable> renderObjects = new List<MaptoolRenderer.IRenderable>();

        public MapEditorView()
        {
            InitializeComponent();
            InitializeDocViews();
            if(MaptoolRenderer.GraphicsDevice.Instance != null)
            {
                renderView.Content.GraphicsDevice = MaptoolRenderer.GraphicsDevice.Instance;
            }
            chino = new MaptoolRenderer.OBJMesh("./chino/gctitm001.obj");
            var objMeshObject = MaptoolRenderer.MeshObject.Create(chino);
            objMeshObject.Transform.Position = new MapToolCore.Position(0f, -1f, 2f);
            renderObjects.Add(objMeshObject);
            renderView.Content.RenderObjects = renderObjects;
            (renderView.Content.CurrentCamera as MaptoolRenderer.PersCamera).Angle = 45.0f;
            renderView.Content.CurrentCamera.Far = 1000.0f;
            renderView.Content.CurrentCamera.Near = 0.1f;
            renderView.Content.CurrentCamera.PropertyChanged += CurrentCamera_PropertyChanged;
            listbox.Content.Items.Add(renderView.Content.CurrentCamera);
            listbox.Content.Items.Add(objMeshObject);
        }

        private void CurrentCamera_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if(propertyView.Content.SelectedObject == renderView.Content.CurrentCamera)
            {
                propertyView.Content.SelectedObject = renderView.Content.CurrentCamera;
            }

        }

        public void InitializeDocViews()
        {
            renderView = new DockView<RenderView>();
            listbox = new DockView<ListBox>();
            propertyView = new DockView<PropertyGrid>();
            listbox.TabText = "월드 오브젝트";
            listbox.Show(dockPanel1, DockState.DockLeft);

            propertyView.Text = "Properties";
            propertyView.Show(dockPanel1, DockState.DockRight);

            renderView.TabText = "View";
            renderView.Show(dockPanel1, DockState.Document);
            renderView.CloseButtonVisible = false;
            renderView.CloseButton = false;
            renderView.IsFloat = false;
            listbox.Content.SelectedIndexChanged += ListBox_SelectedIndexChanged;
            
        }
        private void ListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            object[] objects = new object[listbox.Content.SelectedItems.Count];
            listbox.Content.SelectedItems.CopyTo(objects, 0);
            propertyView.Content.SelectedObject = listbox.Content.SelectedItem;
        }
    }
}
