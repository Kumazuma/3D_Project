#pragma once
#include"MapToolRender.h"
#include"Texture.h"
namespace MapToolRender
{
    using TypeConverter = System::ComponentModel::TypeConverter;
    using ExpandableObjectConverter = System::ComponentModel::ExpandableObjectConverter;
    using ICustomTypeDescriptor = System::ComponentModel::ICustomTypeDescriptor;
    using INotifyCollectionChanged = System::Collections::Specialized::INotifyCollectionChanged;
    using NotifyCollectionChangedEventArgs = System::Collections::Specialized::NotifyCollectionChangedEventArgs;
    using namespace System::Collections::Specialized;
    template<typename T>
    using ICollection = System::Collections::Generic::ICollection<T>;
    template<typename T>
    using IEnumerable = System::Collections::Generic::IEnumerable<T>;

    [TypeConverter(ExpandableObjectConverter::typeid)]
    public ref class TextureCollection :
        ICollection<Texture^>,
        IEnumerable<Texture^>,
        ICustomTypeDescriptor,
        INotifyCollectionChanged
    {
    private:
        array<Texture^>^ m_textures;
    public:
        TextureCollection(int count)
        {
            m_textures = gcnew array<Texture^>(count);
        }
        property Texture^ default[int]
        {
            auto get(int index)->Texture^
            {
                return m_textures[index];
            }
            auto set(int index, Texture ^ value)->void
            {
                auto oldObj {m_textures[index] };
                auto newObj{ value };
                m_textures[index] = value;
                auto arg = gcnew NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction::Replace, oldObj, newObj, index);
                CollectionChanged(this, arg);
            }
        }
        property int Count
        {
            virtual auto get()->int  { return m_textures->Length; }
        }
        property bool IsReadOnly 
        {
            virtual auto get()->bool  { return false; }
        }
        virtual auto Add(Texture^)->void  {}
        virtual auto Clear()->void  {}
        virtual auto Contain(Texture^ tex)->bool  = System::Collections::Generic::ICollection<Texture^>::Contains{
            auto t = static_cast<System::Collections::IList^>(m_textures);
            return t->Contains(tex); }
        virtual auto CopyTo(array<Texture^>^ arr, int arrayIndex)->void  { m_textures->CopyTo(arr, arrayIndex); }
        virtual auto GetAttributes()->AttributeCollection^  { return TypeDescriptor::GetAttributes(this, true); }
        virtual auto GetClassName()->System::String^  { return TypeDescriptor::GetClassName(this, true); }
        virtual auto GetComponentName()->System::String^  { return TypeDescriptor::GetComponentName(this, true); }
        virtual auto GetConverter()->TypeConverter^  { return TypeDescriptor::GetConverter(this, true); }
        
        virtual EventDescriptor^ GetDefaultEvent() {
            return TypeDescriptor::GetDefaultEvent(this, true);
        }
        virtual PropertyDescriptor^ GetDefaultProperty() {
            return TypeDescriptor::GetDefaultProperty(this, true);
        }
        virtual System::Object^ GetEditor(Type^ editorBaseType)  {
            return TypeDescriptor::GetEditor(this, editorBaseType, true);
        }

        virtual IEnumerator<Texture^>^ GetEnumerator()  {
            return static_cast<IEnumerator<Texture^>^>(m_textures->GetEnumerator());
        }
        virtual System::Collections::IEnumerator^ EnumerableGetEnumerator() = System::Collections::IEnumerable::GetEnumerator 
        {
            return m_textures->GetEnumerator();
        }
        virtual EventDescriptorCollection^ GetEvents() {
            return TypeDescriptor::GetEvents(this, true);
        }
        virtual EventDescriptorCollection^ GetEvents(array<Attribute^>^ attributes)  {
            return TypeDescriptor::GetEvents(this, attributes, true);
        }
        virtual System::Object^ GetPropertyOwner(PropertyDescriptor^ pd)  {
            return this;
        }
        virtual PropertyDescriptorCollection^ GetProperties(array<Attribute^>^ attributes) {
            return GetProperties();
        }
        virtual PropertyDescriptorCollection^ GetProperties() ;
        virtual bool Remove(Texture^ item)
        {
            return false;
        }

        // INotifyCollectionChanged을(를) 통해 상속됨
        virtual event System::Collections::Specialized::NotifyCollectionChangedEventHandler^ CollectionChanged;
    };
    public ref class TextureCollectionItemDescriptor : public PropertyDescriptor
    {
        int index;
        TextureCollection^ collection;
    public:
        TextureCollectionItemDescriptor(TextureCollection^ collection, int index) :
            PropertyDescriptor(String::Format("[{0}]", index), nullptr)
        {
            this->collection = collection;
            this->index = index;
        }

        property Type^ ComponentType
        {
            auto get()->Type^ override {
                return this->collection->GetType();
            }
        }
        property bool IsReadOnly
        {
            auto get()->bool override { return false; }
        }
        property Type^ PropertyType
        {
            auto get()->Type^ override { return Texture::typeid; }
        }
        auto CanResetValue(System::Object^ component)->bool override { return true; }
        auto GetValue(System::Object^ component)->System::Object^ override { return this->collection[index]; }
        auto ResetValue(System::Object^)->void override {}
        property System::String^ Description
        {
            auto get()->System::String^ override { return System::String::Format("[{0}]", index); }
        }
        auto SetValue(System::Object^, System::Object^ value)->void override
        {
            this->collection[index] = dynamic_cast<Texture^>(value);

        }
        auto ShouldSerializeValue(System::Object^)->bool override
        {
            return true;
        }
    };


    inline PropertyDescriptorCollection^ TextureCollection::GetProperties()
    {
        PropertyDescriptorCollection^ pds = gcnew PropertyDescriptorCollection(nullptr);
        int len = m_textures->Length;
        // Iterate the list of employees
        for (int i = 0; i < len; i++)
        {
            // Create a property descriptor for the employee item and add to the property descriptor collection
            TextureCollectionItemDescriptor^ pd = gcnew TextureCollectionItemDescriptor(this, i);
            pds->Add(pd);
        }
        // return the property descriptor collection
        return pds;
    }
}