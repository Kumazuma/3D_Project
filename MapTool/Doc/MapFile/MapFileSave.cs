using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MapToolRender;
using System.IO;
using Newtonsoft.Json;
namespace MapTool.Doc
{
    public partial class MapFile
    {
        public void Save(string filePath)
        {
            Stream fileStream = null;
            StreamWriter streamWriter = null;
            JsonTextWriter jsonWriter = null;
            fileStream = File.OpenWrite(filePath);
            streamWriter = new StreamWriter(fileStream);
            jsonWriter = new JsonTextWriter(streamWriter);

            jsonWriter.WriteStartObject();
            jsonWriter.WritePropertyName("skybox");



            jsonWriter.WriteEndObject();
            jsonWriter.Close();
        }
    }
}
