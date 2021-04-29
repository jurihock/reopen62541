#include <reopen62541-cli/Adapters/ClientMethodCallbackAdapter.h>

UA::ClientMethodCallbackAdapter::ClientMethodCallbackAdapter(Action<UA::Variant^>^ request, Action<UA::Variant^>^ response) :
  managed_request(request),
  managed_response(response)
{
  if (managed_request != nullptr)
  {
    managed_request_callback = gcnew UA::ManagedClientMethodRequestCallback(
      this, &UA::ClientMethodCallbackAdapter::RequestCallback);
    native_request = GCHandle::Alloc(managed_request_callback);
    native_request_callback = static_cast<UA::NativeClientMethodRequestCallback>(
      Marshal::GetFunctionPointerForDelegate(managed_request_callback).ToPointer());
  }
  else
  {
    managed_request_callback = nullptr;
    native_request_callback = nullptr;
  }

  if (managed_response != nullptr)
  {
    managed_response_callback = gcnew UA::ManagedClientMethodResponseCallback(
      this, &UA::ClientMethodCallbackAdapter::ResponseCallback);
    native_response = GCHandle::Alloc(managed_response_callback);
    native_response_callback = static_cast<UA::NativeClientMethodResponseCallback>(
      Marshal::GetFunctionPointerForDelegate(managed_response_callback).ToPointer());
  }
  else
  {
    managed_response_callback = nullptr;
    native_response_callback = nullptr;
  }
}

UA::ClientMethodCallbackAdapter::~ClientMethodCallbackAdapter()
{
  this->!ClientMethodCallbackAdapter();
}

UA::ClientMethodCallbackAdapter::!ClientMethodCallbackAdapter()
{
  if (native_request.IsAllocated)
  {
    native_request.Free();
  }

  if (native_response.IsAllocated)
  {
    native_response.Free();
  }
}

void UA::ClientMethodCallbackAdapter::RequestCallback(ua::variant& input)
{
  auto variant = gcnew UA::Variant(input);

  managed_request(variant);
}

void UA::ClientMethodCallbackAdapter::ResponseCallback(const ua::variant& output)
{
  auto variant = gcnew UA::Variant(output);

  managed_response(variant);
}
