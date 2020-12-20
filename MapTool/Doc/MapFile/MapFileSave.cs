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
    public partial class MapFile
    {
        delegate JObject WriteObjectInJson(MapObject obj);
        JObject WriteWowObjMesh(MapObject obj) =>
            new JObject
            (
                new JProperty("type", "OBJ_MESH"),
                new JProperty("name", obj.Name),
                new JProperty("transform", (obj as WowMapMesh).Transform.Serialize()),
                new JProperty("usage", (obj as WowMapMesh).Usage.ToString().ToUpper()),
                new JProperty("path", Utility.FormatString( (obj as WowMapMesh).MeshFilePath) )
            );
        JObject WriteNaviMesh(MapObject obj)
        {
            var naviMesh = obj as NaviMesh;
            var indices = naviMesh.Indices;
            var vertices = naviMesh.Vertices;
            
            return new JObject
            (
                new JProperty("type", "NAVI_MESH"),
                new JProperty("name", obj.Name),
                new JProperty("transform", (obj as NaviMesh).Transform.Serialize()),
                new JProperty("vertices", new JArray(
                    from vertex in vertices select new JObject(
                            new JProperty("x", vertex.X),
                            new JProperty("y", vertex.Y),
                            new JProperty("z", vertex.Z)
                        )
                    )
                ),
                new JProperty("indices", new JArray(indices))
            );
        }
        JObject WriteTarget(MapObject obj) =>
            new JObject
            (
                new JProperty("type", "TARGET"),
                new JProperty("name", obj.Name),
                new JProperty("transform", (obj as RenderObject).Transform.Serialize())
            );
        public void Save()
        {
            var writeFunctionTable = new Dictionary<Type, WriteObjectInJson> {

                { typeof(WowMapMesh),WriteWowObjMesh },
                { typeof(NaviMesh), WriteNaviMesh },
                { typeof(TargetObject), WriteTarget }
            };
            Stream fileStream = null;
            StreamWriter streamWriter = null;
            var tokenWriter = new JTokenWriter();
            var jObj = new JObject(new JProperty("skybox", Utility.FormatString(this.skyboxTexturePath)) );
            var jObjects = new JArray();
            if (File.Exists(filePath))
            {
                fileStream = File.Open(filePath, FileMode.Truncate, FileAccess.ReadWrite);
            }
            else
            {
                fileStream = File.Open(filePath, FileMode.OpenOrCreate, FileAccess.ReadWrite);
            }
            streamWriter = new StreamWriter(fileStream);
            foreach(var obj in this.mapObjects)
            {
                var function = writeFunctionTable[obj.GetType()];
                if (function == null) continue;
                jObjects.Add(function(obj));
            }
            jObj.Add("objects", jObjects);
            var serializer = JsonSerializer.CreateDefault();
            serializer.Serialize(streamWriter, jObj);
            streamWriter.Close();
            streamWriter.Dispose();
        }
    }
}
