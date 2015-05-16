#include <osre/Math/BaseMath.h>

namespace OSRE {
namespace Math {

BaseMath::BaseMath()
{

}

BaseMath::~BaseMath()
{

}

f32 BaseMath::getSPEPS()
{
	return 0.000001f;
}

f32 BaseMath::getPI() {
	return 3.141592653589f;
}

}
}
