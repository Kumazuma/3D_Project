using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections.Specialized;
using WeifenLuo.WinFormsUI.Docking;

namespace MaptoolNightbuild.View
{
    public partial class CharacterMetaEditor : UserControl
    {
        View.DockView<CharacterMetaRenderView> renderView;
        View.DockView<ColliderPropertyView> colliderView;
        View.DockView<AnimationTableView> animationTableView;
        View.DockView<PropertyGrid> propertyView;
        //MaptoolRenderer.OBJMesh chino;
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
            Doc.CharacterMetaDoc doc = Doc.CharacterMetaDoc.Instance;
            doc.PropertyChanged += Doc_PropertyChanged;
            //chino = new MaptoolRenderer.OBJMesh("./chino/gctitm001.obj");
            //var objMeshObject = MaptoolRenderer.MeshObject.Create(chino);
            //objMeshObject.Transform.Position = new MapToolCore.Position(0f, -1f, 2f);
            //doc.RenderObjects.Add(objMeshObject);
            doc.RenderObjects.CollectionChanged += RenderObjects_CollectionChanged;
            renderView.Content.RenderObjects = doc.RenderObjects;
            
            GotFocus += CharacterMetaEditor_GotFocus;
        }

        private void RenderObjects_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            renderView.Content.Render();
            if(e.Action == NotifyCollectionChangedAction.Add)
            {
                foreach(var item in e.NewItems)
                {
                    var obj = item as INotifyPropertyChanged;
                    if(obj != null)
                    {
                        obj.PropertyChanged += RenderObject_PropertyChanged;
                    }
                }
            }
        }

        private void RenderObject_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            renderView.Content.Render();
        }

        private void Doc_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch(e.PropertyName)
            {
                case "RenderObjects":
                    renderView.Content.Render();
                    break;
                case "Mesh":
                    propertyView.Content.SelectedObject = Doc.CharacterMetaDoc.Instance.Mesh;
                    break;
            }
        }

        private void CharacterMetaEditor_GotFocus(object sender, EventArgs e)
        {
            renderView.Content.Render();
        }

        public void InitializeDocViews()
        {
            renderView = new DockView<CharacterMetaRenderView>();
            colliderView = new DockView<ColliderPropertyView>();
            animationTableView = new DockView<AnimationTableView>();
            propertyView = new DockView<PropertyGrid>();
            renderView.TabText = "View";
            renderView.Show(dockPanel1, DockState.Document);
            renderView.CloseButtonVisible = false;
            renderView.CloseButton = false;
            renderView.IsFloat = false;

            colliderView.TabText = "Colliders";
            colliderView.Show(dockPanel1, DockState.DockLeft);

            animationTableView.TabText = "Animations";
            animationTableView.Show(dockPanel1, DockState.DockLeft);

            propertyView.TabText = "X Mesh 정보";
            propertyView.Show(dockPanel1, DockState.DockRight);

        }
    }
}
