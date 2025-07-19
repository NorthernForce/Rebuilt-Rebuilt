#pragma once

#include <string>
#include <rfl.hpp>

struct GitMetadata
{
    std::string branch;
    std::string build_host;
    std::string build_user_email;
    std::string build_user_name;
    std::string build_version;
    std::string closest_tag_commit_count;
    std::string closest_tag_name;
    std::string commit_id;
    std::string commit_id_abbrev;
    std::string commit_id_describe;
    std::string commit_message_full;
    std::string commit_message_short;
    std::string commit_time;
    std::string commit_user_email;
    std::string commit_user_name;
    bool dirty;
    std::string remote_origin_url;
    std::string tags;
    int total_commit_count;
};

extern GitMetadata loadGitMetadata(const std::string& filePath = "git.properties");
inline const GitMetadata& getGitMetadata()
{
    static const GitMetadata metadata = loadGitMetadata();
    return metadata;
}