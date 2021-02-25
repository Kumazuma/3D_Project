using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapToolCore
{
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public abstract class ColliderAttribute: INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        protected void BroadcastPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        public abstract ColliderAttribute Clone() ;
    }
    public enum ColliderType { None, Box, Sphere };
    public class FormatStringConverter : StringConverter
    {
        public override Boolean GetStandardValuesSupported(ITypeDescriptorContext context) { return true; }
        public override Boolean GetStandardValuesExclusive(ITypeDescriptorContext context) { return true; }
        public override TypeConverter.StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
        {
            var type = context.Instance.GetType();
            var property = type.GetProperty("FrameNames");
            if(property == null)
            {
                return new StandardValuesCollection(null);
            }
            var list = property.GetValue(context.Instance);
            return new StandardValuesCollection(list as System.Collections.ICollection);
        }
    }
    

    public class BoxColliderAttribute : ColliderAttribute
    {
        private float width;
        private float height;
        private float depth;
        public BoxColliderAttribute()
        {
            width = 1.0f;
            height = 1.0f;
            depth = 1.0f;
        }
        public BoxColliderAttribute(BoxColliderAttribute rhs) 
        {
            width = rhs.width;
            height = rhs.height;
            depth = rhs.depth;
        }
        public float Width
        {
            get => width;
            set
            {
                width = value;
                BroadcastPropertyChanged("Width");
            }
        }
        public float Height
        {
            get => height;
            set
            {
                height = value;
                BroadcastPropertyChanged("Height");
            }
        }
        public float Depth
        {
            get => depth;
            set
            {
                depth = value;
                BroadcastPropertyChanged("Depth");
            }
        }
        public override ColliderAttribute Clone()
        {
            return new BoxColliderAttribute(this);
        }
    }
    public class SphereColliderAttribute : ColliderAttribute
    {
        private float radius;

        public SphereColliderAttribute()
        {
            radius = 1f;
        }
        public SphereColliderAttribute(SphereColliderAttribute rhs)
        {
            radius = rhs.radius;
        }
        [CategoryAttribute("Sphere")]
        public float Radius
        {
            get => radius;
            set
            {
                radius = value;
                BroadcastPropertyChanged("Radius");
            }
        }
        public override ColliderAttribute Clone()
        {
            return new SphereColliderAttribute(this);
        }
    }
    
}
