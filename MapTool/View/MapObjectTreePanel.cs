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
    public delegate void ChangedTreeNodeSelectedHandler(object selectedObj);
    public partial class MapObjectTreePanel : UserControl
    {
        TreeNode m_rootNode;
        public event ChangedTreeNodeSelectedHandler SelectedObjChanged;
        public MapObjectTreePanel()
        {
            InitializeComponent();
            m_rootNode = treeView1.Nodes.Add(Doc.Document.Instance.World.GetHashCode().ToString(), "world");
            m_rootNode.Tag = Doc.Document.Instance.World;
            Doc.Document.Instance.PropertyChanged += Document_PropertyChanged;
        }

        private void Document_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch(e.PropertyName)
            {
                case "MapObjects":
                    var mapObj = sender as MapToolRender.MapObject;
                    var node = m_rootNode.Nodes.Add(mapObj.GetHashCode().ToString(), mapObj.Name);
                    mapObj.PropertyChanged += MapObj_PropertyChanged;
                    node.Tag = mapObj;
                    break;
                case "SelectedObject":
                    var selectedNode = treeView1.Nodes.Find(Doc.Document.Instance.SelectedObject.GetHashCode().ToString(), true)[0];
                    treeView1.SelectedNode = selectedNode;
                    break;
            }
        }

        private void MapObj_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch(e.PropertyName)
            {
                case "Name":
                    treeView1.Nodes.Find(sender.GetHashCode().ToString(), true)[0].Text = (sender as MapToolRender.MapObject).Name;
                    break;

            }
        }

        private void treeView1_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if(e.Node.Tag != null)
            {
                Doc.Document.Instance.SelectedObject = e.Node.Tag;
            }
        }
    }
}
