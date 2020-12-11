using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MapToolRender;
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

        }
        public static AnimationTable Load(string path)
        {
            return null;
        }
    }
}