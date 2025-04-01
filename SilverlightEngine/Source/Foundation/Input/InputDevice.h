#pragma once

#include "Foundation/Platform.h"

namespace Silverlight
{
    class InputDevice
    {
    public:
        virtual ~InputDevice() = default;
        virtual void Update() = 0;
        virtual bool IsButtonPressed(const int32 _button) const noexcept = 0;
        virtual double GetXChange() const noexcept = 0;
        virtual double GetYChange() const noexcept = 0;
    };
} // End of namespace