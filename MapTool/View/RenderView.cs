﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
namespace MapTool.View
{
    public partial class RenderView : UserControl
    {
        enum MouseOperation
        {
            None,
            MovingUpRight,
            Rotating,
            MovingForward
        }
        MaptoolRenderer.Camera camera;
        IEnumerable<MaptoolRenderer.IRenderable> mapObjects;
        MouseOperation m_mouseOperation = MouseOperation.None;
        Point? m_prevMousePosition = null;
        Action renderAction = null;
        public RenderView()
        {
            InitializeComponent();
            camera = new MaptoolRenderer.PersCamera();
            this.Paint += RenderView_Paint;
            this.MouseWheel += RenderView_MouseWheel;
        }

        private void RenderView_MouseWheel(object sender, MouseEventArgs e)
        {
            var wheelCount = e.Delta /10f;
            camera.MoveForward(wheelCount);
            Render();

        }

        private void RenderView_Paint(object sender, PaintEventArgs e)
        {
            Render();
        }

        public void Initialize(uint backBufferWidth, uint backBufferHeight)
        {
            
        }
        public void Render()
        {
            if (mapObjects == null) return;
            if (!this.Created) return;
            if (renderAction != null) return;
            renderAction = new Action(() =>
            {
                if (renderAction == null) return;
                MaptoolRenderer.GraphicsDevice.Instance.Render(this, mapObjects, camera);
                renderAction = null;
            });
            this.BeginInvoke(renderAction);


        }
        public MaptoolRenderer.Camera CurrentCamera
        {
            get => camera;
        }
        public IEnumerable<MaptoolRenderer.IRenderable> RenderObjects
        {
            get => mapObjects;
            set
            {
                mapObjects = value;
                Render();
            }
        }

        private void RenderView_MouseDown(object sender, MouseEventArgs e)
        {
            //Ctrl이 눌린 상태면 위아래 좌우로 이동하는 행동을 하면 된다.
            if (ModifierKeys != 0) return;
            if (m_mouseOperation != MouseOperation.None) return;
            switch (e.Button)
            {
                case MouseButtons.Left:
                    m_mouseOperation = MouseOperation.Rotating;
                    break;
                case MouseButtons.Right:
                    m_mouseOperation = MouseOperation.MovingUpRight;
                    break;
            }
            m_prevMousePosition = e.Location;
        }

        private void RenderView_MouseLeave(object sender, EventArgs e)
        {
            m_mouseOperation = MouseOperation.None;
            m_prevMousePosition = null;
        }

        private void RenderView_MouseUp(object sender, MouseEventArgs e)
        {
            m_mouseOperation = MouseOperation.None;
            m_prevMousePosition = null;
        }

        private void RenderView_Move(object sender, EventArgs e)
        {
            
        }

        private void RenderView_MouseMove(object sender, MouseEventArgs e)
        {
            Point nowMousePoint = e.Location;
            if (m_prevMousePosition == null)
            {
                //m_prevMousePosition = nowMousePoint;
                return;
            }
            float deltaX = nowMousePoint.X - m_prevMousePosition.Value.X;
            float deltaY = nowMousePoint.Y - m_prevMousePosition.Value.Y;
            var newPrevPos = m_prevMousePosition.Value;
            switch (m_mouseOperation)
            {
                case MouseOperation.MovingUpRight:
                    if (Math.Abs(deltaX) >= 1.0f)
                    {
                        deltaX /= (float)Size.Width * 0.01f;
                        newPrevPos.X = nowMousePoint.X;
                        camera.MoveRight(-deltaX);
                    }
                    if (Math.Abs(deltaY) >= 1.0f)
                    {
                        deltaY /= (float)Size.Height * 0.01f;
                        newPrevPos.Y = nowMousePoint.Y;
                        camera.MoveUp(deltaY);
                    }
                    break;
                case MouseOperation.Rotating:
                    if (Math.Abs(deltaX) >= 1.0f)
                    {
                        deltaX /= (float)Size.Width * 0.01f;
                        newPrevPos.X = nowMousePoint.X;
                        camera.RotationY(-deltaX);
                    }
                    if (Math.Abs(deltaY) >= 1.0f)
                    {
                        deltaY /= (float)Size.Height * 0.01f;
                        newPrevPos.Y = nowMousePoint.Y;
                        camera.RotationX(-deltaY);
                    }
                    break;
                case MouseOperation.MovingForward:
                    newPrevPos.X = nowMousePoint.X; 
                    if (Math.Abs(deltaY) >= 1.0f)
                    {
                        deltaY /= (float)Size.Height * 0.01f;
                        newPrevPos.Y = nowMousePoint.Y;
                        camera.MoveForward(deltaY);
                    }
                    break;
                default:
                    break;
            }
            m_prevMousePosition = newPrevPos;
            Render();
        }

        private void RenderView_SizeChanged(object sender, EventArgs e)
        {
            Render();
        }
    }
}
