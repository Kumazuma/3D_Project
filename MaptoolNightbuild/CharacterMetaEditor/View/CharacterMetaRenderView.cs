﻿using System;
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
        public CharacterMetaRenderView()
        {
            InitializeComponent();
            renderView = new RenderView();
            Controls.Add(renderView);
            renderView.Dock = DockStyle.Fill;
            var document = Document.Instance;
            document.PropertyChanged += Document_PropertyChanged;
            document.RenderObjects.CollectionChanged += RenderObjects_CollectionChanged;
            timer.Tick += Timer_Tick;
            timer.Interval = 1;
        }

        private void Document_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            var document = Document.Instance;
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
            var document = Document.Instance;
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
            var loadingDialog = new XMeshLoadingDialog();
            loadingDialog.Show();
            this.ParentForm.Enabled = false;
            await Controller.Instance.LoadXMesh(openFileDialog.FileName);
            this.ParentForm.Enabled = true;
            loadingDialog.Close();
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

        private async void btnOpenJson_Click(object sender, EventArgs e)
        {
            var openFileDialog = new OpenFileDialog
            {
                Filter = "JSON파일(*.json)|*.json"
            };
            if (openFileDialog.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            var loadingDialog = new XMeshLoadingDialog();
            loadingDialog.Show();
            this.ParentForm.Enabled = false;
            await Controller.Instance.Load(openFileDialog.FileName);
            this.ParentForm.Enabled = true;
            loadingDialog.Close();
        }

        private void btnSaveJSON_Click(object sender, EventArgs e)
        {
            var saveFileDialog = new SaveFileDialog
            {
                Filter = "JSON파일(*.json)|*.json"
            };
            if (saveFileDialog.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            var loadingDialog = new XMeshLoadingDialog();
            loadingDialog.Show();
            this.ParentForm.Enabled = false;
            Controller.Instance.Save(saveFileDialog.FileName);
            this.ParentForm.Enabled = true;
            loadingDialog.Close();
        }
    }
}
