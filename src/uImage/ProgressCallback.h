#pragma once
#include <uBase/Object.h>

namespace uImage
{
    /** \ingroup uBaseImage */
    class ProgressCallback: public uBase::Object
    {
    public:
        virtual void Update(double current, double total) = 0;
    };
}
