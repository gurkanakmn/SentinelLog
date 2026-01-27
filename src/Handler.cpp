#include "../include/Handler.h"
#include <memory>

Handler::Handler(std::unique_ptr<Formatter> formatter){
    this->formatter = move(formatter);
}

void Handler::flush(){}