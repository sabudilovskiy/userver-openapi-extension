#pragma once

#include <uopenapi/enum/postgres/enumerator_func.hpp>
#include <userver/storages/postgres/io/enum_types.hpp>
#include <uopenapi/utils/constexpr_string.hpp>

namespace uopenapi{
template <typename T, utils::ce::string PostgresName>
requires has_introspector<T> struct pg_enum_mapper
    : userver::storages::postgres::io::EnumMappingBase<T>
{
    static constexpr userver::storages::postgres::DBTypeName postgres_name =
        PostgresName.data();
    static constexpr userver::utils::TrivialBiMap enumerators =
        create_enumerator_func<T>();
};

}