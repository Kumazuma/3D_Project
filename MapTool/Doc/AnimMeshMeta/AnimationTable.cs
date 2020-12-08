using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MapToolRender;
namespace MapTool.Doc
{
    class AnimationTableItem
    {
        public AnimationTableItem(uint index, string id)
        {
            Index = index;
            ID = id;
        }
        public uint Index { get; }
        public string ID { get; } = "";
        public override int GetHashCode()
        {
            uint index = Index;
            string id = ID;
            return index.GetHashCode() + id.GetHashCode();
        }
        public override bool Equals(object obj)
        {
            if (obj == this) return true;
            var o = obj as AnimationTableItem;
            if (o == null) return false;
            return o.ID == ID && o.Index == Index;
        }
    }
    
    class AnimationTable : ICollection<AnimationTableItem>
    {
        Dictionary<uint, AnimationTableItem> animIndices = new Dictionary<uint, AnimationTableItem>();
        Dictionary<string, AnimationTableItem> animIDs = new Dictionary<string, AnimationTableItem>();
        HashSet<AnimationTableItem> items = new HashSet<AnimationTableItem>();

        public event PropertyChangedEventHandler PropertyChanged;
        public int Count => throw new NotImplementedException();

        public bool IsReadOnly => throw new NotImplementedException();

        public bool HasIndex(uint index) => animIndices.ContainsKey(index);
        public bool HasID(string id) => animIDs.ContainsKey(id);

        public void Add(AnimationTableItem item)
        {
            if(HasIndex(item.Index) || HasID(item.ID)) return;
            items.Add(item);
            animIndices.Add(item.Index, item);
            animIDs.Add(item.ID, item);
        }
        public IEnumerator<AnimationTableItem> Item { get => items.GetEnumerator(); }
        public void Clear()
        {
            items.Clear();
            animIndices.Clear();
            animIDs.Clear();
        }

        public bool Contains(AnimationTableItem item)
        {
            return items.Contains(item);
        }

        public void CopyTo(AnimationTableItem[] array, int arrayIndex)
        {
            items.CopyTo(array, arrayIndex);
        }

        public bool Remove(AnimationTableItem item)
        {
            if(!items.Contains(item)) return false;
            items.Remove(item);
            animIDs.Remove(item.ID);
            animIndices.Remove(item.Index);
            return true;
        }

        public IEnumerator<AnimationTableItem> GetEnumerator()
        {
            return items.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return items.GetEnumerator();
        }
    }
}