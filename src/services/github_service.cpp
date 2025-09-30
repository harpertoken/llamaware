#include "services/github_service.h"
#include "services/web_service.h"
#include <iostream>
#include <sstream>
#include <regex>
#include <vector>
#include <map>
#include <string>
#include <nlohmann/json.hpp>

namespace Services {

GitHubService::GitHubService() {
    // Constructor
}

std::string GitHubService::get_github_token() {
    const char* token = std::getenv("GITHUB_TOKEN");
    if (token) {
        return std::string(token);
    }
    return "";
}

std::string GitHubService::get_api_base_url() {
    return "https://api.github.com";
}

WebResponse GitHubService::make_github_request(const std::string& endpoint,
                                             const std::string& /*method*/,
                                             const std::string& body) {
    std::string url = get_api_base_url() + endpoint;
    HeaderMap headers;

    std::string token = get_github_token();
    if (!token.empty()) {
        headers["Authorization"] = "Bearer " + token;
    }
    headers["Accept"] = "application/vnd.github.v3+json";
    headers["User-Agent"] = "llamaware-agent/1.0";

    if (!body.empty()) {
        headers["Content-Type"] = "application/json";
    }

    return WebService::fetch_with_headers(url, headers);
}

std::string GitHubService::get_repo_info(const std::string& owner, const std::string& repo) {
    std::string endpoint = "/repos/" + owner + "/" + repo;
    WebResponse response = make_github_request(endpoint);

    if (response.success) {
        try {
            nlohmann::json json = nlohmann::json::parse(response.content);
            std::stringstream ss;
            ss << "Repository: " << json["full_name"] << "\n";
            ss << "Description: " << json.value("description", "No description") << "\n";
            ss << "Stars: " << json.value("stargazers_count", 0) << "\n";
            ss << "Forks: " << json.value("forks_count", 0) << "\n";
            ss << "Language: " << json.value("language", "Unknown") << "\n";
            return ss.str();
        } catch (const std::exception& e) {
            return "Error parsing repository info: " + std::string(e.what());
        }
    } else {
        return "Failed to fetch repository info: " + response.error_message;
    }
}

std::vector<GitHubIssue> GitHubService::get_issues(const std::string& owner,
                                                 const std::string& repo,
                                                 const std::string& state) {
    std::vector<GitHubIssue> issues;
    std::string endpoint = "/repos/" + owner + "/" + repo + "/issues?state=" + state;

    WebResponse response = make_github_request(endpoint);

    if (response.success) {
        try {
            nlohmann::json json = nlohmann::json::parse(response.content);
            for (const auto& item : json) {
                GitHubIssue issue;
                issue.number = item["number"];
                issue.title = item["title"];
                issue.body = item.value("body", "");
                issue.state = item["state"];

                for (const auto& label : item["labels"]) {
                    issue.labels.push_back(label["name"]);
                }

                if (!item["assignee"].is_null()) {
                    issue.assignee = item["assignee"]["login"];
                }

                if (!item["milestone"].is_null()) {
                    issue.milestone = item["milestone"]["number"];
                }

                issues.push_back(issue);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing issues: " << e.what() << std::endl;
        }
    }

    return issues;
}

GitHubIssue GitHubService::create_issue(const std::string& owner,
                                      const std::string& repo,
                                      const std::string& title,
                                      const std::string& body,
                                      const std::vector<std::string>& labels) {
    (void)owner; (void)repo;
    GitHubIssue issue;
    issue.title = title;
    issue.body = body;
    issue.labels = labels;
    issue.state = "open";
    return issue;
}

bool GitHubService::update_issue(const std::string& owner,
                               const std::string& repo,
                               int issue_number,
                               const std::map<std::string, std::string>& updates) {
    (void)owner; (void)repo; (void)issue_number; (void)updates;
    return false;
}

bool GitHubService::add_comment(const std::string& owner,
                              const std::string& repo,
                              int issue_number,
                              const std::string& comment) {
    (void)owner; (void)repo; (void)issue_number; (void)comment;
    return false;
}

std::vector<GitHubPR> GitHubService::get_pull_requests(const std::string& owner,
                                                     const std::string& repo,
                                                     const std::string& state) {
    std::vector<GitHubPR> prs;
    std::string endpoint = "/repos/" + owner + "/" + repo + "/pulls?state=" + state;

    WebResponse response = make_github_request(endpoint);

    if (response.success) {
        try {
            nlohmann::json json = nlohmann::json::parse(response.content);
            for (const auto& item : json) {
                GitHubPR pr;
                pr.number = item["number"];
                pr.title = item["title"];
                pr.body = item.value("body", "");
                pr.state = item["state"];
                pr.head_branch = item["head"]["ref"];
                pr.base_branch = item["base"]["ref"];
                pr.mergeable = item.value("mergeable", false);
                prs.push_back(pr);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing PRs: " << e.what() << std::endl;
        }
    }

    return prs;
}

GitHubPR GitHubService::get_pull_request(const std::string& owner,
                                       const std::string& repo,
                                       int pr_number) {
    GitHubPR pr;
    std::string endpoint = "/repos/" + owner + "/" + repo + "/pulls/" + std::to_string(pr_number);

    WebResponse response = make_github_request(endpoint);

    if (response.success) {
        try {
            nlohmann::json json = nlohmann::json::parse(response.content);
            pr.number = json["number"];
            pr.title = json["title"];
            pr.body = json.value("body", "");
            pr.state = json["state"];
            pr.head_branch = json["head"]["ref"];
            pr.base_branch = json["base"]["ref"];
            pr.mergeable = json.value("mergeable", false);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing PR: " << e.what() << std::endl;
        }
    }

    return pr;
}

bool GitHubService::create_pr_comment(const std::string& owner,
                                    const std::string& repo,
                                    int pr_number,
                                    const std::string& comment) {
    (void)owner; (void)repo; (void)pr_number; (void)comment;
    return false;
}

std::vector<std::string> GitHubService::get_milestones(const std::string& owner,
                                                     const std::string& repo) {
    std::vector<std::string> milestones;
    std::string endpoint = "/repos/" + owner + "/" + repo + "/milestones";

    WebResponse response = make_github_request(endpoint);

    if (response.success) {
        try {
            nlohmann::json json = nlohmann::json::parse(response.content);
            for (const auto& item : json) {
                milestones.push_back(item["title"]);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing milestones: " << e.what() << std::endl;
        }
    }

    return milestones;
}

int GitHubService::create_milestone(const std::string& owner,
                                  const std::string& repo,
                                  const std::string& title,
                                  const std::string& description) {
    (void)owner; (void)repo; (void)title; (void)description;
    return -1;
}

std::vector<std::string> GitHubService::get_labels(const std::string& owner,
                                                 const std::string& repo) {
    std::vector<std::string> labels;
    std::string endpoint = "/repos/" + owner + "/" + repo + "/labels";

    WebResponse response = make_github_request(endpoint);

    if (response.success) {
        try {
            nlohmann::json json = nlohmann::json::parse(response.content);
            for (const auto& item : json) {
                labels.push_back(item["name"]);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing labels: " << e.what() << std::endl;
        }
    }

    return labels;
}

bool GitHubService::create_label(const std::string& owner,
                               const std::string& repo,
                               const std::string& name,
                               const std::string& color,
                               const std::string& description) {
    (void)owner; (void)repo; (void)name; (void)color; (void)description;
    return false;
}

bool GitHubService::create_webhook(const std::string& owner,
                                 const std::string& repo,
                                 const std::string& url,
                                 const std::vector<std::string>& events) {
    (void)owner; (void)repo; (void)url; (void)events;
    return false;
}

std::string GitHubService::run_health_check(const std::string& owner,
                                          const std::string& repo) {
    std::stringstream ss;
    ss << "# Health Check Report for " << owner << "/" << repo << "\n\n";

    ss << "## Repository Information\n";
    ss << get_repo_info(owner, repo) << "\n";

    auto issues = get_issues(owner, repo, "open");
    ss << "## Issues\n";
    ss << "Open issues: " << issues.size() << "\n\n";

    auto prs = get_pull_requests(owner, repo, "open");
    ss << "## Pull Requests\n";
    ss << "Open PRs: " << prs.size() << "\n\n";

    auto milestones = get_milestones(owner, repo);
    ss << "## Milestones\n";
    ss << "Active milestones: " << milestones.size() << "\n";
    for (const auto& milestone : milestones) {
        ss << "- " << milestone << "\n";
    }

    return ss.str();
}

std::string GitHubService::find_related_issues(const std::string& owner,
                                             const std::string& repo,
                                             const std::string& text) {
    auto issues = get_issues(owner, repo, "all");
    std::stringstream ss;
    ss << "Related issues found:\n";

    std::regex word_regex("\\b\\w+\\b");
    std::vector<std::string> keywords;
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), word_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::string word = (*i).str();
        if (word.length() > 3) {
            keywords.push_back(word);
        }
    }

    int found_count = 0;
    for (const auto& issue : issues) {
        bool related = false;
        for (const auto& keyword : keywords) {
            if (issue.title.find(keyword) != std::string::npos ||
                issue.body.find(keyword) != std::string::npos) {
                related = true;
                break;
            }
        }

        if (related) {
            ss << "- #" << issue.number << ": " << issue.title << "\n";
            found_count++;
            if (found_count >= 5) break;
        }
    }

    if (found_count == 0) {
        ss << "No related issues found.";
    }

    return ss.str();
}

bool GitHubService::is_available() {
    std::string token = get_github_token();
    return !token.empty() && WebService::is_available();
}

} // namespace Services