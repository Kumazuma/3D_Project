using MapToolRender;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MapTool.View
{
    public partial class AnimationView : UserControl
    {
        MapToolRender.SkinnedXMeshObj animMeshObj;
        MapToolRender.Camera camera;
        Timer m_timer;
        Stopwatch stopWatch = new Stopwatch();
        TimeSpan lastTimeSpan;
        public AnimationView()
        {
            InitializeComponent();
            camera = new MapToolRender.Camera();
            var position = camera.Position;
            var rotation = camera.Rotation;
            camera.Position = position;
            camera.Rotation = rotation;
            m_timer = new Timer();
            m_timer.Interval = 16;
            m_timer.Tick += OnTimerTick;
            stopWatch.Start();
            lastTimeSpan = stopWatch.Elapsed;
            m_timer.Start();
        }
        private void OnTimerTick(object sender, EventArgs e)
        {
            var timeSpan = stopWatch.Elapsed;
            var delta = timeSpan - lastTimeSpan;
            lastTimeSpan = timeSpan;
            if (animMeshObj != null)
            {
                animMeshObj.Update((int)(delta.TotalSeconds * 1000));
                MapToolRender.GraphicsDevice.Instance.Render(drawPanel, animMeshObj, camera);
            }
        }
        private void btnJSONSave_Click(object sender, EventArgs e)
        {

        }

        private void btnJsonOpen_Click(object sender, EventArgs e)
        {

        }

        private void btnXFileOpen_Click(object sender, EventArgs e)
        {

        }

        private void btnXFileOpen_Click_1(object sender, EventArgs e)
        {
            var openFileDialog = new OpenFileDialog
            {
                Filter = "X파일(*.x)|*.x"
            };
            if(openFileDialog.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            animMeshObj = new MapToolRender.SkinnedXMeshObj(MapToolRender.GraphicsDevice.Instance, openFileDialog.FileName);
            animMeshObj.PropertyChanged += AnimMeshObj_PropertyChanged;
            Doc.Document.Instance.SelectedObject = animMeshObj;
            var animCount = animMeshObj.AnimationCount;
            comboAnim.Items.Clear();
            AnimIndex.Items.Clear();
            for (var i = 0u; i < animCount;++i)
            {
                var idx = $"{i}";
                comboAnim.Items.Add(idx);
                AnimIndex.Items.Add(idx);
            }
        }

        private void AnimMeshObj_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (animMeshObj != null)
            {
                MapToolRender.GraphicsDevice.Instance.Render(drawPanel, animMeshObj, camera);
            }
        }

        private void btnJsonOpen_Click_1(object sender, EventArgs e)
        {

        }

        private void btnJSONSave_Click_1(object sender, EventArgs e)
        {
            if (animMeshObj == null)
            {
                return;
            }

        }

        private void comboAnim_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(animMeshObj != null)
            {
                animMeshObj.PlayAnimIdx((uint)comboAnim.SelectedIndex);
            }
        }

        private void drawPanel_Paint(object sender, PaintEventArgs e)
        {
            if(animMeshObj != null)
            {
                MapToolRender.GraphicsDevice.Instance.Render(drawPanel, animMeshObj, camera);
            }
        }

        private void AnimationView_Enter(object sender, EventArgs e)
        {
            if (animMeshObj != null)
            {
                Doc.Document.Instance.SelectedObject = animMeshObj;
            }
        }
    }
}
