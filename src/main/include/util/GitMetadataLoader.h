#pragma once

#include <string>

/**
 * @brief Structure containing information about the code version deployed to robot
 * 
 * This data helps us know exactly what version of code is running on the robot,
 * which is crucial for debugging and ensuring everyone is using the same code
 * during competitions.
 * 
 * ## Why This Matters:
 * During competitions, teams might deploy code multiple times with small changes.
 * Without version tracking, it's impossible to know if a problem is due to:
 * - Old code still running
 * - Different code on practice vs competition robot
 * - Code changes that weren't properly tested
 * 
 * This metadata is automatically generated during the build process.
 */
struct GitMetadata
{
    std::string branch;                     ///< Git branch name (e.g., "main", "competition")
    std::string build_host;                 ///< Computer that built this code
    std::string build_user_email;           ///< Email of person who built this code
    std::string build_user_name;            ///< Name of person who built this code
    std::string build_version;              ///< Build version identifier
    std::string closest_tag_commit_count;   ///< How many commits since last tag
    std::string closest_tag_name;           ///< Name of nearest version tag
    std::string commit_id;                  ///< Full commit hash (unique identifier)
    std::string commit_id_abbrev;           ///< Short commit hash (first 7 characters)
    std::string commit_id_describe;         ///< Human-readable commit description
    std::string commit_message_full;        ///< Complete commit message
    std::string commit_message_short;       ///< First line of commit message
    std::string commit_time;                ///< When this commit was made
    std::string commit_user_email;          ///< Email of person who made this commit
    std::string commit_user_name;           ///< Name of person who made this commit
    bool dirty;                             ///< True if uncommitted changes existed during build
    std::string remote_origin_url;          ///< Git repository URL
    std::string tags;                       ///< Any tags associated with this commit
    int total_commit_count;                 ///< Total number of commits in repository
};

#include <logging/Logger.h>

/**
 * @brief Logs git metadata to the logging system
 * 
 * This function integrates GitMetadata with our custom logging system.
 * It's called automatically to record version information in the logs.
 * 
 * @param logContext Where to write the git information
 * @param metadata The git metadata to log
 */
inline void Log(const nfr::LogContext& logContext, const GitMetadata& metadata)
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

/**
 * @brief Loads git metadata from a properties file
 * 
 * During the build process, Gradle generates a git.properties file containing
 * version information. This function parses that file and returns the data.
 * 
 * @param filePath Path to the git.properties file
 * @return GitMetadata structure with loaded information
 * @throws std::runtime_error if file cannot be opened
 */
extern GitMetadata loadGitMetadata(const std::string& filePath);

#include <frc/Filesystem.h>

/**
 * @brief Gets git metadata for the currently deployed code
 * 
 * This function provides access to git information about the code currently
 * running on the robot. It automatically loads the data from the deploy
 * directory where git.properties is stored.
 * 
 * Uses a static variable so the file is only read once for efficiency.
 * 
 * @return Reference to GitMetadata for current deployment
 */
inline const GitMetadata& getGitMetadata()
{
    static const GitMetadata metadata = loadGitMetadata(
        frc::filesystem::GetDeployDirectory() + "/git.properties");
    return metadata;
}