using System;
using System.Collections.Generic;
using System.Collections;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapToolCore
{
    public class ObjectCollection<Collection, ItemType> :
    ICollection<ItemType>,
    System.Collections.Specialized.INotifyCollectionChanged
    where Collection : ICollection<ItemType>, new()
    {
        Collection items;
        public ObjectCollection()
        {
            this.items = new Collection();
        }

        public int Count => this.items.Count;
        public bool IsReadOnly => false;

        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public void Add(ItemType item)
        {
            this.items.Add(item);
            var arg = new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add, item);
            CollectionChanged?.Invoke(this, arg);
        }

        public void Clear()
        {
            if (this.items.Count != 0)
            {
                ItemType[] removedItems = new ItemType[this.items.Count];
                this.items.CopyTo(removedItems, 0);
                this.items.Clear();
                var arg = new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Remove, removedItems);
                CollectionChanged?.Invoke(this, arg);
            }

        }

        public bool Contains(ItemType item) => this.items.Contains(item);
        public void CopyTo(ItemType[] array, int arrayIndex)
        {
            this.items.CopyTo(array, arrayIndex);
        }

        public IEnumerator<ItemType> GetEnumerator() => this.items.GetEnumerator();
        public bool Remove(ItemType item)
        {
            var removed = this.items.Remove(item);
            if (removed)
            {
                var arg = new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Remove, item);
                CollectionChanged?.Invoke(this, arg);
            }
            return removed;
        }
        IEnumerator IEnumerable.GetEnumerator() => this.items.GetEnumerator();
        public void AddRange(IEnumerable<ItemType> items)
        {
            var newItems = new List<ItemType>();
            foreach (var item in items)
            {
                this.items.Add(item);
                newItems.Add(item);
            }
            
            var arg = new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Add, newItems as IList);
            CollectionChanged?.Invoke(this, arg);
        }
    }
}
