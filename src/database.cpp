#include "database.h"
#include "server/entry.h"
#include "core/filesystem.h"
#include <stddef.h>
#include <stdio.h>

void database::initialize_database()
{
  core::create_directory(server::configuration()["database_path"].c_str());
}
