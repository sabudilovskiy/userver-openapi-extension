#include <uopenapi/all.hpp>
#include <userver/formats/serialize/boost_uuid.hpp>
#include <userver/utest/utest.hpp>

UTEST(bugs, UuidSerializationAmibigous) {
    userver::formats::json::ValueBuilder test =
        userver::formats::json::Type::kObject;
    boost::uuids::uuid s;
    test["ss"] = s;
}
