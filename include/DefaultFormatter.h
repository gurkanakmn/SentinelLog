#pragma once

#include "Formatter.h"

class DefaultFormatter : public Formatter {
public:
    /** @brief Converts LogRecord into a standard string (Date [Level] [Location] Message). */
    std::string format(const LogRecord& record) override;
    
    /** @brief Returns FormatType::TEXT. */
    FormatType getType() override;
};