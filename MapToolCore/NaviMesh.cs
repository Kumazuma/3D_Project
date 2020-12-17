using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Collections.ObjectModel;

namespace MapToolCore
{
    class NaviMesh : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        ObservableCollection<Position> positions = new ObservableCollection<Position>();
        ObservableCollection<Triangle> triangles = new ObservableCollection<Triangle>();
        CompositingTriangleCollection compositingTriangle;
        public NaviMesh()
        {
            compositingTriangle = new CompositingTriangleCollection(this);
        }
        protected void NotifyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public ObservableCollection<Triangle> Triangles
        {
            get => triangles;
        }
        public ObservableCollection<Position> Positions
        {
            get => positions;
        }
        public CompositingTriangleCollection CompositingTriangle
        {
            get => compositingTriangle;
        }

        public class CompositingTriangleCollection
        {
            NaviMesh parent;
            List<int> indices = new List<int>(3);
            public CompositingTriangleCollection(NaviMesh parent) => this.parent = parent;

        }

    }
}
