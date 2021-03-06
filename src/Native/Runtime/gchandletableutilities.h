// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#ifndef _GCHANDLETABLEUTILITIES_H_
#define _GCHANDLETABLEUTILITIES_H_

#include "gcinterface.h"

#ifdef FEATURE_COMINTEROP
#include <weakreference.h>
#endif

extern "C" IGCHandleTable* g_pGCHandleTable;

class GCHandleTableUtilities
{
public:
    // Retrieves the GC handle table.
    static IGCHandleTable* GetGCHandleTable() 
    {
        LIMITED_METHOD_CONTRACT;

        assert(g_pGCHandleTable != nullptr);
        return g_pGCHandleTable;
    }

private:
    // This class should never be instantiated.
    GCHandleTableUtilities() = delete;
};

// Given a handle, returns an OBJECTREF for the object it refers to.
inline OBJECTREF ObjectFromHandle(OBJECTHANDLE handle)
{
    _ASSERTE(handle);

    // Wrap the raw OBJECTREF and return it
    return UNCHECKED_OBJECTREF_TO_OBJECTREF(*PTR_UNCHECKED_OBJECTREF(handle));
}

inline BOOL ObjectHandleIsNull(OBJECTHANDLE handle)
{
    LIMITED_METHOD_CONTRACT;

    return *(Object **)handle == NULL;
}

#ifndef DACCESS_COMPILE

// Handle creation convenience functions

inline OBJECTHANDLE CreateHandle(IGCHandleStore* store, OBJECTREF object)
{
    return store->CreateHandleOfType(OBJECTREFToObject(object), HNDTYPE_DEFAULT);
}

inline OBJECTHANDLE CreateWeakHandle(IGCHandleStore* store, OBJECTREF object)
{
    return store->CreateHandleOfType(OBJECTREFToObject(object), HNDTYPE_WEAK_DEFAULT);
}

inline OBJECTHANDLE CreateShortWeakHandle(IGCHandleStore* store, OBJECTREF object)
{
    return store->CreateHandleOfType(OBJECTREFToObject(object), HNDTYPE_WEAK_SHORT);
}

inline OBJECTHANDLE CreateLongWeakHandle(IGCHandleStore* store, OBJECTREF object)
{
    return store->CreateHandleOfType(OBJECTREFToObject(object), HNDTYPE_WEAK_LONG);
}

inline OBJECTHANDLE CreateStrongHandle(IGCHandleStore* store, OBJECTREF object)
{
    return store->CreateHandleOfType(OBJECTREFToObject(object), HNDTYPE_STRONG);
}

inline OBJECTHANDLE CreatePinningHandle(IGCHandleStore* store, OBJECTREF object)
{
    return store->CreateHandleOfType(OBJECTREFToObject(object), HNDTYPE_PINNED);
}

inline OBJECTHANDLE CreateAsyncPinningHandle(IGCHandleStore* store, OBJECTREF object)
{
    return store->CreateHandleOfType(OBJECTREFToObject(object), HNDTYPE_ASYNCPINNED);
}

inline OBJECTHANDLE CreateRefcountedHandle(IGCHandleStore* store, OBJECTREF object)
{
    return store->CreateHandleOfType(OBJECTREFToObject(object), HNDTYPE_REFCOUNTED);
}

inline OBJECTHANDLE CreateSizedRefHandle(IGCHandleStore* store, OBJECTREF object)
{
    return store->CreateHandleOfType(OBJECTREFToObject(object), HNDTYPE_SIZEDREF);
}

inline OBJECTHANDLE CreateSizedRefHandle(IGCHandleStore* store, OBJECTREF object, int heapToAffinitizeTo)
{
    return store->CreateHandleOfType(OBJECTREFToObject(object), HNDTYPE_SIZEDREF, heapToAffinitizeTo);
}

// Global handle creation convenience functions

inline OBJECTHANDLE CreateGlobalHandle(OBJECTREF object)
{
    CONDITIONAL_CONTRACT_VIOLATION(ModeViolation, object == NULL);
    return GCHandleTableUtilities::GetGCHandleTable()->CreateGlobalHandleOfType(OBJECTREFToObject(object), HNDTYPE_DEFAULT);
}

inline OBJECTHANDLE CreateGlobalWeakHandle(OBJECTREF object)
{
    return GCHandleTableUtilities::GetGCHandleTable()->CreateGlobalHandleOfType(OBJECTREFToObject(object), HNDTYPE_WEAK_DEFAULT);
}

inline OBJECTHANDLE CreateGlobalShortWeakHandle(OBJECTREF object)
{
    CONDITIONAL_CONTRACT_VIOLATION(ModeViolation, object == NULL);
    return GCHandleTableUtilities::GetGCHandleTable()->CreateGlobalHandleOfType(OBJECTREFToObject(object), HNDTYPE_WEAK_SHORT);
}

