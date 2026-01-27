#pragma once

#include "Formatter.h"

/** @class CsvFormatter @brief Formats log records into Comma-Separated Values. */
class CsvFormatter : public Formatter {
public:
    /** @brief Converts LogRecord into a CSV line. */
    std::string format(const LogRecord& record) override;
    
    /** @brief Returns FormatType::CSV. */
    FormatType getType() override;
};

