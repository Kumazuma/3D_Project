using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MaptoolRenderer;
using MapToolCore;
using System.ComponentModel;

namespace MaptoolNightbuild.MapEditor
{
    public class MapTarget :
        SphereObject,
        IHasTransform,
        IHasName,
        IHasText,
        INotifyPropertyChanged
    {
        string name = "Target";
        Transform transform = new Transform();
        public MapTarget()
        {
            transform.PropertyChanged += Transform_PropertyChanged;
        }

        private void Transform_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Transform"));
        }

        public Transform Transform {
            get => transform;
            set
            {
                transform.PropertyChanged -= Transform_PropertyChanged;
                transform = value;
                transform.PropertyChanged += Transform_PropertyChanged;
            }

        }
        public string Name {
            get => name;
            set
            {
                name = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Name"));
            }
        }
        [Browsable(false)]
        public string Text
        {
            get => $"{name}(type:{GetType().Name})";
        }
        public event PropertyChangedEventHandler PropertyChanged;

        public override void Render(GraphicsDevice renderer)
        {
            DoRender(renderer, 5, transform);
        }
    }
}
