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
using System.Diagnostics;

namespace MaptoolNightbuild.CharacterMetaEditor.View
{
    public partial class CharacterMetaRenderView : UserControl
    {
        RenderView renderView;
        Timer timer = new Timer();
        Stopwatch stopWatch = new Stopwatch();
        TimeSpan lastTimeSpan;
        bool isPlaying = false;
        public CharacterMetaRenderView()
        {
            InitializeComponent();
            renderView = new RenderView();
            Controls.Add(renderView);
            renderView.Dock = DockStyle.Fill;
            var document = Doc.CharacterMetaDoc.Instance;
            document.PropertyChanged += Document_PropertyChanged;
            document.RenderObjects.CollectionChanged += RenderObjects_CollectionChanged;
            timer.Tick += Timer_Tick;
            timer.Interval = 1;
        }

        private void Document_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            var document = Doc.CharacterMetaDoc.Instance;
            if (e.PropertyName == "Mesh")
            {
                if(document.Mesh == null)
                {
                    timer.Stop();
                    stopWatch.Stop();
                }
                else
                {
                    //timer.Start();
                    //stopWatch.Start();
                }
            }
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            var document = Doc.CharacterMetaDoc.Instance;
            var timeSpan = stopWatch.Elapsed;
            var delta = timeSpan - lastTimeSpan;
            lastTimeSpan = timeSpan;

            if (document.Mesh != null)
            {
                document.Mesh.PlayAnimation((float)delta.TotalSeconds);
                renderView.Render();
            }
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
            isPlaying = true;
        }
        public void Stop()
        {
            isPlaying = false;
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

        private void btnPlay_Click(object sender, EventArgs e)
        {
            timer.Start();
            stopWatch.Start();
            lastTimeSpan = stopWatch.Elapsed;
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            timer.Stop();
            stopWatch.Stop();
        }
    }
}
