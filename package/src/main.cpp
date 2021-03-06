#include "logger.hpp"


Napi::Object CreateEventLog(const Napi::CallbackInfo& info) {
    return logger::EventLog::NewInstance(info.Env(), info[0]);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    Napi::Object newExports = Napi::Function::New(env, CreateEventLog, "CreateEventLog");
    return logger::EventLog::Init(env, newExports);
}


NODE_API_MODULE(EventLogger, InitAll)