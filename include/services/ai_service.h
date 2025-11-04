#pragma once
#include "core/agent.h"
#include <nlohmann/json.hpp>
#include <string>

namespace Services {
class AIService {
private:
  Core::Agent::Mode mode_;
  std::string api_key_;

  [[nodiscard]] bool is_online_mode() const;
  nlohmann::json create_standard_payload(const std::string &model,
                                         const std::string &user_input,
                                         const std::string &context);
  nlohmann::json create_payload(const std::string &user_input,
                                const std::string &context);
  std::string get_api_url();
  std::string parse_cerebras_stream(const std::string &response);

public:
  AIService(Core::Agent::Mode mode, const std::string &api_key = "");

  std::string chat(const std::string &user_input, const std::string &context);
  bool is_available();
};
} // namespace Services
