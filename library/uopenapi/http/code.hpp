#pragma once

#include <cstddef>
#include <userver/server/http/http_status.hpp>

namespace uopenapi::http{
    template <size_t I>
    struct status_code
    {
        static_assert(I && false, "Unknowned code");
    };

    template <size_t I>
    constexpr userver::server::http::HttpStatus status_code_v = status_code<I>::value;

#define CODE_IMPL(ENUM_VALUE, INDEX)                                          \
    template <>                                                               \
    struct status_code<INDEX> : std::integral_constant<                              \
                             userver::server::http::HttpStatus,               \
                             userver::server::http::HttpStatus::ENUM_VALUE>   \
    {                                                                         \
        static_assert(INDEX ==                                                \
                          static_cast<int>(                                   \
                              userver::server::http::HttpStatus::ENUM_VALUE), \
                      "ENUM_VALUE value MUST be equal to INDEX");             \
    }

    CODE_IMPL(kContinue, 100);
    CODE_IMPL(kSwitchingProtocols, 101);
    CODE_IMPL(kProcessing, 102);
    CODE_IMPL(kOk, 200);
    CODE_IMPL(kCreated, 201);
    CODE_IMPL(kAccepted, 202);
    CODE_IMPL(kNonAuthoritativeInformation, 203);
    CODE_IMPL(kNoContent, 204);
    CODE_IMPL(kResetContent, 205);
    CODE_IMPL(kPartialContent, 206);
    CODE_IMPL(kMultiStatus, 207);
    CODE_IMPL(kAlreadyReported, 208);
    CODE_IMPL(kImUsed, 226);
    CODE_IMPL(kMultipleChoices, 300);
    CODE_IMPL(kMovedPermanently, 301);
    CODE_IMPL(kFound, 302);
    CODE_IMPL(kSeeOther, 303);
    CODE_IMPL(kNotModified, 304);
    CODE_IMPL(kUseProxy, 305);
    CODE_IMPL(kTemporaryRedirect, 307);
    CODE_IMPL(kPermanentRedirect, 308);
    CODE_IMPL(kBadRequest, 400);
    CODE_IMPL(kUnauthorized, 401);
    CODE_IMPL(kPaymentRequired, 402);
    CODE_IMPL(kForbidden, 403);
    CODE_IMPL(kNotFound, 404);
    CODE_IMPL(kMethodNotAllowed, 405);
    CODE_IMPL(kNotAcceptable, 406);
    CODE_IMPL(kProxyAuthenticationRequired, 407);
    CODE_IMPL(kRequestTimeout, 408);
    CODE_IMPL(kConflict, 409);
    CODE_IMPL(kGone, 410);
    CODE_IMPL(kLengthRequired, 411);
    CODE_IMPL(kPreconditionFailed, 412);
    CODE_IMPL(kPayloadTooLarge, 413);
    CODE_IMPL(kUriTooLong, 414);
    CODE_IMPL(kUnsupportedMediaType, 415);
    CODE_IMPL(kRangeNotSatisfiable, 416);
    CODE_IMPL(kExpectationFailed, 417);
    CODE_IMPL(kMisdirectedRequest, 421);
    CODE_IMPL(kUnprocessableEntity, 422);
    CODE_IMPL(kLocked, 423);
    CODE_IMPL(kFailedDependency, 424);
    CODE_IMPL(kUpgradeRequired, 426);
    CODE_IMPL(kPreconditionRequired, 428);
    CODE_IMPL(kTooManyRequests, 429);
    CODE_IMPL(kRequestHeaderFieldsTooLarge, 431);
    CODE_IMPL(kUnavailableForLegalReasons, 451);
    CODE_IMPL(kClientClosedRequest, 499);
    CODE_IMPL(kInternalServerError, 500);
    CODE_IMPL(kNotImplemented, 501);
    CODE_IMPL(kBadGateway, 502);
    CODE_IMPL(kServiceUnavailable, 503);
    CODE_IMPL(kGatewayTimeout, 504);
    CODE_IMPL(kHttpVersionNotSupported, 505);
    CODE_IMPL(kVariantAlsoNegotiates, 506);
    CODE_IMPL(kInsufficientStorage, 507);
    CODE_IMPL(kLoopDetected, 508);
    CODE_IMPL(kNotExtended, 510);
    CODE_IMPL(kNetworkAuthenticationRequired, 511);

#undef CODE_IMPL
}