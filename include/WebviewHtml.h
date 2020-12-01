#pragma once

#include <string>

const std::string WEBVIEW_HTML_STR =
R"(data:text/html,
    <!doctype html>
    <html>

    <head>
        <script type="text/javascript">
            function writeToBody(content) {
                var myDiv = document.createElement("div");
                myDiv.innerHTML = "<p>".concat(content, "</p>");
                document.body.appendChild(myDiv);
            }
            function openSocket() {
                writeToBody("JS works!");
                var socket = new WebSocket("wss://zprssl.janowski.xyz:9002/");
                socket.onmessage = function (event) {
                    writeToBody(event.data);
                };
            }
        </script>
    </head>

    <body onload="openSocket();">
        This is a placeholder. Some data may appear shortly. <a href="https://localhost:9002/">Please trust the certificate first</a>
        Or go to <a href="https://google.com/">google</a>
    </body>

    </html>
)";