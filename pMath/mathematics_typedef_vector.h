﻿#pragma once

#include "../pEntryPoint/pch.h"

// pMath
#include "Vector.h"
#include "other_types.h"

// объявления всех классов
namespace gmath
{
	template<class T> class Vector_2;
	template<class T> class Vector_3;
	template<class T> class Vector_4;
	//template<class T> class alignas(s) Matrix_2;
	//template<class T> class alignas(s) Matrix_3;
	template<class T> class Matrix_4;
}

// код всех классов - векторов
#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"

// псевдонимы
namespace gmath
{
	using Vector4 = Vector_4<float>;
	using Vector3 = Vector_3<float>;
	using Vector = Vector4;
	using Point3 = Vector_3<float>;
	using Point4 = Vector_4<float>;
	using Point = Vector_3<float>;
    using Point2 = Vector_2<long long>;
	using Color = Vector_4<float>;
    using Coordinate2 = Vector_2<int>;
    using Size2 = Vector_2<int>;
	using UVCoordinate = Vector_2<float>;
}
