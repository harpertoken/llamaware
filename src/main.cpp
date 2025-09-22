#include <iostream>
#include <string>
#include <cstdlib>

#include "core/agent.h"
#include "utils/ui.h"
#include "utils/config.h"

int main() {
    try {
        // Initialize configuration (reads env vars, .env, etc.)
        Utils::Config::load_environment();

        // Check for test mode is handled in Agent::initialize_mode()

        // Display welcome screen
        Utils::UI::print_logo();

        // Create agent
        Core::Agent agent;

        // Run agent
        agent.run();

        std::cout << "Agent run completed" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << Utils::Color::RED << "Fatal error: " << e.what() << Utils::Color::RESET << std::endl;
        return 1;
    }

    return 0;
}
