using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MapToolRender;
using System.IO;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using MapToolCore;

namespace MapTool.Doc
{
    
    
    /// <summary>
    /// 애니메이션 메시에 대한 메타 데이터
    /// </summary>
    partial class AnimationMeshMeta
    {
        AnimationTable animationTable = new AnimationTable();
        ColliderList colliderList = new ColliderList();
        public AnimationMeshMeta()
        {

        }
        private AnimationMeshMeta(JObject jObj)
        {
            if (!jObj.ContainsKey("mesh")) throw new Exception("has no mesh attribute");
            if(jObj["mesh"].Type != JTokenType.String) throw new Exception("has no mesh attribute");
            MeshFilePath = System.IO.Path.Combine(MapToolCore.Environment.Instance.ProjectDirectory, jObj.Value<string>("mesh"));
            if (!jObj.ContainsKey("anim")) throw new Exception("has no anim attribute");
            if(jObj["anim"].Type != JTokenType.Object) throw new Exception("has no anim attribute");
            var anims = jObj["anim"] as JObject;
            foreach(var item in anims)
            {
                uint index = item.Value.Value<uint>();
                animationTable.Add(new AnimationTableItem(index, item.Key) );
            }
            
            if (!jObj.ContainsKey("collider")) throw new Exception("has no collider attribute");
            var jColliders = jObj["collider"];
            if(jColliders.Type != JTokenType.Array) throw new Exception("has no collider attribute");
            foreach(var item in jColliders)
            {
                var collider = new Collider();
                collider.FrameName = item.Value<string>("frame_name");
                collider.Offset = Offset.Parse(item["offset"] as JObject);
                collider.Transform = Transform.Parse(item["transform"] as JObject);
                switch (item.Value<string>("type"))
                {
                    case "BOX":
                        collider.Type = ColliderType.Box;
                        {
                            var attribute = collider.Attribute as BoxColliderAttribute;
                            attribute.Depth = item.Value<float>("depth");
                            attribute.Height = item.Value<float>("height");
                            attribute.Width= item.Value<float>("width");
                        }
                        break;
                    case "SPHARE":
                        collider.Type = ColliderType.Sphere;
                        {
                            var attribute = collider.Attribute as SphereColliderAttribute;
                            attribute.Radius= item.Value<float>("radius");
                        }
                        break;
                }
                colliderList.Add(collider);
            }
        }
        /// <summary>
        /// 애니메이션의 index와 키를 저장하는 테이블
        /// </summary>
        public AnimationTable AnimationTable
        {
            get => animationTable;
        }
        public ColliderList ColliderList
        {
            get => colliderList;
        }
        public string MeshFilePath { get; set; }
        public void Save(string filePath)
        {
            var projectdir = MapToolCore.Environment.Instance.ProjectDirectory;
            //상대적인 파일 경로를 구한다.
            Stream fileStream = null;
            StreamWriter streamWriter = null;
            var tokenWriter = new JTokenWriter();
            var jRoot = new JObject(new JProperty("mesh", Utility.FormatString(MeshFilePath)));
            var jAnims = new JObject();
            //File.Delete(filePath);
            if(File.Exists(filePath))
            {
                fileStream = File.Open(filePath, FileMode.Truncate, FileAccess.ReadWrite);
            }
            else
            {
                fileStream = File.Open(filePath, FileMode.OpenOrCreate, FileAccess.ReadWrite);
            }
            streamWriter = new StreamWriter(fileStream);
            foreach(var item in animationTable)
            {
                jAnims.Add(item.ID, item.Index);
            }
            jRoot.Add("anim", jAnims);
            jRoot["collider"] = SaveColliderMeta(jRoot, ColliderList);
            var serializer = JsonSerializer.CreateDefault();
            serializer.Serialize(streamWriter, jRoot);
            streamWriter.Close();
            streamWriter.Dispose();
        }
        public static AnimationMeshMeta Load(string path)
        {
            Stream fileStream = null;
            fileStream = File.Open(path, FileMode.Open, FileAccess.Read);
            var serializer = JsonSerializer.CreateDefault();
            using(var steramReader = new StreamReader(fileStream))
            {
                var jsonTextReader = new JsonTextReader(steramReader);
                var jRoot = JObject.Load(jsonTextReader);
                return new AnimationMeshMeta(jRoot);
            }
        }
    }
}