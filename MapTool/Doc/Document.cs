using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
namespace MapTool.Doc
{
    class Document: INotifyPropertyChanged
    {
        private static Document s_instance;
        private object m_selectedObjects;
        private List<MapToolRender.MapObject> m_mapObjects = new List<MapToolRender.MapObject>();
        public MapToolRender.MapObject World { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;
        public object SelectedObject
        {
            get => m_selectedObjects;
            set
            {
                m_selectedObjects = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("SelectedObject"));
            }
        }
        public static Document Instance
        {
            get
            {
                if(s_instance == null)
                {
                    lock (typeof(Document))
                    {
                        if (s_instance == null)
                        {
                            s_instance = new Document();
                        }
                    }
                }
                return s_instance;
            }
        }
        public List<MapToolRender.MapObject> MapObjects
        {
            get => m_mapObjects;
            set => m_mapObjects = value;
        }

        public void AddObject(MapToolRender.MapObject mapObj)
        {
            m_mapObjects.Add(mapObj);
            PropertyChanged?.Invoke(mapObj, new PropertyChangedEventArgs("MapObjects"));
        }
    }

}
