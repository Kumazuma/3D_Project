using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MapToolCore;
using MapToolRender;
namespace MapTool.View
{
    public partial class MapObjectTreePanel : UserControl
    {
        BindingList<MapObject> mapObjects = new BindingList<MapObject>();
        public MapObjectTreePanel()
        {
            InitializeComponent();
            listBox1.DisplayMember = nameof(MapObject.Name);
            mapObjects.Add(Doc.Document.Instance.World);
            listBox1.DataSource = mapObjects;

            Doc.Document.Instance.PropertyChanged += Document_PropertyChanged;
        }
        private void Document_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            int idx = 0;
            MapToolRender.MapObject mapObj = null;
            switch (e.PropertyName)
            {
                case "MapObjects":
                    mapObj = sender as MapToolRender.MapObject;
                    if (Doc.Document.Instance.MapObjects.Contains(mapObj))
                    {
                        mapObjects.Add(mapObj);
                        mapObj.PropertyChanged += MapObj_PropertyChanged;
                        mapObjects.ResetBindings();
                    }
                    break;
                case "SelectedObject":
                    mapObj = Doc.Document.Instance.SelectedObject as MapToolRender.MapObject;
                    if (mapObj == null)
                    {
                        return;
                    }
                    if (mapObjects.Contains(mapObj))
                    {
                        listBox1.SelectedItem = mapObj;
                    }
                    break;
            }

        }

        private void MapObj_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            
        }

        private void treeView1_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Node.Tag != null)
            {
                switch (e.Button)
                {
                    case MouseButtons.Left:
                        Doc.Document.Instance.SelectedObject = e.Node.Tag;
                        break;
                    case MouseButtons.Right:
                        contextMenuStrip1.Tag = e.Node.Tag;
                        contextMenuStrip1.Show(sender as Control, new Point(e.X, e.Y));
                        break;
                }
            }

        }

        private void DeleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (listBox1.SelectedItems.Count <= 0)
            {
                return;
            }
            var items = new MapToolRender.MapObject[listBox1.SelectedItems.Count];
            listBox1.SelectedItems.CopyTo(items, 0);
            foreach (var item in items)
            {
                if (Doc.Document.Instance.MapObjects.Contains(item))
                {
                    Doc.Document.Instance.RemoveObject(item);
                    mapObjects.Remove(item);
                }
            }
            mapObjects.ResetBindings();
        }

        private void DuplicateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var items = listBox1.SelectedItems;
            if (items.Count <= 0)
            {
                return;
            }

            var objArray = new MapToolRender.MapObject[items.Count];
            items.CopyTo(objArray, 0);
            foreach (var obj in objArray)
            {
                var o = obj as MapToolRender.RenderObject;
                if (o == null) continue;
                var newObj = o.Clone();
                if (newObj == null) continue;
                Doc.Document.Instance.AddObject(newObj);
                //TODO: 트리에서 복제되었을 때 메인 프레임에서 삭제되었음을 알 수 있게 처리 해야함

            }
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            var items = listBox1.SelectedItems;
            if (items.Count == 1)
            {
                Doc.Document.Instance.SelectedObject = items[0];
            }
            else if (items.Count > 0)
            {
                //TODO:복수개의 이동 방법을 구현해야 한다.
                Doc.Document.Instance.SelectedObject = new MultiSelectObject(items);
            }
        }

        private void listBox1_MouseClick(object sender, MouseEventArgs e)
        {

        }

        private void listBox1_Click(object sender, EventArgs e)
        {

        }

        private void listBox1_MouseDown(object sender, MouseEventArgs e)
        {
            var selectedItems = listBox1.SelectedItems;
            if (selectedItems.Count <= 0) return;
            if (e.Button == MouseButtons.Right)
            {
                var pt = PointToScreen(new Point(e.X, e.Y));
                contextMenuStrip1.Show(pt);
            }
        }
        public class MultiSelectObject : MapToolRender.MapObject
        {
            MapToolRender.RenderObject[] renderObjects;
            MapToolCore.Transform[] baseTransforms;
            MapToolCore.Transform dummyTransform;
            public MultiSelectObject(System.Collections.IList renderObjects)
            {
                int objectCount = renderObjects.Count;

                this.renderObjects = new RenderObject[objectCount];
                baseTransforms = new MapToolCore.Transform[objectCount];

                for(int i = 0; i < objectCount; ++i)
                {
                    this.renderObjects[i] = renderObjects[i] as RenderObject;
                    baseTransforms[i] = this.renderObjects[i].Transform.Clone();
                }
                dummyTransform = new Transform();
                dummyTransform.PropertyChanged += DummyTransform_PropertyChanged;
            }
            public Transform Transform{ get => dummyTransform; }

            private void DummyTransform_PropertyChanged(object sender, PropertyChangedEventArgs e)
            {
                int objectCount = renderObjects.Length;
                for (int i = 0; i < objectCount; ++i)
                {
                    MapToolCore.Position position = baseTransforms[i].Position;
                    MapToolCore.Scale scale = baseTransforms[i].Scale;
                    MapToolCore.Rotation rotation = baseTransforms[i].Rotation;
                    position.X += dummyTransform.Position.X;
                    position.Y += dummyTransform.Position.Y;
                    position.Z += dummyTransform.Position.Z;

                    scale.X = dummyTransform.Scale.X;
                    scale.Y = dummyTransform.Scale.Y;
                    scale.Z = dummyTransform.Scale.Z;

                    rotation.X += dummyTransform.Rotation.X;
                    rotation.Y += dummyTransform.Rotation.Y;
                    rotation.Z += dummyTransform.Rotation.Z;

                    renderObjects[i].Transform.Position = position;
                    renderObjects[i].Transform.Rotation = rotation;
                    renderObjects[i].Transform.Scale = scale;
                }
            }
        }
    }
}
