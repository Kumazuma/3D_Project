using MapToolCore;
using MaptoolRenderer;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MaptoolNightbuild.CharacterMetaEditor
{
    partial class MetaFile
    {
        Dictionary<int, String> animationTable = new Dictionary<int, string>();
        Dictionary<String, ColliderObject> colliderList = new Dictionary<String, ColliderObject>();
        public MetaFile()
        {

        }
        private MetaFile(JObject jObj, string projectDir)
        {
            if (!jObj.ContainsKey("mesh")) throw new Exception("has no mesh attribute");
            if (jObj["mesh"].Type != JTokenType.String) throw new Exception("has no mesh attribute");
            MeshFilePath = projectDir + jObj.Value<string>("mesh");
            if (!jObj.ContainsKey("anim")) throw new Exception("has no anim attribute");
            if (jObj["anim"].Type != JTokenType.Object) throw new Exception("has no anim attribute");
            var anims = jObj["anim"] as JObject;
            foreach (var item in anims)
            {
                int index = item.Value.Value<int>();
                animationTable.Add(index, item.Key);
            }

            if (!jObj.ContainsKey("collider")) throw new Exception("has no collider attribute");
            var jColliders = jObj["collider"];
            if (jColliders.Type != JTokenType.Object) throw new Exception("has no collider attribute");
            foreach (var item in (jColliders as JObject))
            {
                String name = item.Key;
                JObject jCollider = item.Value as JObject;
                var collider = new ColliderObject();
                collider.FrameName = jCollider.Value<string>("frame_name");
                
                collider.Offset = Offset.Parse(jCollider["offset"] as JObject);
                switch (jCollider.Value<string>("type"))
                {
                    case "BOX":
                        collider.Type = ColliderType.Box;
                        {
                            var attribute = collider.Attribute as BoxColliderAttribute;
                            attribute.Depth = jCollider.Value<float>("depth");
                            attribute.Height = jCollider.Value<float>("height");
                            attribute.Width = jCollider.Value<float>("width");
                        }
                        break;
                    case "SPHARE":
                        collider.Type = ColliderType.Sphere;
                        {
                            var attribute = collider.Attribute as SphereColliderAttribute;
                            attribute.Radius = jCollider.Value<float>("radius");
                        }
                        break;
                }
                collider.Name = name;
                colliderList.Add(name, collider);
            }
        }
        delegate JObject SerializerCollider(ColliderObject obj);
        static JObject SerializerSphereCollider(ColliderObject collider)
        {
            var attribute = collider.Attribute as SphereColliderAttribute;
            var jCollider = new JObject();
            jCollider["type"] = "SPHARE";
            jCollider["frame_name"] = collider.FrameName;
            jCollider["offset"] = collider.Offset.Serialize();
            jCollider["radius"] = attribute.Radius;
            return jCollider;
        }
        static JObject SerializerBoxCollider(ColliderObject collider)
        {
            var attribute = collider.Attribute as BoxColliderAttribute;
            var jCollider = new JObject();
            jCollider["type"] = "BOX";
            jCollider["frame_name"] = collider.FrameName;
            jCollider["offset"] = collider.Offset.Serialize();
            jCollider["width"] = attribute.Width;
            jCollider["height"] = attribute.Height;
            jCollider["depth"] = attribute.Depth;
            return jCollider;
        }
        static JContainer SaveColliderMeta(JObject jRoot, Dictionary<String, ColliderObject> colliderList)
        {
            var serializerTable = new Dictionary<Type, SerializerCollider> {

                { typeof(SphereColliderAttribute),SerializerSphereCollider },
                { typeof(BoxColliderAttribute), SerializerBoxCollider }
            };

            var jColliderList = new JObject(
                from pair in colliderList
                select new JProperty(pair.Key, serializerTable[pair.Value.Attribute.GetType()](pair.Value))
                );

            return jColliderList;
        }
        /// <summary>
        /// 애니메이션의 index와 키를 저장하는 테이블
        /// </summary>
        public Dictionary<int, String> AnimationTable
        {
            get => animationTable;
        }
        public Dictionary<String, ColliderObject> ColliderList
        {
            get => colliderList;
        }
        public string MeshFilePath { get; set; }
        public void Save(string filePath, string projectDir)
        {
            //상대적인 파일 경로를 구한다.
            Stream fileStream = null;
            StreamWriter streamWriter = null;
            var tokenWriter = new JTokenWriter();
            var jRoot = new JObject(new JProperty("mesh", Utility.ToRelativePath(projectDir, MeshFilePath)));
            var jAnims = new JObject();
            //File.Delete(filePath);
            if (File.Exists(filePath))
            {
                fileStream = File.Open(filePath, FileMode.Truncate, FileAccess.ReadWrite);
            }
            else
            {
                fileStream = File.Open(filePath, FileMode.OpenOrCreate, FileAccess.ReadWrite);
            }
            streamWriter = new StreamWriter(fileStream);
            foreach (var item in animationTable)
            {
                if(!jAnims.ContainsKey(item.Value))
                {
                    jAnims.Add(item.Value, item.Key);
                }
            }
            jRoot.Add("anim", jAnims);
            jRoot["collider"] = SaveColliderMeta(jRoot, ColliderList);
            jRoot["mesh"] = MapToolCore.Utility.ToRelativePath(projectDir, MeshFilePath);
            var serializer = JsonSerializer.CreateDefault();
            serializer.Serialize(streamWriter, jRoot);
            streamWriter.Close();
            streamWriter.Dispose();
        }

        public static MetaFile Load(string path, string projectDir)
        {
            Stream fileStream = null;
            fileStream = File.Open(path, FileMode.Open, FileAccess.Read);
            var serializer = JsonSerializer.CreateDefault();
            using (var steramReader = new StreamReader(fileStream))
            {
                var jsonTextReader = new JsonTextReader(steramReader);
                var jRoot = JObject.Load(jsonTextReader);
                return new MetaFile(jRoot, projectDir);
            }
        }
    }
}
