#include <reopen62541-cli/Adapters/LogCallbackAdapter.h>

UA::LogCallbackAdapter::LogCallbackAdapter(Action<UA::LogLevel, UA::LogCategory, String^>^ log) :
  managed_log(log)
{
  managed_log_callback = gcnew UA::ManagedLogCallback(
    this, &UA::LogCallbackAdapter::ActionCallback);
  native_log = GCHandle::Alloc(managed_log_callback);
  native_log_callback = static_cast<UA::NativeLogCallback>(
    Marshal::GetFunctionPointerForDelegate(managed_log_callback).ToPointer());
}

UA::LogCallbackAdapter::~LogCallbackAdapter()
{
  this->!LogCallbackAdapter();
}

UA::LogCallbackAdapter::!LogCallbackAdapter()
{
  if (native_log.IsAllocated)
  {
    native_log.Free();
  }
}

void UA::LogCallbackAdapter::ActionCallback(UA_LogLevel level, UA_LogCategory category, const std::string& message)
{
  auto log_level = static_cast<UA::LogLevel>(level);
  auto log_category = static_cast<UA::LogCategory>(category);
  auto log_message = UA::Convert::ToString(message);

  managed_log(log_level, log_category, log_message);
}
