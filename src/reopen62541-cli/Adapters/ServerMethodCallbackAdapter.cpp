#include <reopen62541-cli/Adapters/ServerMethodCallbackAdapter.h>

UA::ServerMethodCallbackAdapter::ServerMethodCallbackAdapter(Action<UA::Variant^, UA::Variant^>^ action) :
  managed_action(action)
{
  managed_action_callback = gcnew UA::ManagedServerMethodCallback(this, &UA::ServerMethodCallbackAdapter::ActionCallback);
  native_action = GCHandle::Alloc(managed_action_callback);
  native_action_callback = static_cast<UA::NativeServerMethodCallback>(
    Marshal::GetFunctionPointerForDelegate(managed_action_callback).ToPointer());
}

UA::ServerMethodCallbackAdapter::~ServerMethodCallbackAdapter()
{
  this->!ServerMethodCallbackAdapter();
}

UA::ServerMethodCallbackAdapter::!ServerMethodCallbackAdapter()
{
  if (native_action.IsAllocated)
  {
    native_action.Free();
  }
}

UA::NativeServerMethodCallback UA::ServerMethodCallbackAdapter::GetNativeActionCallback()
{
  return native_action_callback;
}

void UA::ServerMethodCallbackAdapter::ActionCallback(const ua::variant& input, ua::variant& output)
{
  auto variant_input = gcnew UA::Variant(input);
  auto variant_output = gcnew UA::Variant(output);

  managed_action(variant_input, variant_output);
}
