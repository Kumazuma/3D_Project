using System;
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
        bool m_cameraMoving = false;
        Point? m_prevMousePosition = null;
        public RenderView()
        {
            InitializeComponent();
        }

        private void RenderView_MouseDown(object sender, MouseEventArgs e)
        {
            m_cameraMoving = true;
            m_prevMousePosition = e.Location;
        }

        private void RenderView_MouseLeave(object sender, EventArgs e)
        {
            m_cameraMoving = false;
            m_prevMousePosition = null;

        }

        private void RenderView_MouseUp(object sender, MouseEventArgs e)
        {
            m_cameraMoving = false;
            m_prevMousePosition = null;

        }

        private void RenderView_Move(object sender, EventArgs e)
        {
            
            
        }

        private void RenderView_MouseMove(object sender, MouseEventArgs e)
        {
            if (!m_cameraMoving)
                return;
            Point nowMousePoint = e.Location;
            if(m_prevMousePosition == null)
            {
                m_prevMousePosition = nowMousePoint;
                return;
            }
            float deltaX = nowMousePoint.X - m_prevMousePosition.Value.X;
            float deltaY = nowMousePoint.Y - m_prevMousePosition.Value.Y;
            var newPrevPos = m_prevMousePosition.Value;
            if(Math.Abs(deltaX) >= 1.0f)
            {
                deltaX /= (float)Size.Width * 0.001f;
                
                newPrevPos.X = nowMousePoint.X ;
                MapToolRender.GraphicsDevice.Instance.CurrentCamera.MoveRight(-deltaX);
            }
            if (Math.Abs(deltaY) >= 1.0f)
            {
                deltaY /= (float)Size.Height * 0.001f;
                newPrevPos.Y = nowMousePoint.Y;
                MapToolRender.GraphicsDevice.Instance.CurrentCamera.MoveForward(deltaY);
            }
            m_prevMousePosition = newPrevPos;

            MapToolRender.GraphicsDevice.Instance.Render();
        }
    }
}
