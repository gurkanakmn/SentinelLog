/**
 * @file JsonFormatter.h
 * @brief Formatter specialization for JSON output.
 */

#pragma once

#include "Formatter.h"

/**
 * @class JsonFormatter
 * @brief Formats LogRecords into valid JSON objects.
 * * Useful for integration with log analysis tools like ELK stack or Splunk.
 */
class JsonFormatter : public Formatter {
public:
    /** @brief Serializes a LogRecord into a JSON string. */
    std::string format(const LogRecord& record) override;

    /** @brief Returns FormatType::JSON. */
    FormatType getType() override;
};