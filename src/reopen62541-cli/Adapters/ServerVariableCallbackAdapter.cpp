#include <reopen62541-cli/Adapters/ServerVariableCallbackAdapter.h>

UA::ServerVariableCallbackAdapter::ServerVariableCallbackAdapter(Action<UA::Variant^>^ getter, Action<UA::Variant^>^ setter) :
  managed_getter(getter),
  managed_setter(setter)
{
  if (managed_getter != nullptr)
  {
    managed_getter_callback = gcnew UA::ManagedServerVariableGetterCallback(
      this, &UA::ServerVariableCallbackAdapter::GetterCallback);
    native_getter = GCHandle::Alloc(managed_getter_callback);
    native_getter_callback = static_cast<UA::NativeServerVariableGetterCallback>(
      Marshal::GetFunctionPointerForDelegate(managed_getter_callback).ToPointer());
  }
  else
  {
    managed_getter_callback = nullptr;
    native_getter_callback = nullptr;
  }

  if (managed_setter != nullptr)
  {
    managed_setter_callback = gcnew UA::ManagedServerVariableSetterCallback(
      this, &UA::ServerVariableCallbackAdapter::SetterCallback);
    native_setter = GCHandle::Alloc(managed_setter_callback);
    native_setter_callback = static_cast<UA::NativeServerVariableSetterCallback>(
      Marshal::GetFunctionPointerForDelegate(managed_setter_callback).ToPointer());
  }
  else
  {
    managed_setter_callback = nullptr;
    native_setter_callback = nullptr;
  }
}

UA::ServerVariableCallbackAdapter::~ServerVariableCallbackAdapter()
{
  this->!ServerVariableCallbackAdapter();
}

UA::ServerVariableCallbackAdapter::!ServerVariableCallbackAdapter()
{
  if (native_getter.IsAllocated)
  {
    native_getter.Free();
  }

  if (native_setter.IsAllocated)
  {
    native_setter.Free();
  }
}

UA::NativeServerVariableGetterCallback UA::ServerVariableCallbackAdapter::GetNativeGetterCallback()
{
  return native_getter_callback;
}

UA::NativeServerVariableSetterCallback UA::ServerVariableCallbackAdapter::GetNativeSetterCallback()
{
  return native_setter_callback;
}

void UA::ServerVariableCallbackAdapter::GetterCallback(ua::variant& output)
{
  auto variant = gcnew UA::Variant(output);

  managed_getter(variant);
}

void UA::ServerVariableCallbackAdapter::SetterCallback(const ua::variant& input)
{
  auto variant = gcnew UA::Variant(input);

  managed_setter(variant);
}
