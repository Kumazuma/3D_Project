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
        enum AnimationPlayState { Play, Stop};
        AnimationPlayState animationState = AnimationPlayState.Stop;
        Dictionary<MapToolCore.Collider, RenderObject> collderNRenderObject = new Dictionary<MapToolCore.Collider, RenderObject>();
        HashSet<RenderObject> objList = new HashSet<RenderObject>();
        MapToolRender.SkinnedXMeshObj animMeshObj;
        MapToolRender.Camera camera;
        Timer m_timer;
        Stopwatch stopWatch = new Stopwatch();
        TimeSpan lastTimeSpan;
        RenderView renderView;
        bool m_playing;
        string m_currentJsonPath = null;
        Doc.AnimationMeshMeta meta = new Doc.AnimationMeshMeta();
        public AnimationView()
        {
            InitializeComponent();
            camera = new MapToolRender.Camera();
            renderView = new RenderView();
            renderView.Initialize(800, 600);

            renderView.Parent = this.tableLayoutPanel2;
            renderView.RenderObjects = objList;
            renderView.Dock = DockStyle.Fill;
            this.tableLayoutPanel2.Controls.Add(this.renderView, 0, 1);
            m_timer = new Timer();
            m_timer.Interval = 16;
            m_timer.Tick += OnTimerTick;
            stopWatch.Start();
            lastTimeSpan = stopWatch.Elapsed;
            m_timer.Start();
            m_playing = true;
        }
        private void LoadXMesh(string xMeshPath)
        {
            objList.Remove(animMeshObj);
            animMeshObj = new MapToolRender.SkinnedXMeshObj(MapToolRender.GraphicsDevice.Instance, xMeshPath);
            animMeshObj.PropertyChanged += AnimMeshObj_PropertyChanged;
            meta.MeshFilePath = xMeshPath;
            Doc.Document.Instance.SelectedObject = animMeshObj;
            objList.Add(animMeshObj);
            var animCount = animMeshObj.AnimationCount;
            comboAnim.Items.Clear();
            AnimIndex.Items.Clear();
            for (var i = 0u; i < animCount; ++i)
            {
                var idx = $"{i}";
                comboAnim.Items.Add(idx);
                AnimIndex.Items.Add(idx);
            }
            foreach (var obj in listColliders.Items)
            {
                (obj as MapToolCore.Collider).FrameNames = animMeshObj.FrameNames;
            }
        }
        private void OnTimerTick(object sender, EventArgs e)
        {
            var timeSpan = stopWatch.Elapsed;
            var delta = timeSpan - lastTimeSpan;
            lastTimeSpan = timeSpan;
            
            if (animMeshObj != null && m_playing )
            {
                if(animationState == AnimationPlayState.Play)
                {
                    animMeshObj.Update((int)(delta.TotalSeconds * 1000));
                    foreach (var it in collderNRenderObject)
                    {
                        if (it.Key.FrameName == null) continue;
                        (it.Value as ColliderRenderObject).SetFrameMatrix(animMeshObj, it.Key.FrameName);
                    }
                }
                renderView.Render();
            }
            
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
            LoadXMesh(openFileDialog.FileName);
        }

        private void AnimMeshObj_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (animMeshObj != null && m_playing)
            {
                renderView.Render();
            }
        }

        private void btnJsonOpen_Click_1(object sender, EventArgs e)
        {
            var dialog = new OpenFileDialog();
            dialog.Filter = "anim meta data(*.json)|*.json;";
            if (dialog.ShowDialog() != DialogResult.OK)
            {
                return;
            }
            try
            {
                var newMetaFile = Doc.AnimationMeshMeta.Load(dialog.FileName);
                //TODO: 파일 데이터를 읽어, X파일을 로딩한다.
                if (newMetaFile.MeshFilePath == null || newMetaFile.MeshFilePath.Length == 0)
                    throw new Exception("Mesh File is empty");
                objList.Clear();
                collderNRenderObject.Clear();
                listColliders.Items.Clear();
                LoadXMesh(MapToolCore.Environment.Instance.ProjectDirectory + newMetaFile.MeshFilePath );
                
                foreach (var item in newMetaFile.AnimationTable)
                {
                    dataGridView1.Rows.Add(item.Index.ToString(), item.ID);
                }
                listColliders.Items.AddRange(newMetaFile.ColliderList.ToArray());
                
                foreach (var item in newMetaFile.ColliderList)
                {
                    ColliderRenderObject newObject = null;
                    if (animMeshObj != null)
                    {
                        item.FrameNames = animMeshObj.FrameNames;
                    }
                    item.PropertyChanged += Collider_PropertyChanged;
                    switch(item.Type)
                    {
                        case MapToolCore.ColliderType.Box:
                            newObject = new BoxColliderMeshObject(GraphicsDevice.Instance);
                            break;
                        case MapToolCore.ColliderType.Sphare:
                            newObject = new SphareMesh(GraphicsDevice.Instance);
                            break;
                    }
                    if (newObject != null)
                    {

                        newObject.SetAttribute(item.Attribute);
                        newObject.Offset = item.Offset;
                        newObject.Transform = item.Transform.Clone();
                        if (item.FrameName != null && animMeshObj != null)
                        {
                            newObject.SetFrameMatrix(animMeshObj, item.FrameName);
                        }
                        collderNRenderObject.Add(item, newObject);
                        objList.Add(newObject);
                    }
                }
                m_currentJsonPath = dialog.FileName;
                meta = newMetaFile;
            }
            catch (Exception ex)
            {
                MessageBox.Show($"파일을 열지 못했습니다.\n{ex.Message}");
                Console.WriteLine(ex.StackTrace);
            }
        }

        private void btnJSONSave_Click_1(object sender, EventArgs e)
        {
            if (animMeshObj == null)
            {
                return;
            }
            if(m_currentJsonPath != null)
            {
                var res = MessageBox.Show("기존 파일을 덮어씌우겠습니까?", "파일 저장", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if (res != DialogResult.Yes)
                    m_currentJsonPath = null;
            }
            if(m_currentJsonPath == null)
            {
                var dialog = new SaveFileDialog();
                dialog.Filter = "anim meta data(*.json)|*.json;";
                if (dialog.ShowDialog() != DialogResult.OK)
                {
                    return;
                }
                m_currentJsonPath = dialog.FileName;
            }

            meta.AnimationTable.Clear();
            meta.ColliderList.Clear();
            foreach (DataGridViewRow row in dataGridView1.Rows)
            {
                if(row.Cells[0].Value == null)
                {
                    continue;
                }
                var number = uint.Parse(row.Cells[0].Value as string);
                var idx = row.Cells[1].Value as string;

                meta.AnimationTable.Add(new Doc.AnimationTableItem(number, idx));
            }
            foreach(var obj in listColliders.Items)
            {
                var collider = obj as MapToolCore.Collider;
                if (collider == null) continue;
                meta.ColliderList.Add(collider);
            }
            try
            {
                meta.Save(m_currentJsonPath);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"파일 저장에 실패하였습니다.\n{ex.Message}");
                Console.WriteLine(ex.StackTrace);
            }
        }

        private void comboAnim_SelectedIndexChanged(object sender, EventArgs e)
        {
            if(animMeshObj != null)
            {
                animMeshObj.PlayAnimIdx((uint)comboAnim.SelectedIndex);
            }
        }
        private void AnimationView_Enter(object sender, EventArgs e)
        {
            if (animMeshObj != null)
            {
                Doc.Document.Instance.SelectedObject = animMeshObj;
            }
            m_playing = true;
        }

        private void AnimationView_Leave(object sender, EventArgs e)
        {
            m_playing = false;
        }
        private void btnAddCollider_Click(object sender, EventArgs e)
        {
            var collider = new MapToolCore.Collider();
            listColliders.Items.Add(collider);
            pgCollider.SelectedObject = collider;
            if(animMeshObj != null)
            {
                collider.FrameNames = animMeshObj.FrameNames;
            }
            collider.PropertyChanged += Collider_PropertyChanged;
        }

        private void Collider_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            var collider = sender as MapToolCore.Collider;
            ColliderRenderObject newObject = null;
            if (collider == null) return;
            switch (e.PropertyName)
            {
                case "Attribute":
                    (collderNRenderObject[collider] as ColliderRenderObject).SetAttribute(collider.Attribute);
                    break;
                case "Type":
                    if(collderNRenderObject.ContainsKey(collider))
                    {
                        objList.Remove(collderNRenderObject[collider]);
                        collderNRenderObject.Remove(collider);
                    }
                    switch (collider.Type)
                    {
                        case MapToolCore.ColliderType.Box:
                            newObject = new BoxColliderMeshObject(GraphicsDevice.Instance);
                            break;
                        case MapToolCore.ColliderType.Sphare:
                            newObject = new SphareMesh(GraphicsDevice.Instance);
                            break;
                    }
                    if(newObject != null)
                    {
                        newObject.Offset = collider.Offset;
                        newObject.Transform = collider.Transform.Clone();
                        if (collider.FrameName != null && animMeshObj != null)
                        {
                            newObject.SetFrameMatrix(animMeshObj, collider.FrameName);
                        }
                        collderNRenderObject.Add(collider, newObject);
                        objList.Add(newObject);
                    }
                    break;
                case "FrameName":
                    if(collderNRenderObject.ContainsKey(collider))
                    {
                        (collderNRenderObject[collider] as ColliderRenderObject).SetFrameMatrix(animMeshObj, collider.FrameName);
                    }
                    break;
                case "Offset":
                    if (collderNRenderObject.ContainsKey(collider))
                    {
                        (collderNRenderObject[collider] as ColliderRenderObject).Offset = collider.Offset;
                    }
                    break;
                case "Transform":
                    if (collderNRenderObject.ContainsKey(collider))
                    {
                        (collderNRenderObject[collider] as ColliderRenderObject).Transform = collider.Transform.Clone();
                    }
                    break;
            }
            if(pgCollider.SelectedObject == collider)
            {
                pgCollider.Refresh();
            }

            renderView.Render();
        }

        private void listColliders_SelectedIndexChanged(object sender, EventArgs e)
        {
            pgCollider.SelectedObject = listColliders.SelectedItem;
        }

        private void btnPlayAnim_Click(object sender, EventArgs e)
        {
            animationState = AnimationPlayState.Play;
        }

        private void btnStopAnim_Click(object sender, EventArgs e)
        {
            animationState = AnimationPlayState.Stop;
        }

        private void DeleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (listColliders.SelectedItems.Count == 0) return;
            var colliders = new MapToolCore.Collider[listColliders.SelectedItems.Count];
            listColliders.SelectedItems.CopyTo(colliders, 0);
            foreach(var collider in colliders)
            {
                listColliders.Items.Remove(collider);
                if(!collderNRenderObject.ContainsKey(collider)) continue;
                objList.Remove(collderNRenderObject[collider]);
                collderNRenderObject.Remove(collider);
            }
            renderView.Render();
        }
    }
}
