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
        public Form1()
        {
            InitializeComponent();
            MapToolCore.Environment.Instance.ProjectDirectory =System.IO.Path.GetFullPath(".");

            m_renderView = new DockView<View.RenderView>();
            GraphicsDevice.Initialize(m_renderView.Content, 1920, 1080);
            SkyBox skyBox = new SkyBox(GraphicsDevice.Instance);
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

        private void wowMapMashToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var fileDialog = new OpenFileDialog();
            fileDialog.Filter = "obj 파일(*.obj)|*.obj";
            if (fileDialog.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            var path = fileDialog.FileName;
            var mapMesh = new WowMapMesh(GraphicsDevice.Instance, path);
            mapMesh.Name = "map mesh";
            renderObjects.Add(mapMesh);
            Doc.Document.Instance.AddObject(mapMesh);
            m_renderView.Content.Render();
        }
    }
}
