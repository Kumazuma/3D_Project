#pragma once
#include"OBJObject.hpp"
namespace MaptoolRenderer
{
    using namespace System::ComponentModel;
    using INotifyCollectionChanged = System::Collections::Specialized::INotifyCollectionChanged;
    using NotifyCollectionChangedEventArgs = System::Collections::Specialized::NotifyCollectionChangedEventArgs;
    using namespace System::Collections::Specialized;
    template<typename T>
    using ICollection = System::Collections::Generic::ICollection<T>;
    template<typename T>
    using IEnumerable = System::Collections::Generic::IEnumerable<T>;
    
	ref class OBJObject;
    [TypeConverter(ExpandableObjectConverter::typeid)]
	public ref class OBJSubsetCollection:
		public System::ComponentModel::INotifyPropertyChanged,
        public ICustomTypeDescriptor
	{
	internal:
		OBJSubsetCollection(OBJObject^ objObject, IList<OBJSubset^>^ subsets);
	public:
		// INotifyPropertyChanged을(를) 통해 상속됨
		virtual event System::ComponentModel::PropertyChangedEventHandler^ PropertyChanged;
        auto ToString()->System::String^ override;

	private:
		System::WeakReference<OBJObject^>^ obj_;
        IList<OBJSubset^>^ subsets_;
    public:
        // ICustomTypeDescriptor을(를) 통해 상속됨
        virtual System::ComponentModel::AttributeCollection^ GetAttributes();
        virtual System::String^ GetClassName();
        virtual System::String^ GetComponentName();
        virtual System::ComponentModel::TypeConverter^ GetConverter();
        virtual System::ComponentModel::EventDescriptor^ GetDefaultEvent();
        virtual System::ComponentModel::PropertyDescriptor^ GetDefaultProperty();
        virtual System::Object^ GetEditor(System::Type^ editorBaseType);
        virtual System::ComponentModel::EventDescriptorCollection^ GetEvents();
        virtual System::ComponentModel::EventDescriptorCollection^ GetEvents(array<System::Attribute^, 1>^ attributes);
        virtual System::ComponentModel::PropertyDescriptorCollection^ GetProperties();
        virtual System::ComponentModel::PropertyDescriptorCollection^ GetProperties(array<System::Attribute^, 1>^ attributes);
        virtual System::Object^ GetPropertyOwner(System::ComponentModel::PropertyDescriptor^ pd);
    };
    public ref class OBJSubsetCollectionItemDescriptor : public PropertyDescriptor
    {
        OBJSubset^ subset_;
    public:
        OBJSubsetCollectionItemDescriptor(OBJSubset^ subset):
            PropertyDescriptor(subset->Name, nullptr),
            subset_{ subset }
        {

        }
        property Type^ ComponentType
        {
            auto get()->Type^ override {
                return OBJSubsetCollection::typeid;
            }
        }
        property bool IsReadOnly
        {
            auto get()->bool override { return false; }
        }
        property Type^ PropertyType
        {
            auto get()->Type^ override { return OBJSubsetRenderKind::typeid; }
        }
        auto CanResetValue(System::Object^ component)->bool override { return true; }
        auto GetValue(System::Object^ component)->System::Object^ override { return subset_->RenderKind; }
        auto ResetValue(System::Object^)->void override {}
        property System::String^ Description
        {
            auto get()->System::String^ override { return subset_->Name; }
        }
        auto SetValue(System::Object^, System::Object^ value)->void override
        {
            subset_->RenderKind = safe_cast<OBJSubsetRenderKind>(value);
        }
        auto ShouldSerializeValue(System::Object^)->bool override
        {
            return false;
        }
    };


}