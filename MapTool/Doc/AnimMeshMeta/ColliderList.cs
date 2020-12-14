using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapTool.Doc
{
    class ColliderList : ICollection<MapToolCore.Collider>
    {
        List<MapToolCore.Collider> colliders = new List<MapToolCore.Collider>();

        public int Count => colliders.Count;
        public bool IsReadOnly => false;
        public void Add(MapToolCore.Collider item)=>colliders.Add(item);
        public void Clear()=> colliders.Clear();

        public bool Contains(MapToolCore.Collider item) => colliders.Contains(item);

        public void CopyTo(MapToolCore.Collider[] array, int arrayIndex) => colliders.CopyTo(array, arrayIndex);

        public IEnumerator<MapToolCore.Collider> GetEnumerator() => colliders.GetEnumerator();

        public bool Remove(MapToolCore.Collider item) => colliders.Remove(item);

        IEnumerator IEnumerable.GetEnumerator() => colliders.GetEnumerator();
    }
}
