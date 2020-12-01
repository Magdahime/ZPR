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
            function testSend() {
                writeToBody("Kliknieto przycisk");
                var x = document.getElementById("widthInput").value;
                var y = document.getElementById("heightInput").value;
                setMapSize(x, y);
            }
            function frameNum(num) {
                document.getElementById("frameCounter").innerHTML = "Frame: ".concat(num);
            }
        </script>
    </head>

    <body onload="">
        This is a placeholder. Some data may appear shortly.
          <label for="widthInput">Perlin width:</label>
            <input type="range" id="widthInput" name="widthInput" min="0" max="1000" oninput="this.nextElementSibling.value = this.value">
            <output>0</output>
            </br>
          <label for="heightInput">Perlin height:</label>
            <input type="range" id="heightInput" name="heightInput" min="0" max="1000" oninput="this.nextElementSibling.value = this.value">
            <output>0</output>
            </br>
        <button onclick="testSend();">Hejka</button>
        <div id="frameCounter">FramCounter</div>
    </body>

    </html>
)";