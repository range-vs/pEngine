#pragma once

#include "../pEntryPoint/pch.h"

namespace gmtypes
{
	enum VectorCoordinate
	{
        vx = 0,
        vy = 1,
        vz = 2,
        vw = 3,
        cr = 0,
        cg = 1,
        cb = 2,
        ca = 3,
        sw = 0,
        sh = 1
	};

	enum PlanesParts
	{
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		FARP, // зад
		NEARP // перед
	};

}
