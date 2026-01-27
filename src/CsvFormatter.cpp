#include "../include/CsvFormatter.h"

#include <sstream>
#include <string>

FormatType CsvFormatter::getType(){
    return FormatType::CSV;
}

static std::string escapeCSV(const std::string& field) {
    bool needsQuotes = false;

    for (char c : field) {
        if (c == ',' || c == '"' || c == '\n') {
            needsQuotes = true;
            break;
        }
    }

    if (!needsQuotes) {
        return field;
    }

    std::string escaped = "\"";

    for (char c : field) {
        if (c == '"') {
            escaped += "\"\""; 
        } else {
            escaped += c;
        }
    }

    escaped += "\"";
    return escaped;
}

std::string CsvFormatter::format(const LogRecord& record) {
    std::ostringstream oss;

    oss << escapeCSV(formatTimestampLocal(record.timestamp)) << ","
        << escapeCSV(toString(record.level)) << ",";
    
    if (!record.fileName.empty()) {
        oss << escapeCSV(record.threadId) << "," 
            << escapeCSV(record.fileName) << ","
            << record.lineNumber << ","
            << escapeCSV(record.functionName) << ",";
    } else {
        oss << ",,,,"; 
    }

    oss << escapeCSV(record.message);

    return oss.str();
}