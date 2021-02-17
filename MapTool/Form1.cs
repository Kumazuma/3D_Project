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
using MaptoolRenderer;
using System.Drawing.Design;
using System.Diagnostics;

namespace MapTool
{
    public partial class Form1 : Form
    {
        DockView<View.MapObjectTreePanel> m_mapObjTreePanel;
        DockView<View.RenderView> m_renderView;
        DockView<PropertyGrid> m_propertyView;
        
        DockView<View.ProjectDirectoryPanel> m_projectDirectoryView;
        DockView<View.AnimationView> m_animationJsonEditView;
        DockView<View.ColiiderEditView> m_colliderEditView;
        HashSet<MaptoolRenderer.IRenderable> renderObjects = new HashSet<MaptoolRenderer.IRenderable>();
        Doc.MapFile file;
        //SkyBox skyBox;
        public Form1()
        {
            InitializeComponent();

            MapToolCore.Environment.Instance.ProjectDirectory =System.IO.Path.GetFullPath(Properties.Settings.Default.ProjectDir);
            MapToolCore.Environment.Instance.PropertyChanged += Env_PropertyChanged;
            m_renderView = new DockView<View.RenderView>();
            MaptoolRenderer.GraphicsDevice.Initalize(m_renderView.Content, 1920, 1080);
            m_renderView.Content.Initialize(800, 600);

         
            Doc.Document.Instance.PropertyChanged += Document_PropertyChanged;

            m_mapObjTreePanel = new DockView<View.MapObjectTreePanel>();
            m_propertyView = new DockView<PropertyGrid>();
            m_projectDirectoryView = new DockView<View.ProjectDirectoryPanel>();
            m_animationJsonEditView = new DockView<View.AnimationView>();
            m_colliderEditView = new DockView<View.ColiiderEditView>();
            m_mapObjTreePanel.TabText = "트리";
            m_renderView.TabText = "렌더";
            m_projectDirectoryView.TabText = "디렉토리";
            m_propertyView.TabText = "속성";
            m_animationJsonEditView.TabText = "애니메이션";
            m_colliderEditView.TabText = "콜라이더";
            m_propertyView.Content.AllowDrop = true;
            m_propertyView.Content.DragEnter += PropertyView_DragEnter;
            m_propertyView.Content.DragDrop += PropertyView_DragDrop;

            m_mapObjTreePanel.Show(dockPanel1, DockState.DockLeft);
            m_renderView.Show(dockPanel1, DockState.Document);
            m_propertyView.Show(dockPanel1, DockState.DockRight);
            m_projectDirectoryView.Show(dockPanel1, DockState.DockBottom);
            m_animationJsonEditView.Show(dockPanel1, DockState.Document);
            m_colliderEditView.Show(dockPanel1, DockState.Document);
            m_renderView.Activate();
            m_renderView.CloseButtonVisible = false;
            m_renderView.CloseButton = false;
            m_renderView.IsFloat = false;
            m_animationJsonEditView.CloseButtonVisible = false;
            m_animationJsonEditView.CloseButton = false;
            m_animationJsonEditView.IsFloat = false;
            m_colliderEditView.CloseButtonVisible = false;
            m_colliderEditView.CloseButton= false;
            m_colliderEditView.IsFloat = false;

            //GraphicsDevice.Instance.Render();
            m_renderView.Content.RenderObjects = renderObjects;
            m_renderView.Content.MouseClick += RederView_MouseClick;

        }

        private void Env_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            Properties.Settings.Default.ProjectDir = MapToolCore.Environment.Instance.ProjectDirectory;
        }

        private void RederView_MouseClick(object sender, MouseEventArgs e)
        {

        }

        private void PropertyView_DragDrop(object sender, DragEventArgs e)
        {
            Control child = m_propertyView.Content.GetChildAtPoint(new Point(e.X, e.Y));
            //m_propertyView.Content.
        }

        private void PropertyView_DragEnter(object sender, DragEventArgs e)
        {
            // 데이타가 문자열 타입이면 복사하고, 아니면 Drop 무효처리
            if (e.Data.GetDataPresent(typeof(string)))
            {
                // Drop하여 복사함
                e.Effect = DragDropEffects.Link;
            }
            else
            {
                // Drop 할 수 없음
                e.Effect = DragDropEffects.None;
            }
        }

