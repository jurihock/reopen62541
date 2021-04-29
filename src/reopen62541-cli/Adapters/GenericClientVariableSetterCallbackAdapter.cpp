#include <reopen62541-cli/Adapters/GenericClientVariableSetterCallbackAdapter.h>

generic<class T>
UA::GenericClientVariableSetterCallbackAdapter<T>::GenericClientVariableSetterCallbackAdapter(T value) :
  value(value)
{
  managed_setter_callback = gcnew UA::ManagedClientVariableSetterCallback(
    this, &UA::GenericClientVariableSetterCallbackAdapter<T>::SetterCallback);
  native_setter = GCHandle::Alloc(managed_setter_callback);
  native_setter_callback = static_cast<UA::NativeClientVariableSetterCallback>(
    Marshal::GetFunctionPointerForDelegate(managed_setter_callback).ToPointer());
}

generic<class T>
UA::GenericClientVariableSetterCallbackAdapter<T>::~GenericClientVariableSetterCallbackAdapter()
{
  this->!GenericClientVariableSetterCallbackAdapter();
}

generic<class T>
UA::GenericClientVariableSetterCallbackAdapter<T>::!GenericClientVariableSetterCallbackAdapter()
{
  if (native_setter.IsAllocated)
  {
    native_setter.Free();
  }
}

generic<class T>
UA::NativeClientVariableSetterCallback UA::GenericClientVariableSetterCallbackAdapter<T>::GetNativeSetterCallback()
{
  return native_setter_callback;
}

generic<class T>
void UA::GenericClientVariableSetterCallbackAdapter<T>::SetterCallback(ua::variant& input)
{
  UA::Variant variant(input);

  variant.Set<T>(value);
}
