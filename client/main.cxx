/*
  The entry point into the file synchronization client

  Copyright (C) 2012 William A. Kennington III

  This file is part of Libsync.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <thread>
#include <iostream>

#include "../src/log.hxx"
#include "../src/config.hxx"
#include "../src/connector_sock.hxx"

int main(int argc, char ** argv)
{
  Config conf;
  std::string conf_file("client.conf");
  bool daemonize = false;
  Connector * conn = NULL;

  try
    {
      // Parse command line arguments

      // Retrieve the Configuration File
      conf.read(conf_file);

      // Setup the Global Log
      if (!daemonize)
        global_log.add_output(&std::cout);
      if (conf.exists("log_file"))
        global_log.add_output(conf.get_str("log_file"));
      if (conf.exists("log_level"))
        global_log.set_level(conf.get_int("log_level"));
      else
        global_log.set_level(Log::NOTICE);

      // Attempt to connect
      if (!conf.exists("conn") || conf.get_str("conn") == "sock")
        conn = new SockConnector;
      else
        throw "Unrecognized connector type: " + conf.get_str("conn");
    }
  catch(const char * e)
    {
      std::cerr << e << std::endl;
      return EXIT_FAILURE;
    }
  catch(const std::string & e)
    {
      std::cerr << e << std::endl;
      return EXIT_FAILURE;
    }

  global_log.message("Successfully started!", Log::NOTICE);
  delete conn;

  return EXIT_SUCCESS;
}
