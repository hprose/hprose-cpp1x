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

const StatusContinue                      = 100;
const StatusSwitchingProtocols            = 101;
const StatusProcessing                    = 102;

const StatusOK                            = 200;
const StatusCreated                       = 201;
const StatusAccepted                      = 202;
const StatusNonAuthoritativeInfo          = 203;
const StatusNoContent                     = 204;
const StatusResetContent                  = 205;
const StatusPartialContent                = 206;
const StatusMultiStatus                   = 207;
const StatusAlreadyReported               = 208;
const StatusIMUsed                        = 226;

const StatusMultipleChoices               = 300;
const StatusMovedPermanently              = 301;
const StatusFound                         = 302;
const StatusSeeOther                      = 303;
const StatusNotModified                   = 304;
const StatusUseProxy                      = 305;
const StatusTemporaryRedirect             = 307;
const StatusPermanentRedirect             = 308;

const StatusBadRequest                    = 400;
const StatusUnauthorized                  = 401;
const StatusPaymentRequired               = 402;
const StatusForbidden                     = 403;
const StatusNotFound                      = 404;
const StatusMethodNotAllowed              = 405;
const StatusNotAcceptable                 = 406;
const StatusProxyAuthRequired             = 407;
const StatusRequestTimeout                = 408;
const StatusConflict                      = 409;
const StatusGone                          = 410;
const StatusLengthRequired                = 411;
const StatusPreconditionFailed            = 412;
const StatusRequestEntityTooLarge         = 413;
const StatusRequestURITooLong             = 414;
const StatusUnsupportedMediaType          = 415;
const StatusRequestedRangeNotSatisfiable  = 416;
const StatusExpectationFailed             = 417;
const StatusTeapot                        = 418;
const StatusUnprocessableEntity           = 422;
const StatusLocked                        = 423;
const StatusFailedDependency              = 424;
const StatusUpgradeRequired               = 426;
const StatusPreconditionRequired          = 428;
const StatusTooManyRequests               = 429;
const StatusRequestHeaderFieldsTooLarge   = 431;
const StatusUnavailableForLegalReasons    = 451;

const StatusInternalServerError           = 500;
const StatusNotImplemented                = 501;
const StatusBadGateway                    = 502;
const StatusServiceUnavailable            = 503;
const StatusGatewayTimeout                = 504;
const StatusHTTPVersionNotSupported       = 505;
const StatusVariantAlsoNegotiates         = 506;
const StatusInsufficientStorage           = 507;
const StatusLoopDetected                  = 508;
const StatusNotExtended                   = 510;
const StatusNetworkAuthenticationRequired = 511;

}
} // hprose::http
