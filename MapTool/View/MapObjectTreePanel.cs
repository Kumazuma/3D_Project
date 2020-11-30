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
    public partial class MapObjectTreePanel : UserControl
    {
        Dictionary<MapToolRender.MapObject, int> indexTable = new Dictionary<MapToolRender.MapObject, int>();
        public MapObjectTreePanel()
        {
            InitializeComponent();
            listBox1.Items.Add(Doc.Document.Instance.World);
             
            Doc.Document.Instance.PropertyChanged += Document_PropertyChanged;
        }
        private void Document_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            int idx = 0;
            MapToolRender.MapObject mapObj = null;
            switch(e.PropertyName)
            {
                case "MapObjects":
                    mapObj = sender as MapToolRender.MapObject;
                    idx = listBox1.Items.Add(mapObj);
                    indexTable.Add(mapObj, idx);
                    mapObj.PropertyChanged += MapObj_PropertyChanged;
                    break;
                case "SelectedObject":
                    mapObj = Doc.Document.Instance.SelectedObject as MapToolRender.MapObject;
                    if(mapObj == null)
                    {
                        return;
                    }
                    if (indexTable.ContainsKey(mapObj))
                    {
                        listBox1.SelectedItem = mapObj;
                    }
                    break;
            }
            MapToolRender.GraphicsDevice.Instance.Render();

        }

        private void MapObj_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            
        }

        private void treeView1_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if(e.Node.Tag != null)
            {
                switch(e.Button)
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
                MapToolRender.GraphicsDevice.Instance.Add( newObj);

            }
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            var items = listBox1.SelectedItems;
            if(items.Count == 1)
            {
                Doc.Document.Instance.SelectedObject = items[0];
            }
            else if(items.Count > 0)
            {
                //TODO:복수개의 이동 방법을 구현해야 한다.
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
    }
}
