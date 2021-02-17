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
    public partial class CharacterMetaEditor : UserControl
    {
        View.DockView<RenderView> renderView;
        View.DockView<ColliderPropertyView> colliderView;
        View.DockView<AnimationTableView> animationTableView;
        View.DockView<PropertyGrid> propertyView;
        MaptoolRenderer.OBJMesh chino;
        List<MaptoolRenderer.IRenderable> renderObjects = new List<MaptoolRenderer.IRenderable>();
        public CharacterMetaEditor()
        {
            InitializeComponent();
            InitializeDocViews();

            (renderView.Content.CurrentCamera as MaptoolRenderer.PersCamera).Angle = 45.0f;
            renderView.Content.CurrentCamera.Far = 1000.0f;
            renderView.Content.CurrentCamera.Near = 0.1f;

            if (MaptoolRenderer.GraphicsDevice.Instance != null)
            {
                renderView.Content.GraphicsDevice = MaptoolRenderer.GraphicsDevice.Instance;
                renderView.Content.Render();
            }

            chino = new MaptoolRenderer.OBJMesh("./chino/gctitm001.obj");
            var objMeshObject = MaptoolRenderer.MeshObject.Create(chino);
            objMeshObject.Transform.Position = new MapToolCore.Position(0f, -1f, 2f);
            renderObjects.Add(objMeshObject);
            renderView.Content.RenderObjects = renderObjects;
            GotFocus += CharacterMetaEditor_GotFocus;
        }

        private void CharacterMetaEditor_GotFocus(object sender, EventArgs e)
        {
            renderView.Content.Render();

        }

        public void InitializeDocViews()
        {
            renderView = new DockView<RenderView>();
            colliderView = new DockView<ColliderPropertyView>();
            animationTableView = new DockView<AnimationTableView>();
            renderView.TabText = "View";
            renderView.Show(dockPanel1, DockState.Document);
            renderView.CloseButtonVisible = false;
            renderView.CloseButton = false;
            renderView.IsFloat = false;

            colliderView.TabText = "Colliders";
            colliderView.Show(dockPanel1, DockState.DockLeft);

            animationTableView.TabText = "Animations";
            animationTableView.Show(dockPanel1, DockState.DockLeft);

        }
    }
}
