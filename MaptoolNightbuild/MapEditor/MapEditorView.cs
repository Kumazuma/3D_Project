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
using MaptoolNightbuild.View;
using System.Collections.Specialized;
namespace MaptoolNightbuild.MapEditor
{
    public partial class MainFrame : UserControl
    {
        View.DockView<RenderView> renderView;
        View.DockView<ListBox> listbox;
        View.DockView<PropertyGrid> propertyView;

        public MainFrame()
        {
            InitializeComponent();
            InitializeDocViews();
            if(MaptoolRenderer.GraphicsDevice.Instance != null)
            {
                renderView.Content.GraphicsDevice = MaptoolRenderer.GraphicsDevice.Instance;
            }

            renderView.Content.RenderObjects = Document.Instance.RenderObjects;
            (renderView.Content.CurrentCamera as MaptoolRenderer.PersCamera).Angle = 45.0f;
            renderView.Content.CurrentCamera.Far = 1000.0f;
            renderView.Content.CurrentCamera.Near = 0.1f;
            renderView.Content.CurrentCamera.PropertyChanged += CurrentCamera_PropertyChanged;
            listbox.Content.Items.Add(renderView.Content.CurrentCamera);

            Document.Instance.RenderObjects.CollectionChanged += RenderObjects_CollectionChanged;
        }

        private void RenderObjects_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            renderView.Content.Render();
            var action = new Action(() =>
            {
                switch (e.Action)
                {
                    case NotifyCollectionChangedAction.Add:
                        foreach (var obj in e.NewItems)
                        {
                            listbox.Content.Items.Add(obj);
                        }
                        break;
                    case NotifyCollectionChangedAction.Remove:
                        foreach (var obj in e.OldItems)
                        {
                            listbox.Content.Items.Remove(obj);
                        }
                        break;
                }
            });
            if (this.InvokeRequired)
            {
                this.Invoke(action);
            }
            else
            {
                action();
            }
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
        
        private async void miAddOBJMesh_Click(object sender, EventArgs e)
        {
            var openFileDialog = new OpenFileDialog
            {
                Filter = "OBJ파일(*.obj)|*.obj"
            };
            openFileDialog.Multiselect = true;
            if (openFileDialog.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            var loadingDialog = new OBJMeshLoadingDialog();
            loadingDialog.Show(this.ParentForm);

            this.ParentForm.Enabled = false;
            await Controller.Instance.AddObjMesh(openFileDialog.FileNames);
            this.ParentForm.Enabled = true;
            loadingDialog.Close();

        }

        private async void imNewMap_Click(object sender, EventArgs e)
        {
            await Controller.Instance.NewMap();
        }
    }
}
