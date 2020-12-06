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
using MapToolRender;
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
        HashSet<RenderObject> renderObjects = new HashSet<RenderObject>();
        Doc.MapFile file;
        SkyBox skyBox;
        public Form1()
        {
            InitializeComponent();

            MapToolCore.Environment.Instance.ProjectDirectory =System.IO.Path.GetFullPath(".");

            m_renderView = new DockView<View.RenderView>();
            GraphicsDevice.Initialize(m_renderView.Content, 800, 600);
            m_renderView.Content.Initialize(800, 600);

            skyBox = new SkyBox(GraphicsDevice.Instance);
            renderObjects.Add(skyBox);

            Doc.Document.Instance.World = skyBox;
            Doc.Document.Instance.PropertyChanged += Document_PropertyChanged;
            //GraphicsDevice.Instance.Add(RenderGroup.PRIORITY, terrain);
            //terrain.Name = "Terrain";
            //Doc.Document.Instance.AddObject(terrain);

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

        private void RederView_MouseClick(object sender, MouseEventArgs e)
        {
            if (ModifierKeys != 0)
                return;
            var point = new Point(e.X, e.Y);
            var contextMesh = new ContextMenuStrip();

            contextMesh.Show(m_renderView.Content, point);
            if (m_propertyView.Content.SelectedObject == null)
            {
                return;
            }
            if(m_propertyView.Content.SelectedObject.GetType() != typeof(NaviMesh))
            {
                return;
            }
            var selectedObject = m_propertyView.Content.SelectedObject as NaviMesh;
            if (!renderObjects.Contains(selectedObject))
            {
                return;
            }
            
            var ray = GraphicsDevice.Instance.CreateMouseRay(m_renderView.Content, m_renderView.Content.CurrentCamera, point);
            float t = float.MaxValue;
            foreach(var renderObj in renderObjects)
            {
                if (!renderObj.IsRayPick) continue;
                float? res = renderObj.RayPick(ray);
                if (res == null) continue;
                if(res.Value < t)
                {
                    t = res.Value;
                }
            }
            if(t != float.MaxValue)
            {
                var p = ray.GetPosition(t);
                selectedObject.PushPoint(p.X, p.Y, p.Z, m_renderView.Content.CurrentCamera);
                m_renderView.Content.Render();
            }
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
            using (var dialog = new View.TerrainCreateDialog())
            {
                if(dialog.ShowDialog() == DialogResult.OK)
                {
                    TerrainObject terrain =
                        new TerrainObject(
                            GraphicsDevice.Instance,
                            dialog.HeightmapImgPath,
                            dialog.Interval,
                            dialog.MaxHeight
                        );
                    renderObjects.Add(terrain);
                    terrain.Name = "Terrain";
                    Doc.Document.Instance.AddObject(terrain);
                    m_renderView.Content.Render();
                    //m_mapObjTreePanel.Content.
                }
            }
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

            var xmesh = Doc.MeshManager.Instance.GetStaticMesh(path);
            renderObjects.Add(xmesh);
            xmesh.Name = "XMesh";
            Doc.Document.Instance.AddObject(xmesh);
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
            var task = new System.Threading.Tasks.Task<WowMapMesh[]>(() =>
            {
                List<WowMapMesh> res = new List<WowMapMesh>(paths.Length);
                foreach (var path in paths)
                {
                    var mapMesh = new WowMapMesh(GraphicsDevice.Instance, path);
                    mapMesh.Name = System.IO.Path.GetFileNameWithoutExtension(path);
                    res.Add(mapMesh);
                    
                }
                return res.ToArray();
            });
            task.Start();
            var mapMeshs= await task;
            foreach(var mapMesh in mapMeshs)
            {
                renderObjects.Add(mapMesh);
                Doc.Document.Instance.AddObject(mapMesh);
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
            //if (m_currentJsonPath != null)
            //{
            //    var res = MessageBox.Show("기존 파일을 덮어씌우겠습니까?", "파일 저장", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
            //    if (res != DialogResult.Yes)
            //        m_currentJsonPath = null;
            //}
            //if (m_currentJsonPath == null)
            //{
            //    var dialog = new SaveFileDialog();
            //    dialog.Filter = "anim meta data(*.json)|*.json;";
            //    if (dialog.ShowDialog() != DialogResult.OK)
            //    {
            //        return;
            //    }
            //    m_currentJsonPath = dialog.FileName;
            //}

            //System.IO.Stream fileStream = null;
            //try
            //{
            //    fileStream = System.IO.File.OpenWrite(m_currentJsonPath);
            //}
            //catch (Exception ex)
            //{
            //    Console.WriteLine(ex);
            //    MessageBox.Show("파일을 열지 못 했습니다.");
            //    m_currentJsonPath = null;
            //    return;
            //}
            //var table = new Dictionary<string, int>();
            //var set = new HashSet<int>();
            //foreach (DataGridViewRow row in dataGridView1.Rows)
            //{
            //    if (row.Cells[0].Value == null)
            //    {
            //        continue;
            //    }
            //    var number = int.Parse(row.Cells[0].Value as string);
            //    var idx = row.Cells[1].Value as string;
            //    if (set.Contains(number))
            //    {
            //        continue;
            //    }
            //    if (table.ContainsKey(idx))
            //    {
            //        continue;
            //    }
            //    set.Add(number);
            //    table.Add(idx, number);
            //}

            //var streamWriter = new System.IO.StreamWriter(fileStream);
            //var writer = new Newtonsoft.Json.JsonTextWriter(streamWriter);
            //writer.WriteStartObject();
            //writer.WritePropertyName("x_file_path");
            //var relativePath = MapToolCore.Utility.GetRelativePath(MapToolCore.Environment.Instance.ProjectDirectory, animMeshObj.MeshFilePath);
            //writer.WriteValue(relativePath);
            //writer.WritePropertyName("animations");
            //writer.WriteStartObject();
            //foreach (var item in table)
            //{
            //    writer.WritePropertyName(item.Key);
            //    writer.WriteValue(item.Value);
            //}
            //writer.WriteEndObject();
            //writer.WriteEndObject();
            //writer.Close();
        }
    }
}
