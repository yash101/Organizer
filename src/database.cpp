#include "database.h"
#include "server/entry.h"
#include "core/filesystem.h"
#include <stddef.h>
#include <stdio.h>

std::string database::getDatabasePath(std::string p)
{
  if(p.front() == '/')
    return server::configuration()["database_path"] + p.substr(1, p.size());
  else
    return server::configuration()["database_path"] + p;
}

void database::initialize_database()
{
  core::create_directory(server::configuration()["database_path"].c_str());
  core::create_file(database::getDatabasePath("/authentication_keys.dat").c_str());
  core::create_file(database::getDatabasePath("/users.dat").c_str());
}