inline OBJECTHANDLE CreateGlobalLongWeakHandle(OBJECTREF object)
{
    return GCHandleTableUtilities::GetGCHandleTable()->CreateGlobalHandleOfType(OBJECTREFToObject(object), HNDTYPE_WEAK_LONG);
}

inline OBJECTHANDLE CreateGlobalStrongHandle(OBJECTREF object)
{
    CONDITIONAL_CONTRACT_VIOLATION(ModeViolation, object == NULL);
    return GCHandleTableUtilities::GetGCHandleTable()->CreateGlobalHandleOfType(OBJECTREFToObject(object), HNDTYPE_STRONG);
}

inline OBJECTHANDLE CreateGlobalPinningHandle(OBJECTREF object)
{
    return GCHandleTableUtilities::GetGCHandleTable()->CreateGlobalHandleOfType(OBJECTREFToObject(object), HNDTYPE_PINNED);
}

inline OBJECTHANDLE CreateGlobalRefcountedHandle(OBJECTREF object)
{
    return GCHandleTableUtilities::GetGCHandleTable()->CreateGlobalHandleOfType(OBJECTREFToObject(object), HNDTYPE_REFCOUNTED);
}

// Special handle creation convenience functions

#ifdef FEATURE_COMINTEROP
inline OBJECTHANDLE CreateWinRTWeakHandle(IGCHandleStore* store, OBJECTREF object, IWeakReference* pWinRTWeakReference)
{
    return store->CreateHandleWithExtraInfo(OBJECTREFToObject(object), HNDTYPE_WEAK_WINRT, (void*)pWinRTWeakReference);
}
#endif // FEATURE_COMINTEROP

// Creates a variable-strength handle
inline OBJECTHANDLE CreateVariableHandle(IGCHandleStore* store, OBJECTREF object, uint32_t type)
{
    return store->CreateHandleWithExtraInfo(OBJECTREFToObject(object), HNDTYPE_VARIABLE, (void*)((uintptr_t)type));
}

// Handle destruction convenience functions

inline void DestroyHandle(OBJECTHANDLE handle)
{
    CONTRACTL
    {
        NOTHROW;
        GC_NOTRIGGER;
        MODE_ANY;
        CAN_TAKE_LOCK;
        SO_TOLERANT;
    }
    CONTRACTL_END;

    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_DEFAULT);
}

inline void DestroyWeakHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_WEAK_DEFAULT);
}

inline void DestroyShortWeakHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_WEAK_SHORT);
}

inline void DestroyLongWeakHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_WEAK_LONG);
}

inline void DestroyStrongHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_STRONG);
}

inline void DestroyPinningHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_PINNED);
}

inline void DestroyAsyncPinningHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_ASYNCPINNED);
}

inline void DestroyRefcountedHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_REFCOUNTED);
}

inline void DestroyDependentHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_DEPENDENT);
}

inline void  DestroyVariableHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_VARIABLE);
}

inline void DestroyGlobalHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_DEFAULT);
}

inline void DestroyGlobalWeakHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_WEAK_DEFAULT);
}

inline void DestroyGlobalShortWeakHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_WEAK_SHORT);
}

inline void DestroyGlobalLongWeakHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_WEAK_LONG);
}

inline void DestroyGlobalStrongHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_STRONG);
}

inline void DestroyGlobalPinningHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_PINNED);
}

inline void DestroyGlobalRefcountedHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_REFCOUNTED);
}

inline void DestroyTypedHandle(OBJECTHANDLE handle)
{
    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfUnknownType(handle);
}

#ifdef FEATURE_COMINTEROP
inline void DestroyWinRTWeakHandle(OBJECTHANDLE handle)
{
    CONTRACTL
    {
        NOTHROW;
        GC_NOTRIGGER;
        MODE_ANY;
        CAN_TAKE_LOCK;
        SO_TOLERANT;
    }
    CONTRACTL_END;

    // Release the WinRT weak reference if we have one. We're assuming that this will not reenter the
    // runtime, since if we are pointing at a managed object, we should not be using HNDTYPE_WEAK_WINRT
    // but rather HNDTYPE_WEAK_SHORT or HNDTYPE_WEAK_LONG.
    void* pExtraInfo = GCHandleTableUtilities::GetGCHandleTable()->GetExtraInfoFromHandle(handle);
    IWeakReference* pWinRTWeakReference = reinterpret_cast<IWeakReference*>(pExtraInfo);
    if (pWinRTWeakReference != nullptr)
    {
        pWinRTWeakReference->Release();
    }

    GCHandleTableUtilities::GetGCHandleTable()->DestroyHandleOfType(handle, HNDTYPE_WEAK_WINRT);
}
#endif

#endif // !DACCESS_COMPILE

#endif // _GCHANDLETABLEUTILITIES_H_

