#include <uBase/Atomic.h>
#include <uBase/Object.h>
#include <uBase/Exception.h>
#include <uBase/Traits.h>

namespace uBase
{
    Object::Object()
    {
        _refCount = 1;
    }

    Object::~Object()
    {
        if (_refCount > 1)
            throw BadDeleteException();
    }

    int Object::GetRefCount() const
    {
        return _refCount;
    }

    void Object::Retain()
    {
        AtomicIncrement(&_refCount);
    }

    void Object::Release()
    {
        if (AtomicDecrement(&_refCount) == 0)
            Delete();
    }

    void Object::Delete()
    {
        delete this;
    }

    String Object::ToString() const
    {
        return "<Object " + Default::ToString((void*)this) + ">";
    }
}
