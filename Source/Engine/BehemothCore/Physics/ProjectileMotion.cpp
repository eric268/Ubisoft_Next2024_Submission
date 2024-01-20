#include "pch.h"
#include "ProjectileMotion.h"

namespace Behemoth
{
// 	BMath::Vector3 ProjectileMotion::CalculateInitalVelocity(float power, float angleX, float angleY)
// 	{
// 		BMath::Vector3 velocity;
// 		velocity.x = power * std::cos(angleY) * std::sin(angleX);
// 		velocity.y = power * std::sin(angleY);
// 		velocity.z = power * std::cos(angleY) * std::cos(angleX);
// 
// 		return velocity;
// 	}
	BMath::Vector3 ProjectileMotion::CalculateInitalVelocity(float power, const BMath::Vector3& direction)
	{
		BMath::Vector3 dir = direction;
		dir.Normalize();

		return dir * power;
	}
}