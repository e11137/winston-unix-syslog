#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "logger.hpp"

using namespace v8;

void InitAll(Handle<Object> target) {
	  Logger::Init(target);
}

NODE_MODULE(Logger, InitAll)
