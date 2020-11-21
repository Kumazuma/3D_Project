using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapTool.Doc
{
    class Document
    {
        private static Document s_instance;
        private List<MapToolRender.RenderObject> m_renderObjects;
        public Document Instance
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
        public List<MapToolRender.RenderObject> RenderObjects
        {
            get => m_renderObjects;
            set => m_renderObjects = value;
        }
    }

}
