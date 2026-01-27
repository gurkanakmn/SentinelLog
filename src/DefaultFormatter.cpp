#include "../include/DefaultFormatter.h"

FormatType DefaultFormatter::getType(){
    return FormatType::TEXT;
}

std::string DefaultFormatter::format(const LogRecord& record) {
    std::string meta = "";
    if (!record.fileName.empty()) {
        meta = "[T: " + record.threadId + "][" + record.fileName + ":" + std::to_string(record.lineNumber) + 
               " @" + record.functionName + "] ";
    }

    return "[" + formatTimestampLocal(record.timestamp) + "]" + 
           "[" + toString(record.level) + "] " +
           meta + record.message; 
}