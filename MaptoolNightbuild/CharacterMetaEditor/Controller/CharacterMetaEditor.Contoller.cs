using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MaptoolNightbuild.CharacterMetaEditor
{
    class Controller
    {
        private static Controller s_instance = new Controller();
        public static Controller Instance{ get => s_instance; }
        public void Save()
        {
            var document = Doc.CharacterMetaDoc.Instance;

        }

    }
}
