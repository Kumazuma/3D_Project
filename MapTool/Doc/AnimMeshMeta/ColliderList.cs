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

        public bool Contains(MapToolCore.Collider item)
        {
            throw new NotImplementedException();
        }

        public void CopyTo(MapToolCore.Collider[] array, int arrayIndex)
        {
            throw new NotImplementedException();
        }

        public IEnumerator<MapToolCore.Collider> GetEnumerator()
        {
            throw new NotImplementedException();
        }

        public bool Remove(MapToolCore.Collider item)
        {
            throw new NotImplementedException();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            throw new NotImplementedException();
        }
    }
}
