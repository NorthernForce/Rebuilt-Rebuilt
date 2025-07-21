#pragma once

#include <string>

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

#include <logging/Logger.h>

template<>
inline void nfr::Log(const nfr::LogContext& logContext, const GitMetadata& metadata)
{
    logContext["branch"] << metadata.branch;
    logContext["build_host"] << metadata.build_host;
    logContext["build_user_email"] << metadata.build_user_email;
    logContext["build_user_name"] << metadata.build_user_name;
    logContext["build_version"] << metadata.build_version;
    logContext["closest_tag_commit_count"] << metadata.closest_tag_commit_count;
    logContext["closest_tag_name"] << metadata.closest_tag_name;
    logContext["commit_id"] << metadata.commit_id;
    logContext["commit_id_abbrev"] << metadata.commit_id_abbrev;
    logContext["commit_id_describe"] << metadata.commit_id_describe;
    logContext["commit_message_full"] << metadata.commit_message_full;
    logContext["commit_message_short"] << metadata.commit_message_short;
    logContext["commit_time"] << metadata.commit_time;
    logContext["commit_user_email"] << metadata.commit_user_email;
    logContext["commit_user_name"] << metadata.commit_user_name;
    logContext["dirty"] << metadata.dirty;
    logContext["remote_origin_url"] << metadata.remote_origin_url;
    logContext["tags"] << metadata.tags;
    logContext["total_commit_count"] << metadata.total_commit_count;
}

extern GitMetadata loadGitMetadata(const std::string& filePath);

#include <frc/Filesystem.h>
inline const GitMetadata& getGitMetadata()
{
    static const GitMetadata metadata = loadGitMetadata(frc::filesystem::GetDeployDirectory() + "/git.properties");
    return metadata;
}