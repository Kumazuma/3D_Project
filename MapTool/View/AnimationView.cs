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

            renderView.Parent = splitContainer1.Panel2;
            renderView.RenderObjects = objList;
            renderView.Dock = DockStyle.Fill;
            splitContainer1.Panel2.Controls.Add(renderView);
            m_timer = new Timer();
            m_timer.Interval = 16;
            m_timer.Tick += OnTimerTick;
            stopWatch.Start();
            lastTimeSpan = stopWatch.Elapsed;
            m_timer.Start();
            m_playing = true;
            

        }
        private void OnTimerTick(object sender, EventArgs e)
        {
            var timeSpan = stopWatch.Elapsed;
            var delta = timeSpan - lastTimeSpan;
            lastTimeSpan = timeSpan;
            if (animMeshObj != null && m_playing)
            {
                animMeshObj.Update((int)(delta.TotalSeconds * 1000));
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
            animMeshObj = new MapToolRender.SkinnedXMeshObj(MapToolRender.GraphicsDevice.Instance, openFileDialog.FileName);
            animMeshObj.PropertyChanged += AnimMeshObj_PropertyChanged;
            meta.MeshFilePath = openFileDialog.FileName;
            Doc.Document.Instance.SelectedObject = animMeshObj;
            objList.Clear();
            objList.Add(animMeshObj);
            
            var animCount = animMeshObj.AnimationCount;
            comboAnim.Items.Clear();
            AnimIndex.Items.Clear();
            for (var i = 0u; i < animCount;++i)
            {
                var idx = $"{i}";
                comboAnim.Items.Add(idx);
                AnimIndex.Items.Add(idx);
            }
            foreach(var obj in listColliders.Items)
            {
                (obj as MapToolCore.Collider).FrameNames = animMeshObj.FrameNames;
            }
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
            System.IO.Stream fileStream = null;
            try
            {
                fileStream = System.IO.File.OpenRead(dialog.FileName);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
                MessageBox.Show("파일을 열지 못 했습니다.");
                return;
            }
            var streamReader = new System.IO.StreamReader(fileStream);
            var reader = new Newtonsoft.Json.JsonTextReader(streamReader);
            string propertyName;
            string xMeshPath = "";
            var animTable = new Dictionary<string, int>();
            try
            {
                if (!reader.Read()) throw new Exception("failed parsing");
                if (reader.TokenType != Newtonsoft.Json.JsonToken.StartObject) throw new Exception("failed parsing");
                while(true)
                {
                    if (!reader.Read()) throw new Exception("failed parsing");
                    if (reader.TokenType == Newtonsoft.Json.JsonToken.EndObject) break;
                    if (reader.TokenType != Newtonsoft.Json.JsonToken.PropertyName) throw new Exception("failed parsing");
                    propertyName = reader.Value as string;
                    if (propertyName == null) throw new Exception("failed parsing");
                    if (propertyName == "x_file_path")
                    {
                        xMeshPath = reader.ReadAsString();
                    }
                    else if(propertyName == "animations")
                    {
                        if(!reader.Read()) throw new Exception("failed parsing");
                        if(reader.TokenType != Newtonsoft.Json.JsonToken.StartObject) throw new Exception("failed parsing");
                        while(reader.TokenType != Newtonsoft.Json.JsonToken.EndObject)
                        {
                            if(!reader.Read()) throw new Exception("failed parsing");
                            if (reader.TokenType == Newtonsoft.Json.JsonToken.EndObject) break;
                            if (reader.TokenType != Newtonsoft.Json.JsonToken.PropertyName) throw new Exception("failed parsing");
                            propertyName = reader.Value as string;
                            int? number = reader.ReadAsInt32();
                            if (number == null) throw new Exception("failed parsing");
                            animTable.Add(propertyName, number.Value);
                        }
                    }
                }

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
                MessageBox.Show("파싱에 실패했습니다.");
                return;
            }
            dataGridView1.Rows.Clear();
            objList.Clear();
            string path= System.IO.Path.Combine(MapToolCore.Environment.Instance.ProjectDirectory, xMeshPath);
            animMeshObj = Doc.MeshManager.Instance.GetSkinnedMesh(path);
            animMeshObj.PropertyChanged += AnimMeshObj_PropertyChanged;
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
            foreach(var item in animTable)
            {
                dataGridView1.Rows.Add(item.Value.ToString(), item.Key);
            }
            m_currentJsonPath = dialog.FileName;
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
            
            System.IO.Stream fileStream = null;
            try
            {
                fileStream = System.IO.File.OpenWrite(m_currentJsonPath);
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex);
                MessageBox.Show("파일을 열지 못 했습니다.");
                m_currentJsonPath = null;
                return;
            }
            meta.AnimationTable.Clear();
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
            meta.Save(m_currentJsonPath);
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
            if (collider == null) return;
            if (e.PropertyName != "Attribute") return;
            objList.Remove(collderNRenderObject[collider]);
            collderNRenderObject.Remove(collider);
            switch (collider.Type)
            {
                case MapToolCore.ColliderType.Box:
                    //TODO:여기에서 박스 렌더 오브젝트를 넣어준다.
                    break;
                case MapToolCore.ColliderType.Sphare:
                    //TODO:여기에서 스페어 렌더 오브젝트를 넣어준다.
                    break;
            }
            renderView.Render();
        }

        private void listColliders_SelectedIndexChanged(object sender, EventArgs e)
        {
            pgCollider.SelectedObject = listColliders.SelectedItem;
        }
    }
}
