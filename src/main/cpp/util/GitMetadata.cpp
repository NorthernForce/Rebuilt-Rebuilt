#include "util/GitMetadataLoader.h"
#include <fstream>
#include <sstream>

GitMetadata loadGitMetadata(const std::string& filePath) {
    GitMetadata metadata;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open git metadata file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos) continue; // Skip lines without '='

        std::string key = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);

        if (key == "git.branch") {
            metadata.branch = value;
        } else if (key == "git.build.host") {
            metadata.build_host = value;
        } else if (key == "git.build.user.email") {
            metadata.build_user_email = value;
        } else if (key == "git.build.user.name") {
            metadata.build_user_name = value;
        } else if (key == "git.build.version") {
            metadata.build_version = value;
        } else if (key == "git.closest.tag.commit.count") {
            metadata.closest_tag_commit_count = value;
        } else if (key == "git.closest.tag.name") {
            metadata.closest_tag_name = value;
        } else if (key == "git.commit.id") {
            metadata.commit_id = value;
        } else if (key == "git.commit.id.abbrev") {
            metadata.commit_id_abbrev = value;
        } else if (key == "git.commit.id.describe") {
            metadata.commit_id_describe = value;
        } else if (key == "git.commit.message.full") {
            metadata.commit_message_full = value;
        } else if (key == "git.commit.message.short") {
            metadata.commit_message_short = value;
        } else if (key == "git.commit.time") {
            metadata.commit_time = value;
        } else if (key == "git.commit.user.email") {
            metadata.commit_user_email = value;
        } else if (key == "git.commit.user.name") {
            metadata.commit_user_name = value;
        } else if (key == "git.dirty") {
            metadata.dirty = (value == "true");
        } else if (key == "git.remote.origin.url") {
            metadata.remote_origin_url = value;
        } else if (key == "git.tags") {
            metadata.tags = value;
        } else if (key == "git.total.commit.count") {
            try {
                metadata.total_commit_count = std::stoi(value);
            } catch (const std::invalid_argument&) {
                metadata.total_commit_count = 0; // Default to 0 if conversion fails
            } catch (const std::out_of_range&) {
                metadata.total_commit_count = 0; // Default to 0 if value is out of range
            }
        }
    }
    file.close();
    return metadata;
}