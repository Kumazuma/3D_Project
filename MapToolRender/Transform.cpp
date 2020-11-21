#include "pch.h"
#include "Transform.h"

MapToolRender::Position::Position():
    Position{0.f,0.f,0.f}
{
    
}

MapToolRender::Position::Position(float x, float y, float z):
    x{ x },
    y{ y },
    z{ z }
{
    
}
MapToolRender::Position::Position(Position^  rhs):
    Position{rhs->x,rhs->y, rhs->z}
{
}
auto MapToolRender::Position::X::get()->float
{
    return x;
}
auto MapToolRender::Position::X::set(float value)->void
{
    x = value;
    OnPropertyChanged(L"X");
}
auto MapToolRender::Position::Y::get()->float
{
    return y;
}
auto MapToolRender::Position::Y::set(float value)->void
{
    y = value;
    OnPropertyChanged(L"Y");
}
auto MapToolRender::Position::Z::get()->float
{
    return z;
}
auto MapToolRender::Position::Z::set(float value)->void
{
    z = value;
    OnPropertyChanged(L"Z");
}

auto MapToolRender::Scale::X::get()->float
{
    return x;
}
auto MapToolRender::Scale::X::set(float value)->void
{
    x = value;
    OnPropertyChanged(L"X");
}
auto MapToolRender::Scale::Y::get()->float
{
    return y;
}
auto MapToolRender::Scale::Y::set(float value)->void
{
    y = value;
    OnPropertyChanged(L"Y");
}
auto MapToolRender::Scale::Z::get()->float
{
    return z;
}
auto MapToolRender::Scale::Z::set(float value)->void
{
    z = value;
    OnPropertyChanged(L"Z");
}

auto MapToolRender::Rotation::X::get()->float
{
    return x;
}
auto MapToolRender::Rotation::X::set(float value)->void
{
    x = value;
    OnPropertyChanged(L"X");
}
auto MapToolRender::Rotation::Y::get()->float
{
    return y;
}
auto MapToolRender::Rotation::Y::set(float value)->void
{
    y = value;
    OnPropertyChanged(L"Y");
}
auto MapToolRender::Rotation::Z::get()->float
{
    return z;
}
auto MapToolRender::Rotation::Z::set(float value)->void
{
    z = value;
    OnPropertyChanged(L"Z");
}

MapToolRender::Transform::Transform()
{
    this->m_positionChangedHandler = gcnew PropertyChangedEventHandler(this, &Transform::OnChangedPosition);
    this->m_rotationChangedHandler = gcnew PropertyChangedEventHandler(this, &Transform::OnChangedRotation);
    this->m_scaleChangedHandler = gcnew PropertyChangedEventHandler(this, &Transform::OnChangedScale);

    this->rotation = gcnew MapToolRender::Rotation();
    this->scale = gcnew MapToolRender::Scale();
    this->position = gcnew MapToolRender::Position();

    this->rotation->PropertyChanged += m_rotationChangedHandler;
    this->scale->PropertyChanged += m_scaleChangedHandler;
    this->position->PropertyChanged += m_positionChangedHandler;

}
MapToolRender::Transform::Transform(Transform^  rhs)
{
    this->m_positionChangedHandler = gcnew PropertyChangedEventHandler(this, &Transform::OnChangedPosition);
    this->m_rotationChangedHandler = gcnew PropertyChangedEventHandler(this, &Transform::OnChangedRotation);
    this->m_scaleChangedHandler = gcnew PropertyChangedEventHandler(this, &Transform::OnChangedScale);

    this->rotation = gcnew MapToolRender::Rotation(rhs->rotation);
    this->scale = gcnew MapToolRender::Scale(rhs->scale);
    this->position = gcnew MapToolRender::Position( rhs->position);

    this->rotation->PropertyChanged += m_rotationChangedHandler;
    this->scale->PropertyChanged += m_scaleChangedHandler;
    this->position->PropertyChanged += m_positionChangedHandler;
}
/*
        MapToolRender::Rotation^ rotation;
        MapToolRender::Scale^ scale;
        MapToolRender::Position^ position;
*/
auto MapToolRender::Transform::Rotation::get()->MapToolRender::Rotation^
{
    return rotation;
}
auto MapToolRender::Transform::Rotation::set(MapToolRender::Rotation^ value)->void
{
    rotation->PropertyChanged -= m_rotationChangedHandler;
    rotation = value;
    rotation->PropertyChanged += m_rotationChangedHandler;
    OnPropertyChanged(L"Rotation");
}
auto MapToolRender::Transform::Position::get()->MapToolRender::Position^
{
    return position;
}
auto MapToolRender::Transform::Position::set(MapToolRender::Position^ value)->void
{
    position->PropertyChanged -= m_positionChangedHandler;
    position = value;
    position->PropertyChanged += m_positionChangedHandler;
    OnPropertyChanged(L"Position");
}
auto MapToolRender::Transform::Scale::set(MapToolRender::Scale^ value)->void
{
    scale->PropertyChanged -= m_scaleChangedHandler;
    scale = value;
    scale->PropertyChanged += m_scaleChangedHandler;
    OnPropertyChanged(L"Scale");
}
auto MapToolRender::Transform::Scale::get()->MapToolRender::Scale^
{
    return scale;
}
void MapToolRender::Transform::OnChangedRotation(Object^ obj, PropertyChangedEventArgs^ e)
{
    OnPropertyChanged("Rotation");
}

void MapToolRender::Transform::OnChangedScale(Object^ obj, PropertyChangedEventArgs^ e)
{
    OnPropertyChanged("Scale");
}

void MapToolRender::Transform::OnChangedPosition(Object^ obj, PropertyChangedEventArgs^ e)
{
    OnPropertyChanged("Position");
}

MapToolRender::Scale::Scale():
    Scale{1.f, 1.f, 1.f}
{
}

MapToolRender::Scale::Scale(float x, float y, float z):
    x{ x },
    y{ y },
    z{ z }
{
    
}

MapToolRender::Scale::Scale(Scale^  rhs):
    Scale{ rhs->x,rhs->y, rhs->z }
{
}

MapToolRender::Rotation::Rotation():
    Rotation{0.f, 0.f, 0.f}
{
}

MapToolRender::Rotation::Rotation(float x, float y, float z):
    x{ x },
    y{ y },
    z{ z }
{
}

MapToolRender::Rotation::Rotation(Rotation^  rhs) :
    Rotation{ rhs->x,rhs->y, rhs->z }
{
}