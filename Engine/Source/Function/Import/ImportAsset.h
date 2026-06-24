#pragma once

#include "Core.h"
#include "String/String.h"
#include "World/Resource/Mesh.h"

#include <tiny_obj_loader.h>

#include <glm/vec4.hpp>



bool ImportObj(const String& path, Mesh& mesh);