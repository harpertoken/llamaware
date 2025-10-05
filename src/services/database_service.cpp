#include "services/database_service.h"
#include <iostream>

namespace llamaware {

DatabaseService::DatabaseService() : connection_(nullptr) {}

DatabaseService::~DatabaseService() {
    disconnect();
}

bool DatabaseService::connect(const std::string& host, int port, const std::string& dbname,
                              const std::string& user, const std::string& password) {
    try {
        std::string connection_string = "host=" + host +
                                       " port=" + std::to_string(port) +
                                       " dbname=" + dbname +
                                       " user=" + user +
                                       " password=" + password;

        connection_ = std::make_unique<pqxx::connection>(connection_string);

        if (connection_->is_open()) {
            std::cout << "Connected to database: " << connection_->dbname() << std::endl;
            return true;
        } else {
            std::cerr << "Failed to connect to database" << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Database connection error: " << e.what() << std::endl;
        return false;
    }
}

void DatabaseService::disconnect() {
    if (connection_ && connection_->is_open()) {
        connection_->close();
        connection_.reset();
    }
}

bool DatabaseService::isConnected() const {
    return connection_ && connection_->is_open();
}

bool DatabaseService::executeQuery(const std::string& query) {
    if (!isConnected()) {
        std::cerr << "Not connected to database" << std::endl;
        return false;
    }

    try {
        pqxx::work txn(*connection_);
        txn.exec(query);
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Query execution error: " << e.what() << std::endl;
        return false;
    }
}

std::unique_ptr<pqxx::result> DatabaseService::executeSelect(const std::string& query) {
    if (!isConnected()) {
        std::cerr << "Not connected to database" << std::endl;
        return nullptr;
    }

    try {
        pqxx::work txn(*connection_);
        auto result = std::make_unique<pqxx::result>(txn.exec(query));
        txn.commit();
        return result;
    } catch (const std::exception& e) {
        std::cerr << "Select query error: " << e.what() << std::endl;
        return nullptr;
    }
}

} // namespace llamaware