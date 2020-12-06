using MapToolRender;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MapToolRender;
namespace MapTool.Doc
{
    public partial class MapFile
    {
        public class Collection :
            ICollection<MapObject>,
            IEnumerable<MapObject>
        {
            MapFile mapFile;
            public Collection(MapFile mapFile) => this.mapFile = mapFile;
            public int Count => mapFile.mapObjects.Count;
            public bool IsReadOnly => false;
            public void Add(MapObject item) => mapFile.mapObjects.Add(item);
            public void Clear()=> mapFile.mapObjects.Clear();
            public bool Contains(MapObject item) => mapFile.mapObjects.Contains(item);
            public void CopyTo(MapObject[] array, int arrayIndex) => mapFile.mapObjects.CopyTo(array, arrayIndex);
            public IEnumerator<MapObject> GetEnumerator() => mapFile.mapObjects.GetEnumerator();
            public bool Remove(MapObject item)=>mapFile.mapObjects.Remove(item);
            IEnumerator IEnumerable.GetEnumerator() => mapFile.mapObjects.GetEnumerator();
        }
    }
}
