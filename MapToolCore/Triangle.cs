using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapToolCore
{
    public struct Triangle
    {
        private int index1;
        private int index2;
        private int index3;
        public int this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0:
                        return index1;
                    case 1:
                        return index2;
                    case 2:
                        return index3;
                }
                throw new IndexOutOfRangeException();
            }
            set
            {
                switch (index)
                {
                    case 0:
                        index1 = value;
                        break;
                    case 1:
                        index2 = value;
                        break;
                    case 2:
                        index3 = value;
                        break;
                    default:
                        throw new IndexOutOfRangeException();
                }
            }
        }
        public override string ToString() => $"({index1}, {index2}, {index3})";
    }
}
