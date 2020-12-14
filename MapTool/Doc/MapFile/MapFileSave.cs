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
                new JProperty("transform",
                    new JObject(
                        new JProperty("position",
                            new JObject(
                                new JProperty("x", (obj as WowMapMesh).Transform.Position.X),
                                new JProperty("y", (obj as WowMapMesh).Transform.Position.Y),
                                new JProperty("z", (obj as WowMapMesh).Transform.Position.Z))),
                        new JProperty("scale",
                            new JObject(
                            new JProperty("x", (obj as WowMapMesh).Transform.Scale.X),
                            new JProperty("y", (obj as WowMapMesh).Transform.Scale.Y),
                            new JProperty("z", (obj as WowMapMesh).Transform.Scale.Z))),
                        new JProperty("rotation",
                            new JObject(
                                new JProperty("x", (obj as WowMapMesh).Transform.Rotation.X),
                                new JProperty("y", (obj as WowMapMesh).Transform.Rotation.Y),
                                new JProperty("z", (obj as WowMapMesh).Transform.Rotation.Z)))
                    )
                ),
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
                new JProperty("transform",
                    new JObject(
                        new JProperty("position",
                            new JObject(
                                new JProperty("x", (obj as NaviMesh).Transform.Position.X),
                                new JProperty("y", (obj as NaviMesh).Transform.Position.Y),
                                new JProperty("z", (obj as NaviMesh).Transform.Position.Z))),
                        new JProperty("scale",
                            new JObject(
                            new JProperty("x", (obj as NaviMesh).Transform.Scale.X),
                            new JProperty("y", (obj as NaviMesh).Transform.Scale.Y),
                            new JProperty("z", (obj as NaviMesh).Transform.Scale.Z))),
                        new JProperty("rotation",
                            new JObject(
                                new JProperty("x", (obj as NaviMesh).Transform.Rotation.X),
                                new JProperty("y", (obj as NaviMesh).Transform.Rotation.Y),
                                new JProperty("z", (obj as NaviMesh).Transform.Rotation.Z)))
                    )
                ),
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
        public void Save()
        {
            var writeFunctionTable = new Dictionary<Type, WriteObjectInJson> {

                { typeof(WowMapMesh),WriteWowObjMesh },
                { typeof(NaviMesh), WriteNaviMesh }
            };
            Stream fileStream = null;
            StreamWriter streamWriter = null;
            var tokenWriter = new JTokenWriter();
            var jObj = new JObject(new JProperty("skybox", Utility.FormatString(this.skyboxTexturePath)) );
            var jObjects = new JArray();
            fileStream = File.Open(filePath, FileMode.OpenOrCreate, FileAccess.ReadWrite);
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
