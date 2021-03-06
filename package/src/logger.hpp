#include <napi.h>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace logger {

    static std::string getLastErrorAsString() {
        DWORD errorMessageID = ::GetLastError();
        if (errorMessageID == 0) {
            return std::string();
        }

        DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
        DWORD languageId = 0;
        LPSTR messageBuffer = NULL;
        size_t size = FormatMessageA(flags, NULL, errorMessageID, languageId, (LPSTR)&messageBuffer, 0, NULL);

        std::string message(messageBuffer, size);
        LocalFree(messageBuffer);
        return message;
    }


    enum Severity {
        error,
        warn,
        info
    };

    static bool parseSeverity(logger::Severity severity, WORD *type) {
        switch (severity) {
            case logger::Severity::info :
                *type = EVENTLOG_INFORMATION_TYPE;
                return true;

            case logger::Severity::warn :
                *type = EVENTLOG_WARNING_TYPE;
                return true;

            case logger::Severity::error :
                *type = EVENTLOG_ERROR_TYPE;
                return true;

            default :
                return false;
        }
    }


    class EventLog : public Napi::ObjectWrap<EventLog> {
    private:
        static Napi::FunctionReference constructor;

    public:
        HANDLE eventLogHandle_;
        std::string source;

        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        static Napi::Object NewInstance(Napi::Env env, Napi::Value arg);

        EventLog(const Napi::CallbackInfo& info);

        Napi::Value log(const Napi::CallbackInfo& info);

        ~EventLog() {
            if (eventLogHandle_) {
                DeregisterEventSource(eventLogHandle_);
            }
        }
    };


    struct logArgs {
    public:
        Severity severity;
        std::string message;
        int eventId;

        EventLog* eventLog;

        logArgs(Severity severity, std::string message, int eventId, EventLog* eventLog)
            : severity(severity), message(message), eventId(eventId), eventLog(eventLog) {}
    };

    logArgs CreateLogArgs(Severity severity, const std::string message, EventLog* eventLog, int eventId);


    bool log(logger::Severity severity, const std::string& message, int eventId, EventLog* eventLog, Napi::Env &env);

    Napi::Value log_wrapped(const Napi::CallbackInfo& info, EventLog* eventLog);


    class logWorker : public Napi::AsyncWorker {
    private:
        Severity severity;
        std::string message;
        int eventId;
        bool result;

        EventLog* eventLog;
        WORD type;

        Napi::Promise::Deferred deferred;

    public:
        logWorker(Napi::Env &env, logArgs* args)
            : Napi::AsyncWorker(env),
            severity(args->severity),
            message(args->message),
            eventId(args->eventId),
            eventLog(args->eventLog),
            deferred(Napi::Promise::Deferred::New(env))
        {}

        ~logWorker() {}

        void Execute();
        Napi::Promise GetPromise();

        void OnOK();
        void OnError(Napi::Error const &error);
    };

}