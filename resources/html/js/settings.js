
function setPrimaryValues() {
    document.getElementById("widthInput").value = "800";
    document.getElementById("heightInput").value = "800";
    document.getElementById("creaturesInput").value = "100";
    document.getElementById("widthOutput").value = "800";
    document.getElementById("heightOutput").value = "800";
    document.getElementById("creaturesOutput").value = "100";
}

//FUNCTIONS

function sendData() {
    var x = document.getElementById("widthInput").value;
    var y = document.getElementById("heightInput").value;
    var creatures = document.getElementById("creaturesInput").value;
    var energyThreshhold = document.getElementById("energyThreshhold").value;
    var minWeight = document.getElementById("minWeight").value;
    var weightGained = document.getElementById("weightGained").value;
    var weightLost = document.getElementById("weightLost").value;
    var birthWeightThreshhold = document.getElementById("birthWeightThreshhold").value;
    var energyBirth = document.getElementById("energyBirth").value;
    var energyBirthFailed = document.getElementById("energyBirthFailed").value;
    var weightBirth = document.getElementById("weightBirth").value;
    var birthAgeThreshhold = document.getElementById("birthAgeThreshhold").value;
    var anglePerFrame = document.getElementById("anglePerFrame").value;
    var accelerationMultiplier = document.getElementById("accelerationMultiplier").value;
    var maxSpeed = document.getElementById("maxSpeed").value;

    var element = document.getElementById("butt1");
    element.parentNode.removeChild(element)
    setMapSize(x, y);
    setSimulationParameters(creatures, energyThreshhold, minWeight,
        weightGained, weightLost,
        birthWeightThreshhold, energyBirth,
        energyBirthFailed, weightBirth,
        birthAgeThreshhold, anglePerFrame,
        accelerationMultiplier, maxSpeed)
}

function putNewCreature() {
    var typeV = document.getElementById("creatureType").value;
    var energyV = document.getElementById("creatureEnergy").value;
    var weightV = document.getElementById("creatureWeight").value;
    var hueV = document.getElementById("creatureColor").value;
    var positionXV = document.getElementById("creaturePosX").value;
    var positionYV = document.getElementById("creaturePosY").value;
    var speedMultiplierV = document.getElementById("creatureSpeedMulti").value;
    var creaturesNum = document.getElementById("creaturesNum").value;
    var object = {
        type: typeV, energy: parseFloat(energyV), weight: parseFloat(weightV), hue: parseFloat(hueV),
        positionX: parseFloat(positionXV), positionY: parseFloat(positionYV), speedMultiplier: parseFloat(speedMultiplierV)
    };

    putCreature(object, creaturesNum)
}

function NotSubmitted() {
    var h1Element = document.getElementById("status");
    h1Element.innerHTML = 'YOU DID NOT SUBMITTED THE DATA ABOUT MAP YET. Please go back to settings tab.'
}

function Submitted() {
    var h1Element = document.getElementById("status");
    h1Element.innerHTML = 'Create new custom Creature!'
}

function GetListOfCreatures() {
    var list = getListOfCreatures()
    addCreaturesToDropdown(list)
}

function addCreaturesToDropdown(listOfCreatures) {
    var dropdown = document.getElementById("creatureTypes");
    listOfCreatures.forEach(function (element) {
        console.log(element)
        var option = document.createElement("OPTION");
        option.innerHTML = element;
        option.value = element;
        dropdown.options.add(option);
    });
}

function getCreature(item) {
    var dropdown = document.getElementById("creatureTypes");
    var creatureElem = dropdown.options[item].text;
    getDataAboutCreature(creatureElem)
}

function receiveData(data) {
    var creatureData = JSON.parse(data)
    document.getElementById("creatureType").value = creatureData.type;
    document.getElementById("creatureEnergy").value = creatureData.energy;
    document.getElementById("creatureWeight").value = creatureData.weight;
    document.getElementById("creatureColor").value = creatureData.hue;
    document.getElementById("creaturePosX").value = creatureData.positionX;
    document.getElementById("creaturePosY").value = creatureData.positionY;
    document.getElementById("creatureSpeedMulti").value = creatureData.speedMultiplier;
}