using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MapToolRender;
using System.IO;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using MapToolCore;
using System.Collections.Generic;
using System;

namespace MapTool.Doc
{
    partial class AnimationMeshMeta
    {
        delegate JObject SerializerCollider(Collider obj);
        
        static JObject SerializerSphareCollider(Collider collider)
        {
            var attribute = collider.Attribute as SphareColliderAttribute;
            var jCollider = new JObject();
            jCollider["type"] = "SPHARE";
            jCollider["frame_name"] = collider.FrameName;
            jCollider["transform"] = collider.Transform.Serialize();
            jCollider["offset"] = collider.Offset.Serialize();
            jCollider["radius"] = attribute.Radius;
            return jCollider;
        }
        static JObject SerializerBoxCollider(Collider collider)
        {
            var attribute = collider.Attribute as BoxColliderAttribute;
            var jCollider = new JObject();
            jCollider["type"] = "BOX";
            jCollider["frame_name"] = collider.FrameName;
            jCollider["transform"] = collider.Transform.Serialize();
            jCollider["offset"] = collider.Offset.Serialize();
            jCollider["width"] = attribute.Width;
            jCollider["heigth"] = attribute.Height;
            jCollider["depth"] = attribute.Depth;
            return jCollider;
        }
        static JContainer SaveColliderMeta(JObject jRoot, ColliderList colliderList)
        {
            var serializerTable = new Dictionary<Type, SerializerCollider> {

                { typeof(SphareColliderAttribute),SerializerSphareCollider },
                { typeof(BoxColliderAttribute), SerializerBoxCollider }
            };
            
            var jColliderList = new JArray(
                from collider in colliderList
                select serializerTable[collider.Attribute.GetType()](collider)
                );
            
            return jColliderList;
        }
    }
}
