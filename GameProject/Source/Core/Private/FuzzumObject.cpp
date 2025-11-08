// FuzzumObject.cpp
#include "../Public/FuzzumObject.h"

namespace fuzzum {

std::unordered_map<std::string, FuzzumClassInfo> GClassRegistry;

FuzzumObject* NewObject(const char* className) {
    auto it = GClassRegistry.find(className);
    if (it == GClassRegistry.end()) {
        return nullptr;
    }
    return it->second.Factory();
}

FuzzumObject* NewObject(const std::string& className) {
    return NewObject(className.c_str());
}

bool ClassExists(const char* className) {
    return GClassRegistry.find(className) != GClassRegistry.end();
}

bool ClassExists(const std::string& className) {
    return ClassExists(className.c_str());
}

} // namespace fuzzum