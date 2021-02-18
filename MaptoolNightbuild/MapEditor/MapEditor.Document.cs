using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MapToolCore;
using MaptoolRenderer;
namespace MaptoolNightbuild.MapEditor
{
    class Document :
        INotifyPropertyChanged
    {
        static Document s_instance = new Document();
        public static Document Instance { get => s_instance; }
        public event PropertyChangedEventHandler PropertyChanged;

        ObjectCollection<HashSet<IRenderable>, IRenderable> renderObjects;
        Document()
        {
            renderObjects = new ObjectCollection<HashSet<IRenderable>, IRenderable>();
        }
        public ObjectCollection<HashSet<IRenderable>, IRenderable> RenderObjects { get => renderObjects; }

    }
}
