#include <reopen62541-cli/Adapters/GenericServerVariableCallbackAdapter.h>

generic<class T>
UA::GenericServerVariableCallbackAdapter<T>::GenericServerVariableCallbackAdapter(Func<T>^ getter, Action<T>^ setter) :
  managed_getter(getter),
  managed_setter(setter)
{
  if (managed_getter != nullptr)
  {
    managed_getter_callback = gcnew UA::ManagedServerVariableGetterCallback(
      this, &UA::GenericServerVariableCallbackAdapter<T>::GetterCallback);
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
      this, &UA::GenericServerVariableCallbackAdapter<T>::SetterCallback);
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

generic<class T>
UA::GenericServerVariableCallbackAdapter<T>::~GenericServerVariableCallbackAdapter()
{
  this->!GenericServerVariableCallbackAdapter();
}

generic<class T>
UA::GenericServerVariableCallbackAdapter<T>::!GenericServerVariableCallbackAdapter()
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

generic<class T>
UA::NativeServerVariableGetterCallback UA::GenericServerVariableCallbackAdapter<T>::GetNativeGetterCallback()
{
  return native_getter_callback;
}

generic<class T>
UA::NativeServerVariableSetterCallback UA::GenericServerVariableCallbackAdapter<T>::GetNativeSetterCallback()
{
  return native_setter_callback;
}

generic<class T>
void UA::GenericServerVariableCallbackAdapter<T>::GetterCallback(ua::variant& output)
{
  UA::Variant variant(output);

  T value = managed_getter();

  variant.Set<T>(value);
}

generic<class T>
void UA::GenericServerVariableCallbackAdapter<T>::SetterCallback(const ua::variant& input)
{
  UA::Variant variant(input);

  T value = variant.Get<T>();

  managed_setter(value);
}
