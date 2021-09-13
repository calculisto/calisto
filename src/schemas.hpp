#pragma once
#include <string_view>

    namespace
isto::remote_services::schemas
{
    constexpr std::string_view
json_rpc_request_schema = R"(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "$id": "htpp://calcul-isto.cnrs-orleans.fr/schemas/json_rpc_request.schema.json#",
    "title":   "JSON-RPC 2.0 Specification",
    "definitions": {
        "base": {
            "type": "object",
            "properties": {
                "jsonrpc": {
                    "type": "string",
                    "const": "2.0",
                    "description": "A String specifying the version of the JSON-RPC protocol. MUST be exactly \"2.0\"."
                }
            },
            "required": [ "jsonrpc" ]
        },
        "has_method": {
            "properties": {
                "method": {
                    "type": "string",
                    "description": "A String containing the name of the method to be invoked. Method names that begin with the word rpc followed by a period character (U+002E or ASCII 46) are reserved for rpc-internal methods and extensions and MUST NOT be used for anything else."
                }
            },
            "required": [ "method" ]
        },
        "has_id": {
            "properties": {
                "id": {
                    "type": ["string", "number", "null"],
                    "description": "An identifier established by the Client that MUST contain a String, Number, or NULL value if included. If it is not included it is assumed to be a notification. The value SHOULD normally not be Null [1] and Numbers SHOULD NOT contain fractional parts [2]\nThe Server MUST reply with the same value in the Response object if included. This member is used to correlate the context between the two objects.\n[1] The use of Null as a value for the id member in a Request object is discouraged, because this specification uses a value of Null for Responses with an unknown id. Also, because JSON-RPC 1.0 uses an id value of Null for Notifications this could cause confusion in handling.\n[2] Fractional parts may be problematic, since many decimal fractions cannot be represented exactly as binary fractions."
                }
            },
            "required": [ "id" ]
        },
        "has_params": {
            "properties": {
                "params": {
                    "type": ["array", "object"],
                    "description": "A Structured value that holds the parameter values to be used during the invocation of the method. This member MAY be omitted."
                }
            }
        },
        "notification_or_request": {
            "allOf": [{
                "$ref": "#/definitions/base"
            },{
                "$ref": "#/definitions/has_method"
            }]
        },
        "notification": {
            "allOf": [{
                "$ref": "#/definitions/base"
            },{
                "$ref": "#/definitions/has_method"
            },{
                "not": { 
                    "$ref": "#/definitions/has_id"
                }
            }]
        },
        "request": {
            "allOf": [{
                "$ref": "#/definitions/base"
            },{
                "$ref": "#/definitions/has_method"
            },{
                "$ref": "#/definitions/has_id"
            }]
            
        }
    },
    "oneOf": [{
        "title": "Request",
        "$ref": "#/definitions/request"
    },{
        "title": "Notification",
        "$ref": "#/definitions/notification"
    },{
        "title": "Batch request",
        "type": "array",
        "minItems": 1,
        "items": {
            "$ref": "#/definitions/request"
        }
    },{
        "title": "Batch notification",
        "type": "array",
        "minItems": 1,
        "items": {
            "$ref": "#/definitions/notification"
        }
    }]
}
)";
    constexpr std::string_view
json_rpc_response_schema = R"(
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "$id": "htpp://calcul-isto.cnrs-orleans.fr/schemas/json_rpc_response.schema.json#",
    "title":   "JSON-RPC 2.0 Specification",
    "definitions": {
        "base": {
            "type": "object",
            "properties": {
                "jsonrpc": {
                    "type": "string",
                    "const": "2.0",
                    "description": "A String specifying the version of the JSON-RPC protocol. MUST be exactly \"2.0\"."
                }
            },
            "required": [ "jsonrpc" ]
        },
        "has_id": {
            "properties": {
                "id": {
                    "type": ["string", "number", "null"],
                    "description": "This member is REQUIRED. It MUST be the same as the value of the id member in the Request Object. If there was an error in detecting the id in the Request object (e.g. Parse error/Invalid Request), it MUST be Null."
                }
            },
            "required": [ "id" ]
        },
        "has_result": {
            "properties": {
                "result": {
                    "description": "This member is REQUIRED on success. This member MUST NOT exist if there was an error invoking the method. The value of this member is determined by the method invoked on the Server."
                }
            },
            "required": [ "result" ]
        },
        "has_error": {
            "properties": {
                "error": {
                    "description": "This member is REQUIRED on error. This member MUST NOT exist if there was no error triggered during invocation. The value for this member MUST be an Object as defined in section 5.1.",
                    "type": "object",
                    "properties": {
                        "code": {
                            "type": "integer",
                            "description": "A Number that indicates the error type that occurred. This MUST be an integer."
                        },
                        "message": {
                            "type": "string",
                            "description": "A String providing a short description of the error. The message SHOULD be limited to a concise single sentence."
                        },
                        "data": {
                            "description": "A Primitive or Structured value that contains additional information about the error. This may be omitted. The value of this member is defined by the Server (e.g. detailed error information, nested errors etc.)."
                        }
                    },
                    "required": [ "code", "message" ]
                }
            },
            "required": [ "error" ]
        },
        "response": {
            "allOf": [{
                "$ref": "#/definitions/base"
            },{
                "$ref": "#/definitions/has_id"
            },{
                "oneOf": [{
                    "$ref": "#/definitions/has_result"
                },{
                    "$ref": "#/definitions/has_error"
                }]
            }]
        }
    },
    "oneOf": [{
        "title": "Response",
        "$ref": "#/definitions/response"
    },{
        "title": "Batch response",
        "type": "array",
        "minItems": 1,
        "items": {
            "$ref": "#/definitions/response"
        }
    }]
}
)";
} // namespace isto::remote_services::schemas
