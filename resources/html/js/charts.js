
var populationArray = []
var weightArray = []
var ageArray = []
var creatureTypeArray = []
var populationChart
var weightChart
var ageChart
var creatureTypeChart
var bucketsNumber = 13

function newDataPopulationNum(frame, num) {
    populationArray.push([frame, num])
    populationChart.updateSeries([{
        data: populationArray
    }])
}

function newDataWeightNum(frame, num) {
    weightArray.push([frame, num])
    weightChart.updateSeries([{
        data: weightArray
    }])
}

function newDataAgeNum(frame, num) {
    ageArray.push([frame, num])
    ageChart.updateSeries([{
        data: ageArray
    }])
}

function newDataCreatureType(type, num) {
    let idx = creatureTypeArray.findIndex(elem => elem.x == type)
    if (idx != -1) {
        creatureTypeArray[idx].y = num
        creatureTypeChart.updateSeries([{
            data: creatureTypeArray
        }])
    }
    console.log(creatureTypeArray)
}


function renderPopulationLineChart() {
    var options = {
        chart: {
            type: 'line'
        },
        series: [{
            name: 'Number of Creatures',
            data: populationArray
        }],
        xaxis: {
            type: 'numeric'
        },
        title: {
            text: 'Number of creatures in time',
            align: 'center'
        },
        theme: {
            mode: 'light',
            palette: 'palette7',
        }
    }

    populationChart = new ApexCharts(document.querySelector("#populationchart"), options);
    populationChart.render();
}

function renderWeightLineChart() {
    var options = {
        chart: {
            type: 'line'
        },
        series: [{
            name: 'Weight of Craeture',
            data: weightArray
        }],
        xaxis: {
            type: 'numeric'
        },
        title: {
            text: 'Average weight of Creatures in time',
            align: 'center'
        },
        theme: {
            mode: 'light',
            palette: 'palette7',
        }
    }

    weightChart = new ApexCharts(document.querySelector("#weightchart"), options);
    weightChart.render();
}

function renderAgeLineChart() {
    var options = {
        chart: {
            type: 'line'
        },
        series: [{
            name: 'Age of Creature',
            data: ageArray
        }],
        xaxis: {
            type: 'numeric'
        },
        title: {
            text: 'Average age of crearures by time',
            align: 'center'
        },
        theme: {
            mode: 'light',
            palette: 'palette7',
        }
    }

    ageChart = new ApexCharts(document.querySelector("#agechart"), options);
    ageChart.render();
}

function treeMapColorsChart() {
    var last = 0
    for (var i = 1; i < bucketsNumber; i++) {
        var next = 0 + 30 * i
        var name = last.toString(10) + '-' + next.toString(10)
        creatureTypeArray.push({ x: name, y: 0 });
        last = next
    }

    var options = {
        series: [
            {
                data: creatureTypeArray
            }
        ],
        chart: {
            height: 350,
            type: 'treemap'
        },
        title: {
            text: 'Types of Creatures',
            align: 'center'
        },
        colors: [
            '#fc4103',
            '#fcbe03',
            '#befc03',
            '#41fc03',
            '#03fc41',
            '#03fcbe',
            '#03befc',
            '#0341fc',
            '#4103fc',
            '#be03fc',
            '#fc03be',
            '#fc0341'
        ],
        plotOptions: {
            treemap: {
                distributed: true,
                enableShades: false
            }
        }
    };

    creatureTypeChart = new ApexCharts(document.querySelector("#treemapchart"), options);
    creatureTypeChart.render();
}

function getDataAboutNetwork(graph) {
    var h1Element = document.getElementById("status");
    h1Element.innerHTML = ''
    if (graph != undefined) {
        renderNeuralNetowork(graph)
    }
}

function renderNeuralNetowork(graph) {
    var width = 400,
        height = 1200,
        nodeSize = 30;

    var color = d3.scale.category20();

    var svg = d3.select("#svg").append("svg")
        .attr("width", width)
        .attr("height", height);

    var color = d3.scale.category20();

    console.log(graph)
    graph = JSON.parse(graph)
    var nodes = graph.nodes;

    // get network size
    var netsize = {};
    nodes.forEach(function (d) {
        if (d.layer in netsize) {
            netsize[d.layer] += 1;
        } else {
            netsize[d.layer] = 1;
        }
        d["lidx"] = netsize[d.layer];
    });

    // calc distances between nodes
    var largestLayerSize = Math.max.apply(
        null, Object.keys(netsize).map(function (i) { return netsize[i]; }));

    var xdist = width / Object.keys(netsize).length,
        ydist = height / largestLayerSize;

    // create node locations
    nodes.map(function (d) {
        d["x"] = (d.layer - 0.5) * xdist;
        d["y"] = (d.lidx - 0.5) * ydist;
    });

    // autogenerate links
    var links = [];
    nodes.map(function (d, i) {
        for (var n in nodes) {
            if (d.layer + 1 == nodes[n].layer) {
                links.push({ "source": parseInt(i), "target": parseInt(n), "value": 1 })
            }
        }
    }).filter(function (d) { return typeof d !== "undefined"; });

    // draw links
    var link = svg.selectAll(".link")
        .data(links)
        .enter().append("line")
        .attr("class", "link")
        .attr("x1", function (d) { return nodes[d.source].x; })
        .attr("y1", function (d) { return nodes[d.source].y; })
        .attr("x2", function (d) { return nodes[d.target].x; })
        .attr("y2", function (d) { return nodes[d.target].y; })
        .style("stroke-width", function (d) { return Math.sqrt(d.value); });

    // draw nodes
    var node = svg.selectAll(".node")
        .data(nodes)
        .enter().append("g")
        .attr("transform", function (d) {
            return "translate(" + d.x + "," + d.y + ")";
        }
        );
    var lastLayer = Object.keys(netsize)[Object.keys(netsize).length - 1]
    var circle = node.append("circle")
        .attr("class", "node")
        .attr("r", nodeSize)
        .style("fill", function (d) {
            if (parseFloat(d.label) >= 0.8 && d.layer != 1) {
                return '#00ff00'
            }
            if (parseFloat(d.label) <= -0.8 && d.layer != 1) {
                return '#ff3300'
            }
            return color(d.layer);
        });


    node.append("text")
        .attr("dx", "-.35em")
        .attr("dy", ".35em")
        .text(function (d) { return d.label; });

}

function updateDataAboutNetwork(graph) {
    if (graph != undefined) {
        var svg = d3.select("#svg");
        svg.selectAll("*").remove();
        renderNeuralNetowork(graph)
    }
}

function updateCellValues(tableName, data) {
    var table = document.getElementById(tableName);
    for (var r = 0, n = table.rows.length; r < n; r++) {
        table.rows[r].cells[1].innerHTML = data[r];
    }
}

function updateDataAboutCreature(data) {
    var h1Element = document.getElementById("status");
    h1Element.innerHTML = ''
    data = JSON.parse(data)
    updateCellValues("CreatureTable", data.data)
}

function creatureDead() {
    var h1Element = document.getElementById("status");
    h1Element.innerHTML = 'CREATURE IS DEAD. SELECT ANOTHER'
}