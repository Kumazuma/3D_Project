using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MaptoolRenderer;
using Newtonsoft.Json.Linq;
using MapToolCore;
using System.IO;
using Newtonsoft.Json;

namespace MaptoolNightbuild.MapEditor
{
    class MapFile
    {
        public class Collection :
            ICollection<Object>,
            IEnumerable<Object>
        {
            MapFile mapFile;
            public Collection(MapFile mapFile) => this.mapFile = mapFile;
            public int Count => mapFile.mapObjects.Count;
            public bool IsReadOnly => false;
            public void Add(Object item) => mapFile.mapObjects.Add(item);
            public void Clear() => mapFile.mapObjects.Clear();
            public bool Contains(Object item) => mapFile.mapObjects.Contains(item);
            public void CopyTo(Object[] array, int arrayIndex) => mapFile.mapObjects.CopyTo(array, arrayIndex);
            public IEnumerator<Object> GetEnumerator() => mapFile.mapObjects.GetEnumerator();
            public bool Remove(Object item) => mapFile.mapObjects.Remove(item);
            System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() => mapFile.mapObjects.GetEnumerator();
        }

        List<Object> mapObjects = new List<Object>();
        Collection collection;
        string projectPath;
        string skyboxTexturePath;
        public MapFile()
        {
            collection = new Collection(this);
        }
        private MapFile(JObject jObj, string projectDir)
        {
            collection = new Collection(this);

            if (!jObj.ContainsKey("skybox")) throw new Exception("has no skybox attribute");
            var skyBox = jObj["skybox"];
            if (skyBox.Type != JTokenType.String) throw new Exception("has no skybox attribute");
            if(projectDir.Last<char>() == '\\' || projectDir.Last<char>() == '/')
            {
                projectDir = projectDir.Remove(projectDir.Length - 1);
            }
            skyboxTexturePath = projectDir + skyBox.Value<string>();
            if (!jObj.ContainsKey("objects")) throw new Exception("has no objects attribute");
            var objectsToken = jObj["objects"];
            if (objectsToken.Type != JTokenType.Array) throw new Exception("has no objects attribute");
            var objects = objectsToken as JArray;
            var taskList = new List<Task<object>>();
            foreach (JToken token in objects)
            {
                string name = null;
                string type_ = null;
                string path = null;
                Transform transform;

                if (token.Type != JTokenType.Object) throw new Exception("token in objects array is not object");
                var meshObj = token as JObject;

                if (!meshObj.ContainsKey("type")) throw new Exception("object has no 'type' attribute");
                var typeToken = meshObj["type"];
                if (typeToken.Type != JTokenType.String) throw new Exception("type attribute's type is not string");
                type_ = typeToken.Value<string>();

                if (!meshObj.ContainsKey("transform")) throw new Exception("has no objects attribute");
                var transformToken = meshObj["transform"];
                if (transformToken.Type != JTokenType.Object) throw new Exception("has no objects attribute");
                var jTransform = transformToken as JObject;
                transform = Transform.Parse(jTransform);

                if (!meshObj.ContainsKey("name")) throw new Exception("has no objects attribute");
                var nameToken = meshObj["name"];
                if (nameToken.Type != JTokenType.String) throw new Exception("has no objects attribute");
                name = nameToken.Value<string>();

                if (meshObj.ContainsKey("path"))
                {
                    var pathToken = meshObj["path"];
                    if (pathToken.Type != JTokenType.String) throw new Exception("has no objects attribute");
                    path = pathToken.Value<string>();
                }
                Task<object> task = null;
                switch (type_)
                {
                    case "OBJ_MESH":
                        {
                            OBJMeshObjectUsage usage = OBJMeshObjectUsage.Terrain;
                            if (meshObj.ContainsKey("usage"))
                            {
                                var usageToken = meshObj["usage"];
                                if (usageToken.Type == JTokenType.String)
                                {
                                    switch (usageToken.Value<string>().ToUpper())
                                    {
                                        case "TERRAIN":
                                            usage = OBJMeshObjectUsage.Terrain;
                                            break;
                                        case "STRUCTURE":
                                            usage = OBJMeshObjectUsage.Structure;
                                            break;
                                    }
                                }

                            }
                            var kinds = new System.Collections.Generic.Dictionary<String, OBJSubsetRenderKind>();
                            if (meshObj.ContainsKey("subsets"))
                            {
                                foreach(var item in meshObj["subsets"] as JObject)
                                {
                                    OBJSubsetRenderKind kind = OBJSubsetRenderKind.Deferred;
                                    switch(item.Value.Value<String>())
                                    {
                                        case "DEFERRED":
                                            kind = OBJSubsetRenderKind.Deferred;
                                            break;
                                        case "ALPHATEST":
                                            kind = OBJSubsetRenderKind.AlphaTest;
                                            break;
                                    }
                                    kinds.Add(item.Key, kind);
                                }
                                
                            }
                            var ss = Task<object>.Run(() =>
                            {
                                path = projectDir + path;
                                path = System.IO.Path.GetFullPath(path);
                                var mesh = Doc.ResourceManager.Instance.GetObjMesh(path).Result;
                                if (mesh != null)
                                {
                                    MaptoolRenderer.OBJObject objMesh = new OBJObject(mesh);
                                    objMesh.Transform = transform;
                                    objMesh.Name = name;
                                    objMesh.Usage = usage;
                                    return objMesh as object;
                                }
                                return null;
                            });
                            task = ss;
                        }
                        taskList.Add(task);
                        break;
                    case "TARGET":
                        {
                            var target = new MapTarget();
                            target.Transform = transform;
                            target.Name = name;
                            mapObjects.Add(target);
                        }
                        break;
                }
            }
            foreach (var task in taskList)
            {
                mapObjects.Add(task.Result);
            }
        }
        public Collection Nodes
        {
            get => collection;
        }
        public string ProjectPath
        {
            get => projectPath;
            set => projectPath = value;
        }
        public string SkyBoxTexturePath
        {
            get => skyboxTexturePath;
            set => skyboxTexturePath = value;
        }
        delegate JObject WriteObjectInJson(Object obj);
        JObject WriteObjMesh(Object obj_)
        {
            var meshObject = obj_ as OBJObject;
            var jObj = new JObject();
            var renderKinds = new JObject();
            foreach(var subset  in meshObject.Subsets)
            {
                renderKinds.Add(subset.Name, subset.RenderKind.ToString().ToUpper());
            }
            jObj.Add("type", "OBJ_MESH");
            jObj.Add("name", meshObject.Name);
            jObj.Add("subsets", renderKinds);
            jObj.Add("path", Utility.ToRelativePath(ProjectPath, meshObject.Mesh.FilePath));
            jObj.Add("transform", meshObject.Transform.Serialize());
            jObj.Add("usage", meshObject.Usage.ToString().ToUpper());
            return jObj;
        }
        JObject WriteTarget(Object obj)
        {
            var meshObject = obj as MapTarget;
            return new JObject
            (
                new JProperty("type", "TARGET"),
                new JProperty("name", meshObject.Name),
                new JProperty("transform", meshObject.Transform.Serialize())
            ); 
        }
        public async Task Save(string path)
        {
            Action action = new Action(()=>
            {
                var writeFunctionTable =
                new Dictionary<Type, WriteObjectInJson> {

                    { typeof(OBJObject),WriteObjMesh },
                    { typeof(MapTarget), WriteTarget }
                };
                Stream fileStream = null;
                StreamWriter streamWriter = null;
                var tokenWriter = new JTokenWriter();
                var jObj = new JObject(new JProperty("skybox", Utility.ToRelativePath(ProjectPath, this.skyboxTexturePath)));
                if (File.Exists(path))
                {
                    fileStream = File.Open(path, FileMode.Truncate, FileAccess.ReadWrite);
                }
                else
                {
                    fileStream = File.Open(path, FileMode.OpenOrCreate, FileAccess.ReadWrite);
                }
                streamWriter = new StreamWriter(fileStream);
                var jObjects = new JArray();
                foreach (var obj in this.mapObjects)
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
            });
            await Task.Run(action);
        }
        public static async Task<MapFile> Load(string filePath, string projectDir)
        {
            return await Task<MapFile>.Factory.StartNew(() =>
            {
                using (var fileStream = File.Open(filePath, FileMode.Open, FileAccess.Read))
                {
                    var streamReader = new StreamReader(fileStream);
                    var tokenReader = new JsonTextReader(streamReader);
                    var root = JObject.Load(tokenReader);
                    return new MapFile(root, projectDir) ;
                }
            });
        }
    }
}
