// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#ifndef GCHANDLETABLE_H_
#define GCHANDLETABLE_H_

#include "gcinterface.h"
#include "objecthandle.h"

class GCHandleStore : public IGCHandleStore
{
public:
    GCHandleStore(HandleTableBucket *bucket) 
        : _underlyingBucket(bucket)
        { }

    virtual void Uproot();

    virtual bool ContainsHandle(OBJECTHANDLE handle);

    virtual OBJECTHANDLE CreateHandleOfType(Object* object, int type);

    virtual OBJECTHANDLE CreateHandleOfType(Object* object, int type, int heapToAffinitizeTo);

    virtual OBJECTHANDLE CreateHandleWithExtraInfo(Object* object, int type, void* pExtraInfo);

    virtual OBJECTHANDLE CreateDependentHandle(Object* primary, Object* secondary);

    virtual ~GCHandleStore();

private:
    HandleTableBucket* _underlyingBucket;
};

extern GCHandleStore* g_gcGlobalHandleStore;

class GCHandleTable : public IGCHandleTable
{
public:
    virtual bool Initialize();

    virtual void Shutdown();

    virtual void* GetHandleContext(OBJECTHANDLE handle);

    virtual IGCHandleStore* GetGlobalHandleStore();

    virtual IGCHandleStore* CreateHandleStore(void* context);

    virtual void DestroyHandleStore(IGCHandleStore* store);

    virtual OBJECTHANDLE CreateGlobalHandleOfType(Object* object, int type);

    virtual OBJECTHANDLE CreateDuplicateHandle(OBJECTHANDLE handle);

    virtual void DestroyHandleOfType(OBJECTHANDLE handle, int type);

    virtual void DestroyHandleOfUnknownType(OBJECTHANDLE handle);

    virtual void* GetExtraInfoFromHandle(OBJECTHANDLE handle);
};

#endif  // GCHANDLETABLE_H_
