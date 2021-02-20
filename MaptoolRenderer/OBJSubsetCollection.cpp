#include "pch.h"
#include "OBJObject.hpp"
#include "OBJSubsetCollection.hpp"
MaptoolRenderer::OBJSubsetCollection::OBJSubsetCollection(OBJObject^ objObject, IList<OBJSubset^>^ subsets)
{
	obj_ = gcnew System::WeakReference<OBJObject^>{objObject};
	subsets_ = subsets;
}

auto MaptoolRenderer::OBJSubsetCollection::ToString() -> System::String^
{
	return String::Format("Count={0}", subsets_->Count);
}

System::ComponentModel::AttributeCollection^ MaptoolRenderer::OBJSubsetCollection::GetAttributes()
{
	return TypeDescriptor::GetAttributes(this, true);
}

System::String^ MaptoolRenderer::OBJSubsetCollection::GetClassName()
{
	return TypeDescriptor::GetClassName(this, true);
}

System::String^ MaptoolRenderer::OBJSubsetCollection::GetComponentName()
{
	return TypeDescriptor::GetComponentName(this, true);
}

System::ComponentModel::TypeConverter^ MaptoolRenderer::OBJSubsetCollection::GetConverter()
{
	return TypeDescriptor::GetConverter(this, true);
}

System::ComponentModel::EventDescriptor^ MaptoolRenderer::OBJSubsetCollection::GetDefaultEvent()
{
	return TypeDescriptor::GetDefaultEvent(this, true);
}

System::ComponentModel::PropertyDescriptor^ MaptoolRenderer::OBJSubsetCollection::GetDefaultProperty()
{
	return TypeDescriptor::GetDefaultProperty(this, true);
}

System::Object^ MaptoolRenderer::OBJSubsetCollection::GetEditor(System::Type^ editorBaseType)
{
	return TypeDescriptor::GetEditor(this, editorBaseType, true);
}

System::ComponentModel::EventDescriptorCollection^ MaptoolRenderer::OBJSubsetCollection::GetEvents()
{
	return TypeDescriptor::GetEvents(this, true);
}

System::ComponentModel::EventDescriptorCollection^ MaptoolRenderer::OBJSubsetCollection::GetEvents(array<System::Attribute^, 1>^ attributes)
{
	return TypeDescriptor::GetEvents(this, attributes, true);
}

System::ComponentModel::PropertyDescriptorCollection^ MaptoolRenderer::OBJSubsetCollection::GetProperties()
{
	PropertyDescriptorCollection^ pds = gcnew PropertyDescriptorCollection(nullptr);
	OBJObject^ obj;
	if (!obj_->TryGetTarget(obj))
	{
		return pds;
	}
	for each (auto subset in subsets_)
	{
		OBJSubsetCollectionItemDescriptor^ pd = gcnew OBJSubsetCollectionItemDescriptor(subset);
		pds->Add(pd);
	}
	// return the property descriptor collection
	return pds;
}

System::ComponentModel::PropertyDescriptorCollection^ MaptoolRenderer::OBJSubsetCollection::GetProperties(array<System::Attribute^, 1>^ attributes)
{
	return GetProperties();
}

System::Object^ MaptoolRenderer::OBJSubsetCollection::GetPropertyOwner(System::ComponentModel::PropertyDescriptor^ pd)
{
	return this;
}
System::Collections::IEnumerator^ MaptoolRenderer::OBJSubsetCollection::GetEnumerator2()
{
	return subsets_->GetEnumerator();
}


void MaptoolRenderer::OBJSubsetCollection::Add(MaptoolRenderer::OBJSubset^ item)
{
}

void MaptoolRenderer::OBJSubsetCollection::Clear()
{
}

bool MaptoolRenderer::OBJSubsetCollection::Contains(MaptoolRenderer::OBJSubset^ item)
{
	return subsets_->Contains(item);;
}

void MaptoolRenderer::OBJSubsetCollection::CopyTo(array<MaptoolRenderer::OBJSubset^, 1>^ array, int arrayIndex)
{
	subsets_->CopyTo(array, arrayIndex);
}

bool MaptoolRenderer::OBJSubsetCollection::Remove(MaptoolRenderer::OBJSubset^ item)
{
	return false;
}
