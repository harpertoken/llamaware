#ifndef DATABASE_SERVICE_H
#define DATABASE_SERVICE_H

#include <string>
#include <memory>
#include <vector>
#ifdef HAVE_PQXX
#include <pqxx/pqxx>
#endif

#ifndef HAVE_PQXX
namespace pqxx {
class result {
public:
    result() {}
    size_t size() const { return 0; }
    // Minimal interface for compatibility
};
class connection {
public:
    connection(const std::string&) {}
    bool is_open() const { return false; }
    std::string dbname() const { return ""; }
};
} // namespace pqxx
#endif

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
#ifdef HAVE_PQXX
    std::unique_ptr<pqxx::connection> connection_;
#endif
};

} // namespace llamaware

#endif // DATABASE_SERVICE_H