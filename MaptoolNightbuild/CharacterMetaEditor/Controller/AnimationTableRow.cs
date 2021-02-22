using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MaptoolNightbuild.CharacterMetaEditor
{
    public class AnimationTableRow
    {
        public AnimationTableRow(uint index, string id)
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
            var o = obj as AnimationTableRow;
            if (o == null) return false;
            return o.ID == ID && o.Index == Index;
        }
    }
    public class AnimationTable : ICollection<AnimationTableRow>
    {
        Dictionary<uint, AnimationTableRow> animIndices = new Dictionary<uint, AnimationTableRow>();
        Dictionary<string, AnimationTableRow> animIDs = new Dictionary<string, AnimationTableRow>();
        HashSet<AnimationTableRow> items = new HashSet<AnimationTableRow>();

        public int Count => throw new NotImplementedException();

        public bool IsReadOnly => throw new NotImplementedException();

        public bool HasIndex(uint index) => animIndices.ContainsKey(index);
        public bool HasID(string id) => animIDs.ContainsKey(id);

        public void Add(AnimationTableRow item)
        {
            if (HasIndex(item.Index) || HasID(item.ID)) return;
            items.Add(item);
            animIndices.Add(item.Index, item);
            animIDs.Add(item.ID, item);
        }
        public IEnumerator<AnimationTableRow> Item { get => items.GetEnumerator(); }
        public void Clear()
        {
            items.Clear();
            animIndices.Clear();
            animIDs.Clear();
        }

        public bool Contains(AnimationTableRow item)
        {
            return items.Contains(item);
        }

        public void CopyTo(AnimationTableRow[] array, int arrayIndex)
        {
            items.CopyTo(array, arrayIndex);
        }

        public bool Remove(AnimationTableRow item)
        {
            if (!items.Contains(item)) return false;
            items.Remove(item);
            animIDs.Remove(item.ID);
            animIndices.Remove(item.Index);
            return true;
        }

        public IEnumerator<AnimationTableRow> GetEnumerator()
        {
            return items.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return items.GetEnumerator();
        }
    }
}
