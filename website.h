#include <Ethernet.h>
void website(EthernetClient &client, uint8_t IPaddr[], uint8_t inputNumber[], const char* status, const char* save, uint8_t brightness[], const char* version){
     client.print("<!DOCTYPE html>");
     client.print("<!-- ");
     client.print("        Tally Light System ");
     client.print("    designed by Martin Tavoda");
     client.print("-->");
     client.print("<title>TLS configuration</title>");
     client.print("<html>");
     client.print("");
     client.print("<head>");
     client.print("<link rel=\"icon\" href=\"data:,\">");
     client.print("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
     client.print("<script src=\"https://code.jquery.com/jquery-3.6.0.min.js\"");
     client.print("integrity=\"sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4=\" crossorigin=\"anonymous\"></script>");
     client.print("");
     client.print("<style>");
     client.print("#loader {");
     client.print("position: absolute;");
     client.print("right: 35%;");
     client.print("border: 5px solid #ffffff;");
     client.print("border-radius: 50%;");
     client.print("border-top: 5px solid #666;");
     client.print("width: 25px;");
     client.print("height: 25px;");
     client.print("-webkit-animation: spin 1s linear infinite;");
     client.print("animation: spin 1s linear infinite;");
     client.print("}");
     client.print("");
     client.print("@-webkit-keyframes spin {");
     client.print("0% {");
     client.print("-webkit-transform: rotate(0deg);");
     client.print("}");
     client.print("");
     client.print("100% {");
     client.print("-webkit-transform: rotate(360deg);");
     client.print("}");
     client.print("}");
     client.print("");
     client.print("@keyframes spin {");
     client.print("0% {");
     client.print("transform: rotate(0deg);");
     client.print("}");
     client.print("");
     client.print("100% {");
     client.print("transform: rotate(360deg);");
     client.print("}");
     client.print("}");
     client.print("");
     client.print("input[type=text],");
     client.print("input[type=number],");
     client.print("select,");
     client.print("textarea {");
     client.print("width: 80%;");
     client.print("padding: 12px;");
     client.print("border: 1px solid #ccc;");
     client.print("border-radius: 4px;");
     client.print("box-sizing: border-box;");
     client.print("margin-top: 6px;");
     client.print("margin-bottom: 16px;");
     client.print("resize: vertical;");
     client.print("}");
     client.print("");
     client.print("label {");
     client.print("display: block;");
     client.print("}");
     client.print("");
     client.print("input[type=text]#IPaddr {");
     client.print("width: 100%;");
     client.print("}");
     client.print("");
     client.print("input[type=number].brightness {");
     client.print("width: 19%;");
     client.print("}");
     client.print("");
     client.print("input[type=submit],");
     client.print("button {");
     client.print("background-color: #4CAF50;");
     client.print("color: white;");
     client.print("padding: 12px 20px;");
     client.print("border: none;");
     client.print("border-radius: 4px;");
     client.print("cursor: pointer;");
     client.print("}");
     client.print("");
     client.print("button {");
     client.print("background-color: red;");
     client.print("margin-left: 3px;");
     client.print("}");
     client.print("");
     client.print("button:hover {");
     client.print("background-color: rgb(207, 0, 0);");
     client.print("}");
     client.print("");
     client.print(".button {");
     client.print("background-color: #4CAF50;");
     client.print("color: white;");
     client.print("padding: 12px 20px;");
     client.print("border: none;");
     client.print("border-radius: 4px;");
     client.print("cursor: pointer;");
     client.print("width: 100px;");
     client.print("margin: 0 4px;");
     client.print("}");
     client.print("");
     client.print("input[type=submit].button {");
     client.print("width: 120px;");
     client.print("font-family: Arial, Helvetica, sans-serif;");
     client.print("font-size: 14px;");
     client.print("}");
     client.print("");
     client.print("a.button.refresh {");
     client.print("background-color: #666;");
     client.print("text-decoration: none;");
     client.print("font-family: Arial, Helvetica, sans-serif;");
     client.print("font-size: 14px;");
     client.print("}");
     client.print("");
     client.print(".button:hover {");
     client.print("background-color: #45a049;");
     client.print("}");
     client.print("");
     client.print("a.button.refresh:hover {");
     client.print("background-color: rgb(134, 134, 134);");
     client.print("}");
     client.print("");
     client.print(".buttons {");
     client.print("clear: both;");
     client.print("padding: 20px 0 0 0;");
     client.print("text-align: center;");
     client.print("}");
     client.print("");
     client.print(".buttons>* {");
     client.print("display: inline-block;");
     client.print("}");
     client.print("");
     client.print(".container {");
     client.print("position: relative;");
     client.print("border-radius: 5px;");
     client.print("background-color: #f2f2f2;");
     client.print("padding: 20px;");
     client.print("margin-left: auto;");
     client.print("margin-right: auto;");
     client.print("width: 50%;");
     client.print("}");
     client.print("");
     client.print("#connected {");
     client.print("color: #4CAF50;");
     client.print("margin-top: 10px;");
     client.print("text-align: center;");
     client.print("}");
     client.print("");
     client.print("#disconnected {");
     client.print("color: red;");
     client.print("margin-top: 10px;");
     client.print("text-align: center;");
     client.print("}");
     client.print("");
     client.print("#footer {");
     client.print("position: fixed;");
     client.print("bottom: 10px;");
     client.print("right: 10px;");
     client.print("}");
     client.print("</style>");
     client.print("</head>");
     client.print("");
     client.print("<body>");
     client.print("<script>");
     client.print("$(document).ready(function () {");
     client.print("$(\"#loader\").hide();");
     client.print("if (document.URL.indexOf(\"IPaddr\") >= 0) {");
     client.print("$(\"#loader\").show();");
     client.print("setTimeout(function () {");
     client.print("window.location.replace(\"/\");");
     client.print("}, 1000);");
     client.print("}");
     client.print("});");
     client.print("//<p id=\"connected\">CONNECTED</p>");
     client.print("//<button name=\"save\" type=\"submit\">Save</button>");
     client.print("</script>");
     client.print("");
     client.print("<h3 style=\"text-align: center;\">Tally Light System configuration <br> designed for vMix</h3>");
     client.print("");
     client.print("<div class=\"container\">");
     client.print("<form id=\"form\" action=\"\">");
     client.print("<label for=\"IPaddr\">Video switcher IP address</label>");
     client.print("<input type=\"text\" id=\"IPaddr\" name=\"IPaddr\" placeholder=\"IP address\"");
     client.print("value=\"");
     client.print(IPaddr[0]);
     client.print(".");
     client.print(IPaddr[1]);
     client.print(".");
     client.print(IPaddr[2]);
     client.print(".");
     client.print(IPaddr[3]);
     client.print("\">");
     client.print("<div id=\"loader\"></div>");
     client.print("");
     client.print(status);
     client.print("");
     client.print("<p>Receiver number - vMix input</p>");
     client.print("<label for=\"T1\">1. Receiver</label>");
     client.print("<input type=\"number\" min=\"0\" max=\"255\" id=\"T1\" name=\"T1\" placeholder=\"vMix input number\"");
     client.print("value=\"");
     client.print(inputNumber[0]);
     client.print("\">");
     client.print("<input type=\"number\" min=\"1\" max=\"6\" step=\"1\" id=\"B1\" class=\"brightness\" name=\"B1\" value=\"");
     client.print(brightness[0]);
     client.print("\">");
     client.print("<label for=\"T2\">2. Receiver</label>");
     client.print("<input type=\"number\" min=\"0\" max=\"255\" id=\"T2\" name=\"T2\" placeholder=\"vMix input number\"");
     client.print("value=\"");
     client.print(inputNumber[1]);
     client.print("\">");
     client.print("<input type=\"number\" min=\"1\" max=\"6\" step=\"1\" id=\"B2\" class=\"brightness\" name=\"B2\" value=\"");
     client.print(brightness[1]);
     client.print("\">");
     client.print("<label for=\"T1\">3. Receiver</label>");
     client.print("<input type=\"number\" min=\"0\" max=\"255\" id=\"T3\" name=\"T3\" placeholder=\"vMix input number\"");
     client.print("value=\"");
     client.print(inputNumber[2]);
     client.print("\">");
     client.print("<input type=\"number\" min=\"1\" max=\"6\" step=\"1\" id=\"B3\" class=\"brightness\" name=\"B3\" value=\"");
     client.print(brightness[2]);
     client.print("\">");
     client.print("<label for=\"T2\">4. Receiver</label>");
     client.print("<input type=\"number\" min=\"0\" max=\"255\" id=\"T4\" name=\"T4\" placeholder=\"vMix input number\"");
     client.print("value=\"");
     client.print(inputNumber[3]);
     client.print("\">");
     client.print("<input type=\"number\" min=\"1\" max=\"6\" step=\"1\" id=\"B4\" class=\"brightness\" name=\"B4\" value=\"");
     client.print(brightness[3]);
     client.print("\">");
     client.print("<label for=\"T2\">5. Receiver</label>");
     client.print("<input type=\"number\" min=\"0\" max=\"255\" id=\"T5\" name=\"T5\" placeholder=\"vMix input number\"");
     client.print("value=\"");
     client.print(inputNumber[4]);
     client.print("\">");
     client.print("<input type=\"number\" min=\"1\" max=\"6\" step=\"1\" id=\"B5\" class=\"brightness\" name=\"B5\" value=\"");
     client.print(brightness[4]);
     client.print("\">");
     client.print("");
     client.print("<div class=\"buttons\">");
     client.print("<a class=\"button refresh\" href=\"/\">Refresh</a>");
     client.print("<input class=\"button\" type=\"Submit\" value=\"Submit\">");
     client.print("");
     client.print(save);
     client.print("");
     client.print("</div>");
     client.print("</form>");
     client.print("<div id=\"footer\">");
     client.print(version);
     client.print(" &copy eMT</div>");
     client.print("</div>");
     client.print("");
     client.print("</body>");
     client.print("");
     client.print("</html>");
}