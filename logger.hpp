#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <node.h>

class Logger : public node::ObjectWrap {
   public:
      static void Init(v8::Handle<v8::Object> target);

   private:
      Logger(std::string, std::string);
      ~Logger();

      void writelog(std::string, std::string);
      int getFacilityID(std::string);
      int getPriorityID(std::string);

      static v8::Handle<v8::Value> New(const v8::Arguments& args);
      static v8::Handle<v8::Value> Log(const v8::Arguments& args);
};

#endif
