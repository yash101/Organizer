#ifndef DATABASE_H
#define DATABASE_H
#include <string>
namespace database
{
  void initialize_database();
  std::string getDatabasePath(std::string p);
}
#endif // DATABASE_H
