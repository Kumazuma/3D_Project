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
using MaptoolNightbuild.View;
namespace MaptoolNightbuild.CharacterMetaEditor.View
{
    public partial class CharacterMetaRenderView : UserControl
    {
        RenderView renderView;
        public CharacterMetaRenderView()
        {
            InitializeComponent();
            renderView = new RenderView();
            Controls.Add(renderView);
            renderView.Dock = DockStyle.Fill;
            var document = Doc.CharacterMetaDoc.Instance;
            document.RenderObjects.CollectionChanged += RenderObjects_CollectionChanged;
        }

        private void RenderObjects_CollectionChanged(
            object sender, NotifyCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Add)
            {
                (e.NewItems[0] as INotifyPropertyChanged).PropertyChanged += Object_PropertyChanged;
            }

        }

        private void Object_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            renderView.Render();
        }

        public void Play()
        {

        }
        public void Stop()
        {

        }
        public MaptoolRenderer.Camera CurrentCamera
        {
            get => renderView.CurrentCamera;
        }
        public IEnumerable<MaptoolRenderer.IRenderable> RenderObjects
        {
            get => renderView.RenderObjects;
            set=> renderView.RenderObjects = value;
        }
        public MaptoolRenderer.GraphicsDevice GraphicsDevice
        {
            get => renderView.GraphicsDevice;
            set => renderView.GraphicsDevice= value;
        }
        public void Render()
        {
            renderView.Render();
        }

        private async void btnLoadXMesh_Click(object sender, EventArgs e)
        {
            var openFileDialog = new OpenFileDialog
            {
                Filter = "X파일(*.x)|*.x"
            };
            if (openFileDialog.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            await LoadXMesh(openFileDialog.FileName);
        }
        async Task LoadXMesh(String path)
        {
            var preTime = DateTime.Now;
            var xmesh = await Doc.ResourceManager.Instance.GetSkinnedMesh(path);
            var xSkinnedMeshObject = new MaptoolRenderer.SkinnedXMeshObject(xmesh);
            var prevXMesh = Doc.CharacterMetaDoc.Instance.Mesh;
            Doc.CharacterMetaDoc.Instance.Mesh = xSkinnedMeshObject;
            Doc.CharacterMetaDoc.Instance.RenderObjects.Remove(prevXMesh);
            Doc.CharacterMetaDoc.Instance.RenderObjects.Add(xSkinnedMeshObject);
            xSkinnedMeshObject.PropertyChanged += XSkinnedMeshObject_PropertyChanged;
        }

        private void XSkinnedMeshObject_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            renderView.Render();
        }
    }
}
