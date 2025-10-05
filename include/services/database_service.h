#ifndef DATABASE_SERVICE_H
#define DATABASE_SERVICE_H

#include <string>
#include <memory>
#include <vector>
#include <pqxx/pqxx>
#include <pqxx/version.hxx>

namespace llamaware {

class DatabaseService {
public:
    DatabaseService();
    ~DatabaseService();

    bool connect(const std::string& host, int port, const std::string& dbname,
                 const std::string& user, const std::string& password);
    void disconnect();

    bool isConnected() const;

    // Example methods - extend as needed
    bool executeQuery(const std::string& query, const std::vector<std::string>& params = {});
    std::unique_ptr<pqxx::result> executeSelect(const std::string& query, const std::vector<std::string>& params = {});

private:
    std::unique_ptr<pqxx::connection> connection_;
};

} // namespace llamaware

#endif // DATABASE_SERVICE_H