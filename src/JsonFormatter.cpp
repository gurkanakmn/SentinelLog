#include "../include/JsonFormatter.h"
#include <sstream>
#include <string>

FormatType JsonFormatter::getType(){
    return FormatType::JSON;
}

static std::string escapeJson(const std::string& input) {
    std::ostringstream oss;
    for (char c : input) {
        switch (c) {
            case '\"': oss << "\\\""; break;
            case '\\': oss << "\\\\"; break;
            case '\n': oss << "\\n"; break;
            case '\r': oss << "\\r"; break;
            case '\t': oss << "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    oss << "\\u"
                        << std::hex << std::setw(4)
                        << std::setfill('0') << (int)c;
                } else {
                    oss << c;
                }
        }
    }
    return oss.str();
}
std::string JsonFormatter::format(const LogRecord& record) {
    std::ostringstream oss;
    oss << "{"
        << "\"timestamp\":\"" << formatTimestampUTC(record.timestamp) << "\","
        << "\"level\":\"" << toString(record.level) << "\",";
    
    if (!record.fileName.empty()) {
        oss << "\"thread_id\":\"" << record.threadId << "\","
            << "\"file\":\"" << escapeJson(record.fileName) << "\","
            << "\"line\":" << record.lineNumber << ","
            << "\"func\":\"" << escapeJson(record.functionName) << "\",";
    }

    oss << "\"message\":\"" << escapeJson(record.message) << "\""
        << "}";

    return oss.str();
}