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
        public MapObjectTreePanel()
        {
            InitializeComponent();
            Doc.Document.Instance.PropertyChanged += Document_PropertyChanged;    
        }

        private void Document_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch(e.PropertyName)
            {
                case "MapObjects":
                    var mapObj = sender as MapToolRender.MapObject;
                    var node = treeView1.Nodes.Add(mapObj.GetHashCode().ToString(), mapObj.Name);
                    mapObj.PropertyChanged += MapObj_PropertyChanged;
                    node.Tag = mapObj;
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
    }
}
