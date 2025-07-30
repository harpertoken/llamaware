#include <iostream>
#include <string>
#include <cstdlib>

#include "core/agent.h"
#include "utils/ui.h"
#include "utils/config.h"

int main() {
    try {
        // Initialize configuration
        Utils::Config::load_environment();
        
        // Display welcome screen
        Utils::UI::print_logo();
        
        // Create and run agent
        Core::Agent agent;
        agent.run();
        
    } catch (const std::exception& e) {
        std::cerr << Utils::UI::RED << "Fatal error: " << e.what() << Utils::UI::RESET << std::endl;
        return 1;
    }
    
    return 0;
}