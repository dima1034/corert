// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.
// 

#include "common.h"
#include "gcenv.h"
#include "gchandletableimpl.h"
#include "objecthandle.h"

GCHandleStore* g_gcGlobalHandleStore;

IGCHandleTable* CreateGCHandleTable()
{
    return new (nothrow) GCHandleTable();
}

void GCHandleStore::Uproot()
{
    Ref_RemoveHandleTableBucket(_underlyingBucket);
}

bool GCHandleStore::ContainsHandle(OBJECTHANDLE handle)
{
    return _underlyingBucket->Contains(handle);
}

OBJECTHANDLE GCHandleStore::CreateHandleOfType(Object* object, int type)
{
    HHANDLETABLE handletable = _underlyingBucket->pTable[GetCurrentThreadHomeHeapNumber()];
    return ::HndCreateHandle(handletable, type, ObjectToOBJECTREF(object));
}

OBJECTHANDLE GCHandleStore::CreateHandleOfType(Object* object, int type, int heapToAffinitizeTo)
{
    HHANDLETABLE handletable = _underlyingBucket->pTable[heapToAffinitizeTo];
    return ::HndCreateHandle(handletable, type, ObjectToOBJECTREF(object));
}

OBJECTHANDLE GCHandleStore::CreateHandleWithExtraInfo(Object* object, int type, void* pExtraInfo)
{
    HHANDLETABLE handletable = _underlyingBucket->pTable[GetCurrentThreadHomeHeapNumber()];
    return ::HndCreateHandle(handletable, type, ObjectToOBJECTREF(object), reinterpret_cast<uintptr_t>(pExtraInfo));
}

OBJECTHANDLE GCHandleStore::CreateDependentHandle(Object* primary, Object* secondary)
{
    HHANDLETABLE handletable = _underlyingBucket->pTable[GetCurrentThreadHomeHeapNumber()];
    OBJECTHANDLE handle = ::HndCreateHandle(handletable, HNDTYPE_DEPENDENT, ObjectToOBJECTREF(primary));
    ::SetDependentHandleSecondary(handle, ObjectToOBJECTREF(secondary));

    return handle;
}

GCHandleStore::~GCHandleStore()
{
    Ref_DestroyHandleTableBucket(_underlyingBucket);
}

bool GCHandleTable::Initialize()
{
    return Ref_Initialize();
}

void GCHandleTable::Shutdown()
{
    Ref_Shutdown();
}

IGCHandleStore* GCHandleTable::GetGlobalHandleStore()
{
    return g_gcGlobalHandleStore;
}

IGCHandleStore* GCHandleTable::CreateHandleStore(void* context)
{
#ifndef FEATURE_REDHAWK
    HandleTableBucket* newBucket = ::Ref_CreateHandleTableBucket(ADIndex((DWORD)(uintptr_t)context));
    return new (nothrow) GCHandleStore(newBucket);
#else
    assert("CreateHandleStore is not implemented when FEATURE_REDHAWK is defined!");
    UNREFERENCED_PARAMETER(context);
    return nullptr;
#endif
}

void GCHandleTable::DestroyHandleStore(IGCHandleStore* store)
{
    delete store;
}

void* GCHandleTable::GetHandleContext(OBJECTHANDLE handle)
{
    return (void*)((uintptr_t)::HndGetHandleTableADIndex(::HndGetHandleTable(handle)).m_dwIndex);
}

OBJECTHANDLE GCHandleTable::CreateGlobalHandleOfType(Object* object, int type)
{
    return ::HndCreateHandle(g_HandleTableMap.pBuckets[0]->pTable[GetCurrentThreadHomeHeapNumber()], type, ObjectToOBJECTREF(object)); 
}

OBJECTHANDLE GCHandleTable::CreateDuplicateHandle(OBJECTHANDLE handle)
{
    return ::HndCreateHandle(HndGetHandleTable(handle), HNDTYPE_DEFAULT, ::HndFetchHandle(handle));
}

void GCHandleTable::DestroyHandleOfType(OBJECTHANDLE handle, int type)
{
    ::HndDestroyHandle(::HndGetHandleTable(handle), type, handle);
}

void GCHandleTable::DestroyHandleOfUnknownType(OBJECTHANDLE handle)
{
    ::HndDestroyHandleOfUnknownType(::HndGetHandleTable(handle), handle);
}

void* GCHandleTable::GetExtraInfoFromHandle(OBJECTHANDLE handle)
{
    return (void*)::HndGetHandleExtraInfo(handle);
}
