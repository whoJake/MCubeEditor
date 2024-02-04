#pragma once

#include "hashers.h"
#include "logging/jclog.h"

/*
 * IF THERE ARE COMPILE ERRORS EG.
 * CG2660 'std::pair<a, b>::pair': function does not take 2 arguments.
 * THEN ITS LIKELY A MISSING COPY CONSTRUCTOR ON ONE OF THE RESOURCES ( Resource::Resource(Resource&&) )
 */

template<typename T, typename... A>
T& request_resource(const jclog::Log& log, std::unordered_map<size_t, T>& resources, A&... args)
{
    size_t hash{ 0 };
    hash_params(hash, args...);

    auto it = resources.find(hash);
    if( it != resources.end() )
        return (*it).second;

    // Could be reduced to a single .emplace call but its useful to know when an object is being created before creating it

    JCLOG_TRACK(log, "Generating #{} ( {} ) cache object", resources.size(), typeid(T).name());
    T resource(args...);
    auto ins_it = resources.emplace(hash, std::move(resource));
    return (ins_it.first)->second;
}

template<typename T, typename... A>
T& request_resource(std::unordered_map<size_t, T>& resources, A&... args)
{
    return request_resource(jclog::Log(), resources, args...);
}