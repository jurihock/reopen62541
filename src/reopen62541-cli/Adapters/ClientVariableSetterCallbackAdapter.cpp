#include <reopen62541-cli/Adapters/ClientVariableSetterCallbackAdapter.h>

UA::ClientVariableSetterCallbackAdapter::ClientVariableSetterCallbackAdapter(Action<UA::Variant^>^ setter) :
  managed_setter(setter)
{
  if (managed_setter != nullptr)
  {
    managed_setter_callback = gcnew UA::ManagedClientVariableSetterCallback(
      this, &UA::ClientVariableSetterCallbackAdapter::SetterCallback);
    native_setter = GCHandle::Alloc(managed_setter_callback);
    native_setter_callback = static_cast<UA::NativeClientVariableSetterCallback>(
      Marshal::GetFunctionPointerForDelegate(managed_setter_callback).ToPointer());
  }
  else
  {
    managed_setter_callback = nullptr;
    native_setter_callback = nullptr;
  }
}

UA::ClientVariableSetterCallbackAdapter::~ClientVariableSetterCallbackAdapter()
{
  this->!ClientVariableSetterCallbackAdapter();
}

UA::ClientVariableSetterCallbackAdapter::!ClientVariableSetterCallbackAdapter()
{
  if (native_setter.IsAllocated)
  {
    native_setter.Free();
  }
}

UA::NativeClientVariableSetterCallback UA::ClientVariableSetterCallbackAdapter::GetNativeSetterCallback()
{
  return native_setter_callback;
}

void UA::ClientVariableSetterCallbackAdapter::SetterCallback(ua::variant& input)
{
  auto variant = gcnew UA::Variant(input);

  managed_setter(variant);
}
