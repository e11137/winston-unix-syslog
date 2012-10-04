#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "logger.hpp"

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#define SYSLOG_NAMES
#include <syslog.h>

using namespace v8;



Logger::Logger(std::string name, std::string facilityName) {
	char* cname = new char [name.size()+1];
	strcpy (cname, name.c_str());
	int facility = getFacilityID(facilityName);

	openlog (cname, LOG_CONS | LOG_PID | LOG_NDELAY, facility);
};


int Logger::getFacilityID(std::string facilityName) {
	CODE *c;
	char* cname =  new char [facilityName.size()+1];
	strcpy (cname, facilityName.c_str());
	if (isdigit(*cname))
                return (atoi(cname));
	for (c = facilitynames; c->c_name; c++) 
		if(!facilityName.compare(c->c_name)) 
			return (c->c_val);
	return -1;
}


int Logger::getPriorityID(std::string priorityName) {
	CODE *c;
	char* cname =  new char [priorityName.size()+1];
	strcpy (cname, priorityName.c_str());
	if (isdigit(*cname))
		return (atoi(cname));
	for (c = prioritynames; c->c_name; c++) 
		if(priorityName.compare(c->c_name)) 
			return (c->c_val);
	return -1;
}


Logger::~Logger() {
	closelog ();
};


void Logger::writelog(std::string level, std::string message){
	int priority = getPriorityID(level);
	syslog (priority, message.c_str());
}


void Logger::Init(Handle<Object> target) {
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("Logger"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	// Prototype
	tpl->PrototypeTemplate()->Set(String::NewSymbol("log"),
			FunctionTemplate::New(Log)->GetFunction());

	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("Logger"), constructor);
}


Handle<Value> Logger::New(const Arguments& args) {
	HandleScope scope;
	Logger* obj;

	if(args.Length() == 2 && args[0]->IsString() && args[1]->IsString()){
		Local<String> refMessage = args[0]->ToString ();
		std::string name;
		Local<String> refFacilityName = args[1]->ToString ();
                std::string facilityName;

		if(refMessage->IsUndefined()) name = "default";
		else {
			String::AsciiValue paramMessage(refMessage);
			name = std::string(*paramMessage);
		}
		if(refFacilityName->IsUndefined()) facilityName = "local0";
		else {
			String::AsciiValue paramFacilityName(refFacilityName);
			facilityName = std::string(*paramFacilityName);
		}

		obj = new Logger(name, facilityName);
		obj->Wrap(args.This());
	}

	return args.This();
}

Handle<Value> Logger::Log(const Arguments& args) {
	HandleScope scope;
	std::string priority;
	std::string message;
	if(args.Length() == 2) {
		if(args[0]->IsString()) {
			Local<String> refPriority = args[0]->ToString ();
			String::AsciiValue paramPriority(refPriority);
			priority = std::string(*paramPriority);
		}
		if(args[1]->IsString()){
			Local<String> refMessage = args[1]->ToString ();
			String::AsciiValue paramMessage(refMessage);
                        message = std::string(*paramMessage);
		}
	}

	Logger* obj = ObjectWrap::Unwrap<Logger>(args.This());
	obj->writelog(priority, message);

	return scope.Close(Undefined());
}
