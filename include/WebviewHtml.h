/////////////////////////////////////////////////////////
///                                                   ///
///     Author: Bartlomiej Janowski                   ///
///     This header file holds the data URI           ///
///     that is forwarded to WebView window           ///
///     on program launch                             ///
///                                                   ///
/////////////////////////////////////////////////////////
#pragma once

#include <string>

namespace HtmlFiles {

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
                function testSend() {
                    writeToBody("Kliknieto przycisk");
                    var x = document.getElementById("widthInput").value;
                    var y = document.getElementById("heightInput").value;
                    var element = document.getElementById("butt1");
                    element.parentNode.removeChild(element)
                    setMapSize(x, y);
                }
                function frameNum(num) {
                    document.getElementById("frameCounter").innerHTML = "Frame: ".concat(num);
                }
                function dataAvailable() {
                    writeToBody("Data available");
                }
            </script>
        </head>

        <body onload="">
            This is a placeholder. Some data may appear shortly.
            <label for="widthInput">Perlin width:</label>
            <input type="range" id="widthInput" name="widthInput" min="0" max="1000"
                oninput="this.nextElementSibling.value = this.value">
            <output>500</output>
            </br>
            <label for="heightInput">Perlin height:</label>
            <input type="range" id="heightInput" name="heightInput" min="0" max="1000"
                oninput="this.nextElementSibling.value = this.value">
            <output>500</output>
            </br>
            <button onclick="testSend();" id="butt1">Hejka</button>
            <div id="frameCounter">FramCounter</div>
        </body>

        </html>
    )";

    const std::string WEBVIEW_JS_INIT = 
    R"()";

}