/**********************************************************\
|                                                          |
|                          hprose                          |
|                                                          |
| Official WebSite: http://www.hprose.com/                 |
|                   http://www.hprose.org/                 |
|                                                          |
\**********************************************************/

/**********************************************************\
 *                                                        *
 * hprose/http/Status.h                                   *
 *                                                        *
 * hprose http status for cpp.                            *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

namespace hprose {
namespace http {

const int StatusContinue                      = 100;
const int StatusSwitchingProtocols            = 101;
const int StatusProcessing                    = 102;

const int StatusOK                            = 200;
const int StatusCreated                       = 201;
const int StatusAccepted                      = 202;
const int StatusNonAuthoritativeInfo          = 203;
const int StatusNoContent                     = 204;
const int StatusResetContent                  = 205;
const int StatusPartialContent                = 206;
const int StatusMultiStatus                   = 207;
const int StatusAlreadyReported               = 208;
const int StatusIMUsed                        = 226;

const int StatusMultipleChoices               = 300;
const int StatusMovedPermanently              = 301;
const int StatusFound                         = 302;
const int StatusSeeOther                      = 303;
const int StatusNotModified                   = 304;
const int StatusUseProxy                      = 305;
const int StatusTemporaryRedirect             = 307;
const int StatusPermanentRedirect             = 308;

const int StatusBadRequest                    = 400;
const int StatusUnauthorized                  = 401;
const int StatusPaymentRequired               = 402;
const int StatusForbidden                     = 403;
const int StatusNotFound                      = 404;
const int StatusMethodNotAllowed              = 405;
const int StatusNotAcceptable                 = 406;
const int StatusProxyAuthRequired             = 407;
const int StatusRequestTimeout                = 408;
const int StatusConflict                      = 409;
const int StatusGone                          = 410;
const int StatusLengthRequired                = 411;
const int StatusPreconditionFailed            = 412;
const int StatusRequestEntityTooLarge         = 413;
const int StatusRequestURITooLong             = 414;
const int StatusUnsupportedMediaType          = 415;
const int StatusRequestedRangeNotSatisfiable  = 416;
const int StatusExpectationFailed             = 417;
const int StatusTeapot                        = 418;
const int StatusUnprocessableEntity           = 422;
const int StatusLocked                        = 423;
const int StatusFailedDependency              = 424;
const int StatusUpgradeRequired               = 426;
const int StatusPreconditionRequired          = 428;
const int StatusTooManyRequests               = 429;
const int StatusRequestHeaderFieldsTooLarge   = 431;
const int StatusUnavailableForLegalReasons    = 451;

const int StatusInternalServerError           = 500;
const int StatusNotImplemented                = 501;
const int StatusBadGateway                    = 502;
const int StatusServiceUnavailable            = 503;
const int StatusGatewayTimeout                = 504;
const int StatusHTTPVersionNotSupported       = 505;
const int StatusVariantAlsoNegotiates         = 506;
const int StatusInsufficientStorage           = 507;
const int StatusLoopDetected                  = 508;
const int StatusNotExtended                   = 510;
const int StatusNetworkAuthenticationRequired = 511;

}
} // hprose::http
