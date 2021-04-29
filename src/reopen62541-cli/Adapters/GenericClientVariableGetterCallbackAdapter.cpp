#include <reopen62541-cli/Adapters/GenericClientVariableGetterCallbackAdapter.h>

generic<class T>
UA::GenericClientVariableGetterCallbackAdapter<T>::GenericClientVariableGetterCallbackAdapter() :
  value(T())
{
  managed_getter_callback = gcnew UA::ManagedClientVariableGetterCallback(
    this, &UA::GenericClientVariableGetterCallbackAdapter<T>::GetterCallback);
  native_getter = GCHandle::Alloc(managed_getter_callback);
  native_getter_callback = static_cast<UA::NativeClientVariableGetterCallback>(
    Marshal::GetFunctionPointerForDelegate(managed_getter_callback).ToPointer());
}

generic<class T>
UA::GenericClientVariableGetterCallbackAdapter<T>::~GenericClientVariableGetterCallbackAdapter()
{
  this->!GenericClientVariableGetterCallbackAdapter();
}

generic<class T>
UA::GenericClientVariableGetterCallbackAdapter<T>::!GenericClientVariableGetterCallbackAdapter()
{
  if (native_getter.IsAllocated)
  {
    native_getter.Free();
  }
}

generic<class T>
UA::NativeClientVariableGetterCallback UA::GenericClientVariableGetterCallbackAdapter<T>::GetNativeGetterCallback()
{
  return native_getter_callback;
}

generic<class T>
void UA::GenericClientVariableGetterCallbackAdapter<T>::GetterCallback(const ua::variant& output)
{
  UA::Variant variant(output);

  value = variant.Get<T>();
}
