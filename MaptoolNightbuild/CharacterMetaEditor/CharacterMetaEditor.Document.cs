using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MaptoolRenderer;
using MapToolCore;
namespace MaptoolNightbuild.CharacterMetaEditor
{

    public class Document:
        INotifyPropertyChanged
    {
        
        public event PropertyChangedEventHandler PropertyChanged;
        static Document s_instance;
        ObjectCollection<HashSet<IRenderable>, IRenderable> renderObjects;
        ObjectCollection<List<ColliderObject>, ColliderObject> colliderObjects;
        SkinnedXMeshObject xMeshObject;
        private Document()
        {
            renderObjects = new ObjectCollection<HashSet<IRenderable>, IRenderable>();
            renderObjects.CollectionChanged += RenderObjects_CollectionChanged;

            colliderObjects = new ObjectCollection<List<ColliderObject>, ColliderObject>();
            colliderObjects.CollectionChanged += ColliderObjects_CollectionChanged;
        }

        private void ColliderObjects_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            switch(e.Action)
            {
                case NotifyCollectionChangedAction.Add:
                    {
                        IRenderable[] renderables = new IRenderable[e.NewItems.Count];
                        e.NewItems.CopyTo(renderables, 0);
                        renderObjects.AddRange(renderables);
                    }
                    break;
                case NotifyCollectionChangedAction.Reset:
                case NotifyCollectionChangedAction.Remove:
                    foreach (var item in e.OldItems)
                    {
                        renderObjects.Remove(item as IRenderable);
                    }
                    break;
            }
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("ColliderObject"));
        }

        public ObjectCollection<HashSet<IRenderable>, IRenderable> RenderObjects
        {
            get => renderObjects;
        }
        public ObjectCollection<List<ColliderObject>, ColliderObject> ColliderObject
        {
            get => colliderObjects;
        }
        public SkinnedXMeshObject Mesh
        {
            get => xMeshObject;
            set
            {
                xMeshObject = value;
                PropertyChanged?.Invoke(this,new PropertyChangedEventArgs("Mesh"));
            }
        }

        private void RenderObjects_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            PropertyChanged?.Invoke(this,new PropertyChangedEventArgs("RenderObjects"));
        }
        public static Document Instance
        {
            get
            {
                if(s_instance == null)
                {
                    lock(typeof(Document))
                    {
                        if(s_instance == null)
                        {
                            s_instance = new Document();
                        }
                    }
                }
                return s_instance;
            }
        }
    }
}
