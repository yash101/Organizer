#include "httpserver_int.h"
#include <iostream>
#include <sstream>
void generate_home_page(dev::HttpServerSession& session)
{
  std::stringstream responseText;
//I am lazy, so this preprocessor def will make thing much easier for me.
#define o responseText << std::endl <<
  o "<!DOCTYPE html>";
  o "<html>";
    o "<head>";
      o "<title>DevLib HTTP Server Test Page!</title>";
      o "<style type=\"text/css\">";
        o "table";
        o "{";
          o "width: 100%;";
          o "background-color: #C6C6C6;";
          o "color: black;";
          o "border: 1px solid #C6C6C6;";
        o "}";

        o "table thead";
        o "{";
          o "border: 1px solid red;";
          o "background-color: black;";
          o "color: white;";
        o "}";

        o "table tr td";
        o "{";
          o "border: 1px solid black;";
          o "height: 100%;";
        o "}";

        o "table tr th:nth-child(1)";
        o "{";
          o "width: 250px;";
        o "}";

        o "input";
        o "{";
          o "width: 100%;";
          o "box-sizing: border-box;";
        o "}";
      o "</style>";

      o "<script type=\"text/javascript\">";
        o "var update_get_form = function()";
        o "{";
          o "var key = document.getElementById(\"GET_KEY\").value;";
          o "document.getElementById(\"GET_VALUE\").name = key;";
        o "};";

        o "var update_post_form = function()";
        o "{";
          o "var key = document.getElementById(\"POST_KEY\").value;";
          o "document.getElementById(\"POST_VALUE\").name = key;";
        o "};";
      o "</script>";

    o "</head>";
    o "<body>";
      o "<table><tbody><tr><td><h1 style=\"text-align: center;\">DevLib Integrated Web Server Test Page</h1></td></tr></tbody></table>";
      o "<table>";
        o "<thead>";
          o "<tr>";
            o "<th>Name</th><th>Value</th>";
          o "</tr>";
        o "</thead>";
        o "<tbody>";

          o "<tr>";
            o "<td>Method</td>";
            o "<td>" << (session.method == dev::GET ? "GET" : "POST") << "</td>";
          o "</tr>";

          o "<tr>";
            o "<td>Path</td>";
            o "<td>" << session.path << "</td>";
          o "</tr>";

          o "<tr>";
            o "<td>Processed Path</td>";
            o "<td>" << session.processed_path << "</td>";
          o "</tr>";

          o "<tr>";
            o "<td>Protocol</td>";
            o "<td>" << (session.proto == dev::HTTP_1_0 ? "HTTP/1.0" : "HTTP/1.1") << "</td>";
          o "</tr>";

          o "<tr>";
            o "<td>Queries</td>";
            o "<td>";
              o "<table>";
                o "<thead><tr><th>Name</th><th>Value</th></tr></thead>";
                o "<tbody>";
                for(std::map<std::string, dev::HttpQuery>::const_iterator it = session.queries.begin(); it != session.queries.end(); ++it)
                {
                  o "<tr>";
                    o "<td>" << it->first << "</td>";
                    o "<td><b>[" << (it->second.type == dev::GET ? "GET" : "POST") << ":]</b>[" << it->second.value << "]</td>";
                  o "</tr>";
                }
                o "</tbody>";
              o "</table>";
            o "</td>";
          o "</tr>";

          o "<tr>";
            o "<td>GET Queries</td>";
            o "<td>";
              o "<form action=\"/\">";
                o "<table>";
                  o "<thead><tr><th>Name</th><th>Value</th></tr></thead>";
                  o "<tbody>";
                  for(std::map<std::string, dev::HttpQuery>::const_iterator it = session.get.begin(); it != session.get.end(); ++it)
                  {
                    o "<tr>";
                      o "<td>[" << it->first << "]</td>";
                      o "<td>[" << it->second.value << "]</td>";
                    o "</tr>";
                    o "<tr style=\"display: none;\">";
                      o "<td><input type=\"text\" name=\"" << it->first << "\" value=\"" << it->second.value << "\" \"></td>";
                    o "</tr>";
                  }
                    o "<tr>";
                      o "<td><input type=\"text\" value=\"\" onkeyup=\"javascript:update_get_form();\" placeholder=\"Key\" id=\"GET_KEY\"></td>";
                      o "<td><table><tr><td><input type=\"text\" value=\"\" placeholder=\"Value\" id=\"GET_VALUE\"></td><td><input type=\"submit\" value=\"Submit\"></td></tr></table></td>";
                    o "</tr>";
                  o "</tbody>";
                o "</table>";
              o "</form>";
            o "</td>";
          o "</tr>";

          o "<tr>";
            o "<td>POST Queries</td>";
            o "<td>";
              o "<form action=\"/\" method=\"POST\">";
                o "<table>";
                  o "<thead><tr><th>Name</th><th>Value</th></tr></thead>";
                  o "<tbody>";
                  for(std::map<std::string, dev::HttpQuery>::const_iterator it = session.post.begin(); it != session.post.end(); ++it)
                  {
                    o "<tr>";
                      o "<td>[" << it->first << "]</td>";
                      o "<td>[" << it->second.value << "]</td>";
                    o "</tr>";
                    o "<tr style=\"display: none;\">";
                      o "<tr><input type=\"text\" name=\"" << it->first << "\" value=\"" << it->second.value << "\"></td>";
                    o "</tr>";
                  }
                    o "<tr>";
                      o "<td><input type=\"text\" value=\"\" onkeyup=\"javascript:update_post_form();\" placeholder=\"Key\" id=\"POST_KEY\"></td>";
                      o "<td><table><tr><td><input type=\"text\" value=\"\" placeholder=\"Value\" id=\"POST_VALUE\"></td><td><input type=\"submit\" value=\"Submit\"></td></tr></table></td>";
                    o "</tr>";
                  o "</tbody>";
                o "</table>";
              o "</form>";
            o "</td>";
          o "</tr>";

          o "<tr>";
            o "<td>Cookies</td>";
            o "<td>";
              o "<div><h3><b style=\"color: red; text-align: center;\">Note: Cookies are bugged at the moment and may not work properly.</b></h3></div>";
              o "<form action=\"/cookie_add\" method=\"GET\">";
                o "<table>";
                  o "<thead><tr><th>Name</th><th>Value</th></tr></thead>";
                  o "<tbody>";
                    for(std::map<std::string, dev::HttpCookie>::const_iterator it = session.cookies.begin(); it != session.cookies.end(); ++it)
                    {
                    o "<tr>";
                      o "<td>[" << it->first << "]</td>";
                      o "<td><b>[" << it->second.path << ":]</b>[" << it->second.value << "]</td>";
                    o "</tr>";
                    }
                    o "<tr>";
                      o "<td><input type=\"text\" name=\"name\" value=\"\" placeholder=\"Name\"></td>";
                      o "<td><table><tr><td><input type=\"text\" name=\"value\" value=\"\" placeholder=\"Value\"></td><td><input type=\"submit\" value=\"Add\"></td></tr></table></td>";
                    o "</tr>";
                  o "</tbody>";
                o "</table>";
              o "</form>";
            o "</td>";
          o "</tr>";

          o "<tr>";
            o "<td>Incoming Headers</td>";
            o "<td>";
              o "<table>";
                o "<thead><tr><th>Field</th><th>Value</th></tr></thead>";
                o "<tbody>";
                for(std::map<std::string, std::string>::const_iterator it = session.incoming_headers.begin(); it != session.incoming_headers.end(); ++it)
                {
                  o "<tr>";
                    o "<td>[" << it->first << "]</td>";
                    o "<td><b>[" << it->second << "]</td>";
                  o "</tr>";
                }
                o "</tbody>";
              o "</table>";
            o "</td>";
          o "</tr>";

        o "</tbody>";
      o "</table>";

      o "<table><tbody><tr><td><p style=\"text-align: center;\">Powered by DevLib integrated web server!</p></td></tr></tbody></table>";

    o "</body>";
  o "</html>";
#undef o

  session.response = responseText.str();
}
