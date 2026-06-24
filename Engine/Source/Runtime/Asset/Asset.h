#pragma once

#include "Core.h"
#include "Memory/RefCount.h"

class Asset : public RefCount
{
public:
    virtual ~Asset() = default;
};