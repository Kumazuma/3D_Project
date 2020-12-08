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
    public enum ColliderType { None, Box, Sphare };
    public class FormatStringConverter : StringConverter
    {
        public override Boolean GetStandardValuesSupported(ITypeDescriptorContext context) { return true; }
        public override Boolean GetStandardValuesExclusive(ITypeDescriptorContext context) { return true; }
        public override TypeConverter.StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
        {
            var t = context.Instance as Collider;
            return new StandardValuesCollection(t.FrameNames as System.Collections.ICollection);
        }
    }
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class Collider: INotifyPropertyChanged
    {
        protected Offset offset;
        protected ColliderType type = ColliderType.None;
        protected string frameName;
        protected ColliderAttribute attribute;
        protected ICollection<string> frameNames;
        PropertyChangedEventHandler m_offsetPropertyChangedEventHandler;
        public event PropertyChangedEventHandler PropertyChanged;
        
        public Collider()
        {
            m_offsetPropertyChangedEventHandler = new PropertyChangedEventHandler(OnOffsetPropertyChanged);
            offset = new Offset();
            offset.PropertyChanged += m_offsetPropertyChangedEventHandler;
        }
        protected Collider(Collider collider)
        {
            m_offsetPropertyChangedEventHandler = new PropertyChangedEventHandler(OnOffsetPropertyChanged);
            offset = new Offset();
            offset.PropertyChanged += m_offsetPropertyChangedEventHandler;
            offset.X = collider.Offset.X;
            offset.Y = collider.Offset.Y;
            offset.Z = collider.Offset.Z;
        }
        private void OnOffsetPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Offset"));
        }
        [Browsable(false)]
        public ICollection<string> FrameNames
        {
            get => frameNames;
            set
            {
                frameNames = value;
            }
        }
        [TypeConverter(typeof(FormatStringConverter))]
        public string FrameName
        {
            get => frameName;
            set => frameName = value;
        }

        [CategoryAttribute("Common")]
        public ColliderType Type
        {
            get => type;
            set
            {
                type = value;
                switch (type)
                {
                    case ColliderType.None:
                        attribute = null;
                        break;
                    case ColliderType.Box:
                        attribute = new BoxColliderAttribute();
                        break;
                    case ColliderType.Sphare:
                        attribute = new SphareColliderAttribute();
                        break;
                }
                BroadcastPropertyChanged("Type");
                BroadcastPropertyChanged("Attribute");
            }
        }
        [CategoryAttribute("Common")]
        public ColliderAttribute Attribute{ get => attribute; }

        [CategoryAttribute("Common")]
        public Offset Offset
        {
            get => offset;
            set
            {
                if(value == null) return;
                this.offset.PropertyChanged -= m_offsetPropertyChangedEventHandler;
                this.offset = value;
                this.offset.PropertyChanged += m_offsetPropertyChangedEventHandler;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Offset"));
            }
        }
        protected void BroadcastPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class Offset: INotifyPropertyChanged
    {
        float x;
        float y;
        float z;
        public Offset():
            this(0.0f, 0.0f, 0.0f)
        {
        }
        public Offset(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        public float X
        {
            get => x;
            set
            {
                x = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("X"));
            }
        }
        public float Y
        {
            get => y;
            set
            {
                y = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Y"));
            }
        }
        public float Z
        {
            get => z;
            set
            {
                z = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Z"));
            }
        }
        
        public event PropertyChangedEventHandler PropertyChanged;
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
    public class SphareColliderAttribute : ColliderAttribute
    {
        private float radius;

        public SphareColliderAttribute()
        {
            radius = 1f;
        }
        public SphareColliderAttribute(SphareColliderAttribute rhs)
        {
            radius = rhs.radius;
        }
        [CategoryAttribute("Sphare")]
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
            return new SphareColliderAttribute(this);
        }
    }
    public class BoxCollider : Collider
    {
        private float width;
        private float height;
        private float depth;
        public BoxCollider()
        {
            width = 1.0f;
            height = 1.0f;
            depth = 1.0f;
        }
        public BoxCollider(BoxCollider rhs):
            base(rhs)
        {
            width = rhs.width;
            height = rhs.height;
            depth = rhs.depth;
        }
        [CategoryAttribute("Box")]
        public float Width
        {
            get => width;
            set
            {
                width = value;
                BroadcastPropertyChanged("Width");
            }
        }
        [CategoryAttribute("Box")]
        public float Height
        {
            get => height;
            set
            {
                height = value;
                BroadcastPropertyChanged("Height");
            }
        }
        [CategoryAttribute("Box")]
        public float Depth
        {
            get => depth;
            set
            {
                depth = value;
                BroadcastPropertyChanged("Depth");
            }
        }
    }
    public class SphareCollider : Collider
    {
        private float radius;
        public SphareCollider()
        {
            radius = 1.0f;
        }
        public SphareCollider(SphareCollider rhs) :
            base(rhs)
        {
            radius = rhs.radius;
        }
        [CategoryAttribute("Sphare")]
        public float Radius
        {
            get => radius;
            set
            {
                radius = value;
                BroadcastPropertyChanged("Radius");
            }
        }
    }
}
