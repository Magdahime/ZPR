
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
    writeToBody("Kliknieto przycisk");
    var x = document.getElementById("widthInput").value;
    var y = document.getElementById("heightInput").value;
    var creatures = document.getElementById("creaturesInput").value;
    var energyThreshhold =  document.getElementById("energyThreshhold").value;
    var minWeight =  document.getElementById("minWeight").value;
    var birthWeightThreshhold =  document.getElementById("birthWeightThreshhold").value;
    var energyBirth =  document.getElementById("energyBirth").value;
    var energyBirthFailed =  document.getElementById("energyBirthFailed").value;
    var weightBirth =  document.getElementById("weightBirth").value;
    var birthAgeThreshhold =  document.getElementById("birthAgeThreshhold").value;
    var anglePerFrame =  document.getElementById("anglePerFrame").value;
    var accelerationMultiplier =  document.getElementById("accelerationMultiplier").value;
    var maxSpeed =  document.getElementById("maxSpeed").value;
    
    var element = document.getElementById("butt1");
    element.parentNode.removeChild(element)
    setMapSize(x, y);
    setSimulationParameters( creatures, energyThreshhold, minWeight,
                             birthWeightThreshhold, energyBirth, 
                             energyBirthFailed, weightBirth, 
                             birthAgeThreshhold,anglePerFrame,
                             accelerationMultiplier, maxSpeed)
}