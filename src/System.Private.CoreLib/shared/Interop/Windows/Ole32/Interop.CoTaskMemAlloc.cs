// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

#nullable enable
using System;
using System.Runtime.InteropServices;

internal partial class Interop
{
    internal partial class Ole32
    {
        [DllImport(Libraries.Ole32)]
        internal static extern IntPtr CoTaskMemAlloc(UIntPtr cb);

        [DllImport(Libraries.Ole32)]
        internal static extern IntPtr CoTaskMemRealloc(IntPtr pv, UIntPtr cb);

        [DllImport(Libraries.Ole32)]
        internal static extern void CoTaskMemFree(IntPtr ptr);
    }
}
