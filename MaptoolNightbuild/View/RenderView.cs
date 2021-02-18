using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MaptoolNightbuild.View
{
    enum MouseOperation
    {
        None,
        MovingUpRight,
        Rotating,
        MovingForward
    }

    public partial class RenderView : Control
    {
        MaptoolRenderer.GraphicsDevice graphicsDevice;
        MaptoolRenderer.Camera camera;
        IEnumerable<MaptoolRenderer.IRenderable> mapObjects;
        MouseOperation m_mouseOperation = MouseOperation.None;
        Point? m_prevMousePosition = null;
        IAsyncResult asyncResult = null;
        bool cameraMoveEnable = true;
        public RenderView()
        {
            InitializeComponent();
            camera = new MaptoolRenderer.PersCamera();
            camera.PropertyChanged += Camera_PropertyChanged;
            this.MouseWheel += RenderView_MouseWheel;
            this.SizeChanged += RenderView_SizeChanged;
        }

        private void Camera_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            Render();
        }

        public MaptoolRenderer.GraphicsDevice GraphicsDevice
        {
            get => graphicsDevice;
            set
            {
                graphicsDevice = value;
            }
        }
        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);
            if(cameraMoveEnable == true)
            {
                RenderView_MouseMove(this, e);
            }
        }
        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);
            if (cameraMoveEnable == true)
            {
                RenderView_MouseDown(this, e);
            }
        }
        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            RenderView_MouseUp(this, e);
        }
        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);
            Render();
        }
        private void RenderView_MouseWheel(object sender, MouseEventArgs e)
        {
            var wheelCount = e.Delta / 10f;
            camera.MoveForward(wheelCount);
            Render();

        }
        public void Render()
        {
            //렌더가 여러번 호출되더라도 이전에 진행중인 렌더 명령이 있다면 무시해버린다.
            if (graphicsDevice == null)
                return;
            if (mapObjects == null) return;
            if (!this.Created) return;
            if (asyncResult != null && !asyncResult.IsCompleted) return;
            var oldAsyncId = asyncResult;
            lock (this)
            {
                if(oldAsyncId == asyncResult)
                {
                    if(oldAsyncId !=null)
                    {
                        this.EndInvoke(oldAsyncId);
                    }
                    var renderAction = new Action(() =>
                    {
                        graphicsDevice.Render(this, mapObjects, camera);
                    });
                    asyncResult = this.BeginInvoke(renderAction);
                }
                else
                {
                    Console.WriteLine("건너 띄어짐!");
                }
            }
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
