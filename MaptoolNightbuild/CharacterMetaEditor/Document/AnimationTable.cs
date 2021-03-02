using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MaptoolNightbuild.CharacterMetaEditor
{
    class AnimationIDEnumerator :
    IEnumerator<KeyValuePair<int, String>>
    {
        List<String> animationIDs;
        int index;
        public AnimationIDEnumerator(List<String> ids)
        {
            animationIDs = ids;
            index = -1;
        }
        public KeyValuePair<int, string> Current => new KeyValuePair<int, string>(index, animationIDs[index]);

        object IEnumerator.Current => new KeyValuePair<int, string>(index, animationIDs[index]);

        public void Dispose()
        {
            animationIDs = null;
        }

        public bool MoveNext()
        {
            index++;
            return index < animationIDs.Count;
        }

        public void Reset()
        {
            index = -1;
        }
    }
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class AnimationTable :
        ICollection<KeyValuePair<int, String>>,
        ICustomTypeDescriptor

    {
        List<String> animationIDs = new List<string>();
        public AnimationTable(int animCount)
        {
            animationIDs.Capacity = animCount + 1;
            for (int i = 0; i < animCount; ++i)
            {
                animationIDs.Add("");
            }
        }
        public String this[int index]
        {
            get => animationIDs[index];
            set => animationIDs[index] = value;
        }

        public int Count => animationIDs.Count;

        public bool IsReadOnly => true;

        public void Add(KeyValuePair<int, string> item)
        {
            throw new NotImplementedException();
        }

        public void Clear()
        {

        }

        public bool Contains(KeyValuePair<int, string> item)
        {
            return item.Key < animationIDs.Count;
        }

        public void CopyTo(KeyValuePair<int, string>[] array, int arrayIndex)
        {
            int count = Math.Min(array.Length - arrayIndex, animationIDs.Count);
            for (int i = 0; i < count; ++i)
            {
                array[i + arrayIndex] = new KeyValuePair<int, string>(i, animationIDs[i]);
            }
        }
        public IEnumerator<KeyValuePair<int, string>> GetEnumerator()
        {
            return new AnimationIDEnumerator(animationIDs);
        }
        public bool Remove(KeyValuePair<int, string> item)
        {
            throw new NotImplementedException();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return new AnimationIDEnumerator(animationIDs);
        }

        public AttributeCollection GetAttributes() => TypeDescriptor.GetAttributes(this, true);
        public string GetClassName() => TypeDescriptor.GetClassName(this,true);
        public string GetComponentName() => TypeDescriptor.GetComponentName(this, true);
        public TypeConverter GetConverter() => TypeDescriptor.GetConverter(this, true);
        public EventDescriptor GetDefaultEvent() => TypeDescriptor.GetDefaultEvent(this, true);
        public PropertyDescriptor GetDefaultProperty() => TypeDescriptor.GetDefaultProperty(this, true);
        public object GetEditor(Type editorBaseType) => TypeDescriptor.GetEditor(this, editorBaseType, true);

        public EventDescriptorCollection GetEvents() => TypeDescriptor.GetEvents(this, true);

        public EventDescriptorCollection GetEvents(Attribute[] attributes) => TypeDescriptor.GetEvents(this, attributes,true);
        public PropertyDescriptorCollection GetProperties(Attribute[] attributes) => GetProperties();
        public object GetPropertyOwner(PropertyDescriptor pd) => this;

        public PropertyDescriptorCollection GetProperties()
        {
            var collection = new PropertyDescriptorCollection(null);
            for (int i = 0; i < animationIDs.Count; ++i)
            {
                collection.Add(new AnimationTableItemDescriptor(animationIDs, i));
            }
            return collection;
        }
    }

    public class AnimationTableItemDescriptor : PropertyDescriptor
    {
        List<String> animationIDs;
        int index;

        public AnimationTableItemDescriptor(List<String> animationIDs, int index_) :
            base(index_.ToString(), null)
        {
            this.animationIDs = animationIDs;
            this.index = index_;
        }
        public override Type ComponentType => typeof(AnimationTable);
        public override bool IsReadOnly => false;
        public override Type PropertyType => typeof(String);
        public override bool CanResetValue(object component) => false;
        public override object GetValue(object component) => animationIDs[index];

        public override void ResetValue(object component)
        {
            animationIDs[index] = "";
        }

        public override void SetValue(object component, Object value) => animationIDs[index] = value as String;
        public override bool ShouldSerializeValue(object component)
        {
            return false;
        }
    }
}
