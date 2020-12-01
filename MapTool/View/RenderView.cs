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
        enum MouseOperation
        {
            None,
            MovingUpRight,
            Rotating,
            MovingForward
        }
        MapToolRender.Camera camera;
        IEnumerable<MapToolRender.RenderObject> mapObjects;
        MouseOperation m_mouseOperation = MouseOperation.None;
        Point? m_prevMousePosition = null;
        public RenderView()
        {
            InitializeComponent();
            camera = new MapToolRender.Camera();
        }
        public void Render()
        {
            if (mapObjects == null) return;
            MapToolRender.GraphicsDevice.Instance.Render(this, mapObjects, camera);
        }
        public IEnumerable<MapToolRender.RenderObject> RenderObjects
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
            if( (ModifierKeys & Keys.Control) != 0)
            {
                m_mouseOperation = MouseOperation.MovingUpRight;
            }
            else if((ModifierKeys & Keys.Alt) != 0)
            {
                m_mouseOperation = MouseOperation.MovingForward;
            }
            else
            {
                m_mouseOperation = MouseOperation.Rotating;
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
    }
}
