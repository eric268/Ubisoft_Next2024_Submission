#pragma once

// Minimum header files for the engine to run at a basic level

// Application
#include "Core/ResourceManager.h"

// ECS
#include "ECS/Registry.h"
#include "ECS/Entity.h"

// Factories
#include "Factories/CameraFactory.h"
#include "Factories/LightFactory.h"
#include "Factories/GameObjectFactory.h"
#include "Factories/SkySphereFactory.h"

// Events
#include "Event/WindowEvents.h"

// Input
#include "Input/Input.h"

// Math
#include "Math/MathCore.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

// Misc
#include "Core/Log.h"
#include "Core/Stopwatch.h"

// Systems
#include "Systems/SystemManager.h"

// World
#include "World/Scene.h"
#include "World/World.h"