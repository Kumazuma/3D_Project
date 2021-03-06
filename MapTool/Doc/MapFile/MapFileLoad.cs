﻿using System;
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
        private MapFile(JObject jObj)
        {
            collection = new Collection(this);

            string projectDir = MapToolCore.Environment.Instance.ProjectDirectory;
            if (!jObj.ContainsKey("skybox")) throw new Exception("has no skybox attribute");
            var skyBox = jObj["skybox"];
            if(skyBox.Type != JTokenType.String) throw new Exception("has no skybox attribute");
            SkyBoxTexturePath = System.IO.Path.Combine(projectDir, skyBox.Value<string>());
            if (!jObj.ContainsKey("objects")) throw new Exception("has no objects attribute");
            var objectsToken = jObj["objects"];
            if (objectsToken.Type != JTokenType.Array) throw new Exception("has no objects attribute");
            var objects = objectsToken as JArray;
            var taskList = new List<Task<RenderObject> >();
            foreach(JToken token in objects)
            {
                string name = null;
                string type_ = null;
                string path = null;
                Transform transform;
                
                if(token.Type != JTokenType.Object) throw new Exception("token in objects array is not object");
                var meshObj = token as JObject;
                
                if (!meshObj.ContainsKey("type")) throw new Exception("object has no 'type' attribute");
                var typeToken = meshObj["type"];
                if (typeToken.Type != JTokenType.String) throw new Exception("type attribute's type is not string");
                type_ = typeToken.Value<string>();

                if (!meshObj.ContainsKey("transform")) throw new Exception("has no objects attribute");
                var transformToken = meshObj["transform"];
                if(transformToken.Type !=  JTokenType.Object) throw new Exception("has no objects attribute");
                var jTransform = transformToken as JObject;
                transform = Transform.Parse(jTransform);
                
                if(!meshObj.ContainsKey("name")) throw new Exception("has no objects attribute");
                var nameToken = meshObj["name"];
                if (nameToken.Type != JTokenType.String) throw new Exception("has no objects attribute");
                name = nameToken.Value<string>();
                
                if(meshObj.ContainsKey("path"))
                {
                    var pathToken = meshObj["path"];
                    if(pathToken.Type != JTokenType.String) throw new Exception("has no objects attribute");
                    path = pathToken.Value<string>();
                }
                Task<RenderObject> task = null;
                switch(type_)
                {
                    case "OBJ_MESH":
                        {
                            Usage usage = Usage.None;
                            if (meshObj.ContainsKey("usage"))
                            {
                                var usageToken = meshObj["usage"];
                                if (usageToken.Type == JTokenType.String)
                                {
                                    switch (usageToken.Value<string>().ToUpper())
                                    {
                                        case "NONE":
                                            usage = Usage.None;
                                            break;
                                        case "TERRAIN":
                                            usage = Usage.Terrain;
                                            break;
                                        case "STRUCTURE":
                                            usage = Usage.Structure;
                                            break;
                                        case "FOILAGE":
                                            usage = Usage.Foliage;
                                            break;
                                    }

                                }
                            }
                            var ss = Task<RenderObject>.Run(() =>
                            {
                                path = projectDir + path;
                                path = System.IO.Path.GetFullPath(path);
                                var mesh = ResourceManager.Instance.GetObjMesh(path).Result;
                                if (mesh != null)
                                {
                                    mesh.Transform = transform;
                                    mesh.Name = name;
                                    mesh.Usage = usage;
                                    return mesh as RenderObject;
                                }
                                return null;
                            });
                            task = ss;
                        }
                        taskList.Add(task);
                        break;
                    case "TARGET":
                        {
                            var target = ResourceManager.Instance.GetTargetObject();
                            target.Transform = transform;
                            target.Name = name;
                            mapObjects.Add(target);
                        }
                        break;
                }
                //var nameToken = 
            }
            foreach(var task in taskList)
            {
                mapObjects.Add(task.Result);
            }
        }
        public static MapFile Load(string filePath)
        {
            
            using(var fileStream = File.Open(filePath, FileMode.Open, FileAccess.Read))
            {
                var streamReader = new StreamReader(fileStream);
                var tokenReader = new JsonTextReader(streamReader);
                var root = JObject.Load(tokenReader);
                return new MapFile(root) { filePath = filePath };
            }
        }
    }
}
