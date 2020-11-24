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

namespace MapTool
{
    public partial class Form1 : Form
    {
        DockView<View.MapObjectTreePanel> m_mapObjTreePanel;
        DockView<View.RenderView> m_renderView;
        DockView<PropertyGrid> m_propertyView;
        DockView<View.ProjectDirectoryPanel> m_projectDirectoryView;
        public Form1()
        {
            InitializeComponent();
            MapToolCore.Environment.Instance.ProjectDirectory = ".";

            m_renderView = new DockView<View.RenderView>();
            GraphicsDevice.Initialize(m_renderView.Content, 1920, 1080);
            SkyBox skyBox = new SkyBox(GraphicsDevice.Instance);
            GraphicsDevice.Instance.Add(RenderGroup.PRIORITY, skyBox);

            Doc.Document.Instance.World = skyBox;
            Doc.Document.Instance.PropertyChanged += Document_PropertyChanged;
            //GraphicsDevice.Instance.Add(RenderGroup.PRIORITY, terrain);
            //terrain.Name = "Terrain";
            //Doc.Document.Instance.AddObject(terrain);

            m_mapObjTreePanel = new DockView<View.MapObjectTreePanel>();
            m_propertyView = new DockView<PropertyGrid>();
            m_projectDirectoryView = new DockView<View.ProjectDirectoryPanel>();

            m_mapObjTreePanel.TabText = "트리";
            m_renderView.TabText = "렌더";
            m_projectDirectoryView.TabText = "디렉토리";
            m_propertyView.TabText = "속성";

            m_propertyView.Content.AllowDrop = true;
            m_propertyView.Content.DragEnter += PropertyView_DragEnter;
            m_propertyView.Content.DragDrop += PropertyView_DragDrop;

            m_mapObjTreePanel.Show(dockPanel1, DockState.DockLeft);
            m_renderView.Show(dockPanel1, DockState.Document);
            m_propertyView.Show(dockPanel1, DockState.DockRight);
            m_projectDirectoryView.Show(dockPanel1, DockState.DockBottom);

            m_renderView.CloseButtonVisible = false;
            m_renderView.CloseButton = false;
            m_renderView.IsFloat = false;
            
            GraphicsDevice.Instance.Render();
            m_renderView.Content.Paint += Form1_Paint;
            //StaticXMeshObj mesh = new StaticXMeshObj(GraphicsDevice.Instance, "./Tree01.X");
            //GraphicsDevice.Instance.Add(RenderGroup.NONALPHA, mesh);
            //mesh.Name = "mesh";
            //Doc.Document.Instance.AddObject(mesh);
            //TerrainObject terrain =
            //    new TerrainObject(
            //        GraphicsDevice.Instance,
            //        "./Height1.bmp",
            //        10.0f,
            //        200.0f
            //    );
            //Texture texture = new Texture(GraphicsDevice.Instance,  "./Grass_1.png");
            //terrain.DiffuseTexture = texture;
            //m_propertyView.Content.SelectedObject = terrain;
            //GraphicsDevice.Instance.Add(RenderGroup.PRIORITY, terrain);

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
            }
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            GraphicsDevice.Instance.Render();
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
                    GraphicsDevice.Instance.Add(RenderGroup.NONALPHA, terrain);
                    terrain.Name = "Terrain";
                    Doc.Document.Instance.AddObject(terrain);
                    //m_mapObjTreePanel.Content.
                }
            }
        }

        private void StaticXMeshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //TODO:
        }
    }
}
