using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
namespace MaptoolNightbuild.CharacterMetaEditor.View
{
    [TypeConverterAttribute(typeof(ComboBoxDataConverter))]
    public class ComboBoxDataType
    {
        public ComboBoxDataType() { }
        public String Text => value.ToString();
        public int Value { get => value; set => this.value = value; }
        int value;

    }
    public class ComboBoxDataConverter:
        Int32Converter
    {
        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            if(sourceType == typeof(int)) return true;
            if (sourceType == typeof(String)) return true;
            return false;
        }
        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(int)) return true;
            if (destinationType == typeof(String)) return true;
            return false;
        }
        public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
        {
            if (value.GetType() == typeof(int))
            {
                var item = new ComboBoxDataType();
                item.Value = (int)value;
                return item;
            }
            if (value.GetType() == typeof(String))
            {
                var item = new ComboBoxDataType();
                item.Value = int.Parse(value as String);
                return item;
            }
            return null;
        }
        public override object ConvertTo(ITypeDescriptorContext context, CultureInfo culture, object value, Type destinationType)
        {
            if(value.GetType() == destinationType)
            {
                return value;
            }

            var obj = value as ComboBoxDataType;
            
            if (destinationType == typeof(int))
            {
                if(value.GetType() == typeof(String))
                {
                    return int.Parse(value as String);
                }
                return obj.Value;
            }
            if (destinationType == typeof(String))
            {
                if (value.GetType() == typeof(int))
                {
                    return value.ToString();
                }
                return obj.Text;
            }
            return null;
        }
        public override object CreateInstance(ITypeDescriptorContext context, IDictionary propertyValues)
        {
            return new ComboBoxDataType();
        }
        public override bool GetCreateInstanceSupported(ITypeDescriptorContext context)
        {
            return true;
        }
    }
}
