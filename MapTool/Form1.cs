﻿using System;
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
namespace MapTool
{
    public partial class Form1 : Form
    {
        DockView<View.MapObjectTreePanel> m_mapObjTreePanel;
        DockView<View.RenderView> m_renderView;
        DockView<PropertyGrid> m_propertyView;
        public Form1()
        {
            InitializeComponent();
            MapToolCore.Environment.Instance.ProjectDirectory = ".";
            m_mapObjTreePanel = new DockView<View.MapObjectTreePanel>();
            m_renderView = new DockView<View.RenderView>();
            m_propertyView = new DockView<PropertyGrid>();
            m_mapObjTreePanel.TabText = "트리";
            m_renderView.TabText = "렌더";

            m_propertyView.TabText = "속성";
            m_propertyView.TabText = "속성";
            m_mapObjTreePanel.Show(dockPanel1, DockState.DockLeft);
            m_renderView.Show(dockPanel1, DockState.Document);
            m_propertyView.Show(dockPanel1, DockState.DockRight);

            m_renderView.CloseButtonVisible = false;
            m_renderView.CloseButton = false;
            m_renderView.IsFloat = false;
            
            GraphicsDevice.Initialize(m_renderView.Content, 1920, 1080);
            GraphicsDevice.Instance.CurrentCamera.Position.Y = 700f;
            GraphicsDevice.Instance.Render();
            m_renderView.Content.Paint += Form1_Paint;
            
            
            TerrainObject terrain =
                new TerrainObject(
                    GraphicsDevice.Instance,
                    "./Height1.bmp",
                    10.0f,
                    200.0f
                );
            Texture texture = new Texture(GraphicsDevice.Instance,  "./Grass_1.png");
            terrain.DiffuseTexture = texture;
            m_propertyView.Content.SelectedObject = terrain;
            GraphicsDevice.Instance.Add(RenderGroup.PRIORITY, terrain);
            
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

    }
}
