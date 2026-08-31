#include "mistpch.h"

#include "IManagedRuntime.h"

#include "DotNetRuntime.h"

namespace Mist {

IManagedRuntime* IManagedRuntime::s_ManagedRuntime = new DotNetRuntime;

} // namespace Mist