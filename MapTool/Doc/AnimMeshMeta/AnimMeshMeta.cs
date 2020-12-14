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
            fileStream = File.Open(filePath, FileMode.OpenOrCreate, FileAccess.ReadWrite);
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
        public static AnimationTable Load(string path)
        {
            return null;
        }
    }
}