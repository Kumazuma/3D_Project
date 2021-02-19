using MapToolCore;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MaptoolNightbuild.MapEditor
{
    public class MultiSelectTransforms: MapToolCore.IHasTransform
    {
        IList<IHasTransform> renderObjects;
        MapToolCore.Transform[] baseTransforms;
        MapToolCore.Transform dummyTransform;
        public MultiSelectTransforms(IList<IHasTransform> renderObjects)
        {
            this.renderObjects = renderObjects;
            baseTransforms = new MapToolCore.Transform[renderObjects.Count];

            for (int i = 0; i < renderObjects.Count; ++i)
            {
                baseTransforms[i] = this.renderObjects[i].Transform.Clone();
            }
            dummyTransform = new Transform();
            dummyTransform.PropertyChanged += DummyTransform_PropertyChanged;
        }
        //public Transform Transform { get => dummyTransform; }
        public virtual Transform Transform { get => dummyTransform; set => throw new NotImplementedException(); }

        private void DummyTransform_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            int objectCount = renderObjects.Count;
            System.Threading.Tasks.Parallel.ForEach(renderObjects, (IHasTransform renderObject, ParallelLoopState state, long index) =>
            {
                MapToolCore.Position position = baseTransforms[index].Position;
                MapToolCore.Rotation rotation = baseTransforms[index].Rotation;
                position.X += dummyTransform.Position.X;
                position.Y += dummyTransform.Position.Y;
                position.Z += dummyTransform.Position.Z;


                rotation.X += dummyTransform.Rotation.X;
                rotation.Y += dummyTransform.Rotation.Y;
                rotation.Z += dummyTransform.Rotation.Z;

                renderObjects[(int)index].Transform.Position = position;
                renderObjects[(int)index].Transform.Rotation = rotation;
                renderObjects[(int)index].Transform.Scale = baseTransforms[index].Scale;
            });
        }
    }
}
