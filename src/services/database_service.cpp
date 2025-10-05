#include "services/database_service.h"
#include <iostream>
#include <vector>

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

        // The pqxx::connection constructor throws on failure, so if we get here,
        // the connection is open.
        std::cout << "Connected to database: " << connection_->dbname() << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Database connection error: " << e.what() << std::endl;
        return false;
    }
}

void DatabaseService::disconnect() {
    connection_.reset();
}

bool DatabaseService::isConnected() const {
    return connection_ && connection_->is_open();
}

bool DatabaseService::executeQuery(const std::string& query, const std::vector<std::string>& params) {
    if (!isConnected()) {
        std::cerr << "Not connected to database" << std::endl;
        return false;
    }

    try {
        pqxx::work txn(*connection_);
#if defined(__APPLE__) || PQXX_VERSION >= 70000
        pqxx::params p;
        for (const auto& param : params) {
            p.append(param);
        }
        txn.exec(query, p);
#else
        switch (params.size()) {
            case 0: txn.exec(query); break;
            case 1: txn.exec(query, params[0]); break;
            case 2: txn.exec(query, params[0], params[1]); break;
            case 3: txn.exec(query, params[0], params[1], params[2]); break;
            case 4: txn.exec(query, params[0], params[1], params[2], params[3]); break;
            case 5: txn.exec(query, params[0], params[1], params[2], params[3], params[4]); break;
            default: throw std::runtime_error("Too many parameters");
        }
#endif
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Query execution error: " << e.what() << std::endl;
        return false;
    }
}

std::unique_ptr<pqxx::result> DatabaseService::executeSelect(const std::string& query, const std::vector<std::string>& params) {
    if (!isConnected()) {
        std::cerr << "Not connected to database" << std::endl;
        return nullptr;
    }

    try {
        pqxx::work txn(*connection_);
#if defined(__APPLE__) || PQXX_VERSION >= 70000
        pqxx::params p;
        for (const auto& param : params) {
            p.append(param);
        }
        auto result = txn.exec(query, p);
        txn.commit();
        return std::make_unique<pqxx::result>(std::move(result));
#else
        pqxx::result res;
        switch (params.size()) {
            case 0: res = txn.exec(query); break;
            case 1: res = txn.exec(query, params[0]); break;
            case 2: res = txn.exec(query, params[0], params[1]); break;
            case 3: res = txn.exec(query, params[0], params[1], params[2]); break;
            case 4: res = txn.exec(query, params[0], params[1], params[2], params[3]); break;
            case 5: res = txn.exec(query, params[0], params[1], params[2], params[3], params[4]); break;
            default: throw std::runtime_error("Too many parameters");
        }
        txn.commit();
        return std::make_unique<pqxx::result>(std::move(res));
#endif
    } catch (const std::exception& e) {
        std::cerr << "Select query error: " << e.what() << std::endl;
        return nullptr;
    }
}

} // namespace llamaware