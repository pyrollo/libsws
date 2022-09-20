#ifndef SWSEXCEPTIONS_H
#define SWSEXCEPTIONS_H

#include <exception>
#include <string>

namespace sws {

    class exception: public std::exception
    {
    public:
        virtual const char* what() const throw() { return mMsg.c_str(); }
        std::string mMsg;
    protected:
        exception(std::string msg): mMsg(msg) {}
    };

    class internal_error: public exception
    {
    public:
        internal_error(std::string msg): exception(msg) {}

    };

    class unknown_object: public exception
    {
    public:
        unknown_object(std::string objectType, std::string objectName):
            exception("Unknown " + objectType + " \"" + objectName + "\""), mObjectName(objectName) {}

        std::string mMsg;
        std::string mObjectName;
    };

    class unknown_plug: public unknown_object
    {
    public:
        unknown_plug(std::string objectName): unknown_object("plug", objectName) {}
    };

    class unknown_module: public unknown_object
    {
    public:
        unknown_module(std::string objectName): unknown_object("module", objectName) {}
    };

    class unknown_schema: public unknown_object
    {
    public:
        unknown_schema(std::string objectName): unknown_object("schema", objectName) {}
    };

    class illegal_connection: public exception
    {
    public:
        illegal_connection(): exception("Such connection is illegal") {}
    };

    class illegal_operation: public exception
    {
    public:
        illegal_operation(): exception("Such operation is illegal") {}
    };
    class already_connected: public exception
    {
    public:
        already_connected(): exception("Plug already connected") {}
    };

    class duplicate_name: public exception
    {
    public:
        duplicate_name(): exception("Name already used") {}
    };

    class unable_to_schedule_children: public exception
    {
    public:
        unable_to_schedule_children(): exception("Unable to schedule children") {}
    };

    class loop_in_instanciations: public exception
    {
    public:
        loop_in_instanciations(): exception("Loop in instanciations are forbidden") {}
    };

    class invalid_name: public exception
    {
    public:
        invalid_name(std::string objectName):
            exception("\"" + objectName + "\" is not a valid name") {}
    };

    class unknown_type: public exception
    {
    public:
        unknown_type(std::string typeName):
            exception("Unkown type \"" + typeName + "\"") {}
    };

}

#endif // SWSEXCEPTIONS_H
