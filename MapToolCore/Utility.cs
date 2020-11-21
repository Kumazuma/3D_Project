using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.InteropServices;

namespace MapToolCore
{
    

    public class Utility
    {
        /// <summary>
        /// Creates a relative path from one file or folder to another.
        /// </summary>
        /// <param name="fromPath">Contains the directory that defines the start of the relative path.</param>
        /// <param name="toPath">Contains the path that defines the endpoint of the relative path.</param>
        /// <returns>The relative path from the start directory to the end path or <c>toPath</c> if the paths are not related.</returns>
        /// <exception cref="ArgumentNullException"></exception>
        /// <exception cref="UriFormatException"></exception>
        /// <exception cref="InvalidOperationException"></exception>
        public static String MakeRelativePath(String fromPath, String toPath)
        {
            if (String.IsNullOrEmpty(fromPath)) throw new ArgumentNullException("fromPath");
            if (String.IsNullOrEmpty(toPath)) throw new ArgumentNullException("toPath");

            Uri fromUri = new Uri(fromPath);
            Uri toUri = new Uri(toPath);

            if (fromUri.Scheme != toUri.Scheme) { return toPath; } // path can't be made relative.

            Uri relativeUri = fromUri.MakeRelativeUri(toUri);
            String relativePath = Uri.UnescapeDataString(relativeUri.ToString());

            if (toUri.Scheme.Equals("file", StringComparison.InvariantCultureIgnoreCase))
            {
                relativePath = relativePath.Replace(Path.AltDirectorySeparatorChar, Path.DirectorySeparatorChar);
            }

            return relativePath;
        }
        public static string GetRelativePath(string fromPath, string toPath)
        {
            int fromAttr = GetPathAttribute(fromPath);
            int toAttr = GetPathAttribute(toPath);

            StringBuilder path = new StringBuilder(260); // MAX_PATH
            if (PathRelativePathTo(
                path,
                fromPath,
                fromAttr,
                toPath,
                toAttr) == 0)
            {
                throw new ArgumentException("Paths must have a common prefix");
            }
            return path.ToString();
        }

        private static int GetPathAttribute(string path)
        {
            DirectoryInfo di = new DirectoryInfo(path);
            if (di.Exists)
            {
                return FILE_ATTRIBUTE_DIRECTORY;
            }

            FileInfo fi = new FileInfo(path);
            if (fi.Exists)
            {
                return FILE_ATTRIBUTE_NORMAL;
            }

            throw new FileNotFoundException();
        }

        private const int FILE_ATTRIBUTE_DIRECTORY = 0x10;
        private const int FILE_ATTRIBUTE_NORMAL = 0x80;
        [DllImport("shlwapi.dll", SetLastError = true)]
        private static extern int PathRelativePathTo(StringBuilder pszPath,
            string pszFrom, int dwAttrFrom, string pszTo, int dwAttrTo);

    }
}