        private void Document_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (m_propertyView == null) return;
            switch(e.PropertyName)
            {
                case "SelectedObject":
                    m_propertyView.Content.SelectedObject = Doc.Document.Instance.SelectedObject;
                    break;
                case "MapObjects":
                    if(!Doc.Document.Instance.MapObjects.Contains(sender as MapObject))
                    {
                        renderObjects.Remove(sender as RenderObject);
                        m_renderView.Content.Render();
                    }
                    break;
            }
        }


        public class DockView<T>:  WeifenLuo.WinFormsUI.Docking.DockContent where T : Control, new ()
        {
            private T m_panel;
            public T Content { get => m_panel; }
            public DockView()
            {
                m_panel = new T();
                m_panel.Dock = DockStyle.Fill;
                m_panel.Parent = this;
                m_panel.Visible = true;
            }
        }

        private void TerrainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //using (var dialog = new View.TerrainCreateDialog())
            //{
            //    if(dialog.ShowDialog() == DialogResult.OK)
            //    {
            //        TerrainObject terrain =
            //            new TerrainObject(
            //                GraphicsDevice.Instance,
            //                dialog.HeightmapImgPath,
            //                dialog.Interval,
            //                dialog.MaxHeight
            //            );
            //        renderObjects.Add(terrain);
            //        terrain.Name = "Terrain";
            //        Doc.Document.Instance.AddObject(terrain);
            //        terrain.PropertyChanged += RenderObj_PropertyChanged;
            //        m_renderView.Content.Render();
            //        //m_mapObjTreePanel.Content.
            //    }
            //}
        }

        private void StaticXMeshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var fileDialog = new OpenFileDialog();
            fileDialog.Filter = "X 파일(*.x)|*.x";
            if (fileDialog.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            var path = fileDialog.FileName;

            var xmesh = Doc.ResourceManager.Instance.GetStaticMesh(path);
            renderObjects.Add(xmesh);
            xmesh.Name = "XMesh";
            xmesh.PropertyChanged += RenderObj_PropertyChanged;
            Doc.Document.Instance.AddObject(xmesh);
            m_renderView.Content.Render();
        }

        private void RenderObj_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            m_renderView.Content.Render();
        }

        private async void wowMapMashToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var fileDialog = new OpenFileDialog();
            fileDialog.Filter = "obj 파일(*.obj)|*.obj";
            fileDialog.Multiselect = true;
            if (fileDialog.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            var paths = fileDialog.FileNames;
            List<Task<OBJMesh> > taskList = new List<Task<OBJMesh>>();
            foreach(var path in paths)
            {
                var task = System.Threading.Tasks.Task<OBJMesh>.Factory.StartNew(() =>
                {
                    var mesh = Doc.ResourceManager.Instance.GetObjMesh(path).Result;
                    mesh.Name = System.IO.Path.GetFileNameWithoutExtension(path);
                    return mesh;
                });
                taskList.Add(task);
            }
            foreach(var task in taskList)
            {
                var mesh = await task;
                var meshObject = MeshObject.Create(mesh);
                renderObjects.Add(meshObject);
                meshObject.PropertyChanged += RenderObj_PropertyChanged;
                Doc.Document.Instance.AddObject(meshObject);
            }
            m_renderView.Content.Render();
        }

        private void naviMeshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var naviMesh = new NaviMesh(GraphicsDevice.Instance);
            renderObjects.Add(naviMesh);
            naviMesh.Name = "Navi Mesh";
            Doc.Document.Instance.AddObject(naviMesh);
            m_renderView.Content.Render();
        }

        private void projectDirectoryToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var folderDialog = new FolderBrowserDialog();
            folderDialog.SelectedPath = System.IO.Path.GetFullPath(MapToolCore.Environment.Instance.ProjectDirectory);
            if (folderDialog.ShowDialog() != DialogResult.OK) return;
            MapToolCore.Environment.Instance.ProjectDirectory = folderDialog.SelectedPath;
        }

        private void SaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(file != null)
            {
                var res = MessageBox.Show("기존 파일을 덮어씌우겠습니까?", "파일 저장", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if (res != DialogResult.Yes)
                {
                    file = null;
                }
            }
            if (file == null)
            {
                var dialog = new SaveFileDialog();
                dialog.Filter = "map file(*.json)|*.json;";
                if (dialog.ShowDialog() != DialogResult.OK)
                {
                    return;
                }
                file = new Doc.MapFile();
                file.FilePath = dialog.FileName;
            }
            if(skyBox.SkyBoxTexture != null)
            {
                file.SkyBoxTexturePath = skyBox.SkyBoxTexture.FilePath;
            }
            file.Nodes.Clear();
            foreach(var item in Doc.Document.Instance.MapObjects)
            {
                file.Nodes.Add(item);
            }
            file.Save();
        }

        private void OpenFIleToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var dialog = new OpenFileDialog();
            dialog.Filter = "map file(*.json)|*.json;";
            if (dialog.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            try
            {
                var projectDir = MapToolCore.Environment.Instance.ProjectDirectory;
                var mapFile = Doc.MapFile.Load(dialog.FileName);
                Doc.Document.Instance.SelectedObject = null;
                {
                    var t = new List<MapObject>();
                    t.AddRange(Doc.Document.Instance.MapObjects);
                    foreach(var obj in t)
                    {
                        Doc.Document.Instance.RemoveObject(obj);
                    }
                }
                renderObjects.Clear();
                renderObjects.Add(skyBox);
                skyBox.SkyBoxTexture = new CubeTexture(MapToolRender.GraphicsDevice.Instance, projectDir + mapFile.SkyBoxTexturePath);
                foreach(var obj in mapFile.Nodes)
                {
                    var renderObj = obj as RenderObject;
                    if (renderObj == null) continue;
                    renderObjects.Add(renderObj);
                    Doc.Document.Instance.MapObjects.Add(renderObj);
                }
                file = mapFile;
                m_mapObjTreePanel.Content.ReloadObjects();
                m_renderView.Content.Render();
            }
            catch(Exception ex)
            {
                MessageBox.Show($"파일을 열지 못했습니다.\n{ex.Message}");
                Console.WriteLine(ex.StackTrace);
            }
        }

        private void targetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var target = Doc.ResourceManager.Instance.GetTargetObject();
            renderObjects.Add(target);
            target.PropertyChanged += RenderObj_PropertyChanged;
            Doc.Document.Instance.AddObject(target);
            m_propertyView.Content.SelectedObject = target;
            m_renderView.Content.Render();
        }
    }
}
