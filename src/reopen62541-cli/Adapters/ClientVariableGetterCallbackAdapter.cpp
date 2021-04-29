#include <reopen62541-cli/Adapters/ClientVariableGetterCallbackAdapter.h>

UA::ClientVariableGetterCallbackAdapter::ClientVariableGetterCallbackAdapter(Action<UA::Variant^>^ getter) :
  managed_getter(getter)
{
  if (managed_getter != nullptr)
  {
    managed_getter_callback = gcnew UA::ManagedClientVariableGetterCallback(
      this, &UA::ClientVariableGetterCallbackAdapter::GetterCallback);
    native_getter = GCHandle::Alloc(managed_getter_callback);
    native_getter_callback = static_cast<UA::NativeClientVariableGetterCallback>(
      Marshal::GetFunctionPointerForDelegate(managed_getter_callback).ToPointer());
  }
  else
  {
    managed_getter_callback = nullptr;
    native_getter_callback = nullptr;
  }
}

UA::ClientVariableGetterCallbackAdapter::~ClientVariableGetterCallbackAdapter()
{
  this->!ClientVariableGetterCallbackAdapter();
}

UA::ClientVariableGetterCallbackAdapter::!ClientVariableGetterCallbackAdapter()
{
  if (native_getter.IsAllocated)
  {
    native_getter.Free();
  }
}

UA::NativeClientVariableGetterCallback UA::ClientVariableGetterCallbackAdapter::GetNativeGetterCallback()
{
  return native_getter_callback;
}

void UA::ClientVariableGetterCallbackAdapter::GetterCallback(const ua::variant& output)
{
  auto variant = gcnew UA::Variant(output);

  managed_getter(variant);
}
