using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MapToolRender;
using System.IO;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
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
                new JProperty
                (
                    "transform",
                    new JObject
                    (
                        "position",
                        new JObject
                        (
                            new JProperty("x", (obj as WowMapMesh).Transform.Position.X),
                            new JProperty("y", (obj as WowMapMesh).Transform.Position.Y),
                            new JProperty("z", (obj as WowMapMesh).Transform.Position.Z)
                        ),
                        "scale",
                        new JObject
                        (
                            new JProperty("x", (obj as WowMapMesh).Transform.Scale.X),
                            new JProperty("y", (obj as WowMapMesh).Transform.Scale.Y),
                            new JProperty("z", (obj as WowMapMesh).Transform.Scale.Z)
                        ),
                        "rotations",
                        new JObject
                        (
                            new JProperty("x", (obj as WowMapMesh).Transform.Rotation.X),
                            new JProperty("y", (obj as WowMapMesh).Transform.Rotation.Y),
                            new JProperty("z", (obj as WowMapMesh).Transform.Rotation.Z)
                        )
                    )
                ),
                new JProperty("path", this.FormatString( (obj as WowMapMesh).MeshFilePath) )
            );
        JObject WriteNaviMesh(MapObject obj)
        {
            var naviMesh = obj as NaviMesh;
            
            return null;
        }
        public void Save(string filePath)
        {
            var writeFunctionTable = new Dictionary<Type, WriteObjectInJson> {

                { typeof(WowMapMesh),WriteWowObjMesh }
            };
            Stream fileStream = null;
            StreamWriter streamWriter = null;
            var tokenWriter = new JTokenWriter();
            var jObj = new JObject(new JProperty("skybox", this.FormatString(this.skyboxTexturePath)) );
            var jObjects = new JArray();
            fileStream = File.OpenWrite(filePath);
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
