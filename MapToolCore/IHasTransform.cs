﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapToolCore
{
    public interface IHasTransform
    {
        Transform Transform
        {
            get;set;
        }
    }
}
