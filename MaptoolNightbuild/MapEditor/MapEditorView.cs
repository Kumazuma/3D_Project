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
        BindingList<MapToolCore.IHasText> worldObjectsBindList = new BindingList<MapToolCore.IHasText>();

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
            listbox.Content.DataSource = worldObjectsBindList;
            listbox.Content.DisplayMember = "Text";
            listbox.Content.SelectionMode = SelectionMode.MultiExtended;
            worldObjectsBindList.Add(renderView.Content.CurrentCamera);
            worldObjectsBindList.ResetBindings();
            
            Document.Instance.RenderObjects.CollectionChanged += RenderObjects_CollectionChanged;
            Document.Instance.SkyBox = new MaptoolRenderer.SkyBox();
            Document.Instance.SkyBox.PropertyChanged += Object_PropertyChanged;
            Controller.Instance.NewMap();
            this.KeyDown += MainFrame_KeyDown;
            renderView.Content.KeyDown += MainFrame_KeyDown;
        }

        private void Object_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if(e.PropertyName != "Name")
            {
                renderView.Content.Render();
            }
            else
            {
                int i = worldObjectsBindList.IndexOf(sender as MapToolCore.IHasText);
                if(i != -1)
                {
                    worldObjectsBindList.ResetBindings();
                }
            }
        }

        private void MainFrame_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.KeyCode == Keys.W)
            {
                renderView.Content.CurrentCamera.MoveForward(1);
            }
            else if(e.KeyCode == Keys.S)
            {
                renderView.Content.CurrentCamera.MoveForward(-1);
            }
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
                            worldObjectsBindList.Add(obj as MapToolCore.IHasText);
                            (obj as INotifyPropertyChanged).PropertyChanged += Object_PropertyChanged;
                        }
                        worldObjectsBindList.ResetBindings();
                        break;
                    case NotifyCollectionChangedAction.Remove:
                        foreach (var obj in e.OldItems)
                        {
                            worldObjectsBindList.Remove(obj as MapToolCore.IHasText);
                        }
                        worldObjectsBindList.ResetBindings();
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
            var listBox = listbox.Content;
            if (listBox.SelectedItems.Count == 1)
            {
                propertyView.Content.SelectedObject = listbox.Content.SelectedItem;
            }
            else if(listBox.SelectedItems.Count > 1)
            {
                var objects = new List<MapToolCore.IHasTransform>();
                foreach(var item in listBox.SelectedItems)
                {
                    if(item is MapToolCore.IHasTransform)
                    {
                        objects.Add(item as MapToolCore.IHasTransform);
                    }
                }
                if(objects.Count != 0)
                {
                    propertyView.Content.SelectedObject = new MultiSelectTransforms(objects);
                }
            }
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

        private void miAddTarget_Click(object sender, EventArgs e)
        {
            Document.Instance.RenderObjects.Add(new MapTarget());
        }

        private async void miSaveFile_Click(object sender, EventArgs e)
        {
            using (var saveFileDialog = new SaveFileDialog{Filter = "JSON파일(*.json)|*.json"})
            {
                if (saveFileDialog.ShowDialog() != DialogResult.OK)
                {
                    return;
                }
                await Controller.Instance.SaveMap(saveFileDialog.FileName);
                MessageBox.Show("완료되었습니다");
            }
        }

        private async void miOpenFile_Click(object sender, EventArgs e)
        {
            using (var openFileDialog = new OpenFileDialog { Filter = "JSON파일(*.json)|*.json" })
            {
                if (openFileDialog.ShowDialog() != DialogResult.OK)
                {
                    return;
                }
                await Controller.Instance.OpenMap(openFileDialog.FileName);
                MessageBox.Show("완료되었습니다");
            }
        }
    }
}
