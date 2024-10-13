import QtQuick 2.15
import QtQuick.Controls 2.15
import QtPositioning 5.15
import QtLocation 5.15
import Qt5Compat.GraphicalEffects
import com.monapplication 1.0
import "mapitems"


ApplicationWindow {
    visible: true
    width: 1920
    height: 1080
    title: "V2V"

    ListModel {
        id: vehicleModel
    }
    ListModel{
        id: displayModel
    }

    Component.onCompleted: {
        socketClient.newDataReceived.connect(addOrUpdateVehicle);
    }


    property var connectedVehicles: [] // Tableau pour stocker les coordonnées des véhicules connectés
    property var mainVehicle: {"lat": 0, "lon": 0} // Utilisation de guillemets pour les clés


    function addOrUpdateVehicle(vehId, latStr, lonStr, redStr, greenStr, blueStr, ptStr) {
        if(vehId == "0"){
            for (var i = 0; i < cellulesModel.length; i++) {
                cellulesModel[i].fillColor = "#00000000"
            }
        }else if(vehId == "-1"){
            updateHexagonColors();
        }else if(vehId == "-2"){
            for (var i = 0; i < cellulesModel.length; i++) {
                cellulesModel[i].fillColor = "#00000000"
            }
            vehicleModel.clear()
        }else if(vehId == "-3"){
            console.log("Received notice", latStr)
            var indicesCorrespondants = []; // Initialisation d'une liste vide

            for (var i = 0; i < cellulesModel.length; i++) {
                for (var j = 0; j < cellulesModel[i].getNombreDeVehicules(); j++) {
                    if (cellulesModel[i].vehicules[j].id == latStr) {
                        indicesCorrespondants.push(i); // Ajouter l'indice i à la liste
                        mainVehicle = {"lat": cellulesModel[i].vehicules[j].lat(), "lon": cellulesModel[i].vehicules[j].lon()}; // Mise à jour des coordonnées
                        console.log(mainVehicle.lat, mainVehicle.lon)




                    }
                }
            }

            var vehiculesTrouves = []; // Liste pour stocker les ID des véhicules trouvés

            for (var i = 0; i < indicesCorrespondants.length; i++) {
                var celluleIndex = indicesCorrespondants[i];
                var cellule = cellulesModel[celluleIndex];

                for (var j = 0; j < cellule.getNombreDeVehicules(); j++) {
                    var vehicule = cellule.vehicules[j];
                    if (vehicule.id !== latStr) {
                        var vehiculeExisteDeja = false;

                        // Vérifier si le véhicule existe déjà dans le modèle
                        for (var k = 0; k < displayModel.count; ++k) {
                            var vehicleExistant = displayModel.get(k);
                            if (vehicleExistant.id === vehicule.id) {
                                vehiculeExisteDeja = true;
                                displayModel.set(k,{"id": vehicule.id, "lat": vehicule.lat(), "lon": vehicule.lon()});
                                break; // Sortir de la boucle si le véhicule existe déjà
                            }
                        }

                        // Ajouter le véhicule s'il n'existe pas déjà
                        if (!vehiculeExisteDeja) {
                            displayModel.append({"id": vehicule.id, "lat": vehicule.lat(), "lon": vehicule.lon()});
                        }

                        // Ajouter l'ID du véhicule à la liste des véhicules trouvés
                        vehiculesTrouves.push(vehicule.id);
                    }
                }
            }

            // Supprimer les véhicules de displayModel qui n'ont pas été trouvés
            for (var i = displayModel.count - 1; i >= 0; i--) {
                var vehicleId = displayModel.get(i).id;
                if (!vehiculesTrouves.includes(vehicleId)) {
                    displayModel.remove(i);
                }
            }


                //cellulesModel[i].clearVehicules()

                /*for(var j = 0; j < cellulesModel[i].vehicule().size; j++){
                    print(j)
                }*/

        }else if(vehId == "-4"){
            displayModel.clear()
        }else{
            var lat = parseFloat(latStr);
            var lon = parseFloat(lonStr);
            var red = parseFloat(redStr);
            var green = parseFloat(greenStr);
            var blue = parseFloat(blueStr);
            var pt = parseInt(ptStr);

            var screenPosition = map.fromCoordinate(QtPositioning.coordinate(lat, lon));
            var screenX = screenPosition.x;
            var screenY = screenPosition.y;

                // Chercher si le vehicule existe
                for (var i = 0; i < vehicleModel.count; ++i) {
                    var vehicle = vehicleModel.get(i)
                    if (vehicle && vehicleModel.get(i).id === vehId) {

                        if(lat ===0){
                            vehicleModel.remove(i)
                            for(var j = 0; j < displayModel.count; ++j){
                                var vehicle2 = displayModel.get(j)
                                if(vehicle2 && vehicle2.id === vehId){
                                    displayModel.remove(j)
                                }
                            }
                        }else{
                            vehicleModel.set(i, {"id": vehId, "lat": lat, "lon": lon, "red": red, "green": green, "blue": blue, "pt": pt, "radius": vehicleModel.get(i).radius, "x": screenX, "y": screenY});
                        }
                        //console.log("Mise à jour du véhicule:", vehId, "lat:", lat, "lon:", lon, "red:", red, "green", green, "blue:", blue, "pt", pt, "radius", vehicleModel.get(i).radius, "x", screenX, "y", screenY);
                        return;
                    }
                }

                if(lat !== 0){
                    // Si le véhicule n'existe pas, l'ajouter
                    var radius = Math.ceil(20*(Math.sqrt(pt*100000/ (4* Math.PI))/892));
                    //console.log("Mise à jour du véhicule:", vehId, "lat:", lat, "lon:", lon, "red:", red, "green", green, "blue:", blue, "pt", pt);
                    vehicleModel.append({"id": vehId, "lat": lat, "lon": lon,"red": red, "green": green, "blue": blue, "pt": pt, "radius": radius, "x": screenX, "y": screenY});
                }
            }
        }

    function updateHexagonColors() {
        for (var i = 0; i < cellulesModel.length; i++) {
            var cell = cellulesModel[i];
            cell.clearVehicules()
            var dominantVehicle = findDominantVehicleInHexagon(cell, vehicleModel);
            if (dominantVehicle) {
                updateFillColor(i,dominantVehicle.red,dominantVehicle.green, dominantVehicle.blue,0.5);
                //console.log(cellulesModel[i].vehicules[0].id)
            }else{
                cellulesModel[i].fillColor = "#00000000"
            }
        }
    }


    function findDominantVehicleInHexagon(hexagon, vehicleModel) {
        var dominantVehicle = null;
        var highestAlpha = -1; // Assurez-vous que ceci est inférieur à votre plus basse valeur alpha
        for (var j = 0; j < vehicleModel.count; j++) {
            var vehicle = vehicleModel.get(j);

            if (isVehicleInHexagon(hexagon.centerX,hexagon.centerY,vehicle.x,vehicle.y)) {
                var newVehicle = Qt.createQmlObject('import com.monapplication 1.0; Vehicule { id: "' + vehicle.id + '" }', hexagon);
                newVehicle.setLat(vehicle.lat)
                newVehicle.setLon(vehicle.lon)
                hexagon.addVehicule(newVehicle)
                if (vehicle.pt > highestAlpha) {
                    highestAlpha = vehicle.pt;
                    dominantVehicle = vehicle;
                }
            }
        }

        return dominantVehicle;
    }

    function updateFillColor(index, red, green, blue, alpha) {
        // Assurez-vous que l'index est dans la plage valide
        if (index < 0 || index >= cellulesModel.count) {
            console.error("Index hors de portée.");
            return;
        }

        // Convertir les valeurs RGB et Alpha de 0-1 à 0-255, puis en chaîne hexadécimale
        var redHex = Math.round(red * 255).toString(16).padStart(2, '0');
        var greenHex = Math.round(green * 255).toString(16).padStart(2, '0');
        var blueHex = Math.round(blue * 255).toString(16).padStart(2, '0');
        var alphaHex = Math.round(alpha * 255).toString(16).padStart(2, '0');

        // Construire la chaîne de couleur RGBA
        var color = "#" +alphaHex + redHex + greenHex + blueHex;
        //console.log(color);  // Log pour le débogage

        cellulesModel[index].fillColor =color;

        // Mettre à jour la couleur de l'élément dans le modèle
    }

    function isVehicleInHexagon(hexagonX, hexagonY, vehicleX, vehicleY) {
        // Calcule les bornes du carré autour de l'hexagone
           var minX = hexagonX - 75 / 2;
           var maxX = hexagonX + 75 / 2;
           var minY = hexagonY - 75 / 2;
           var maxY = hexagonY + 75 / 2;

           // Vérifie si les coordonnées de la voiture sont à l'intérieur de ces bornes
           return vehicleX >= minX && vehicleX <= maxX && vehicleY >= minY && vehicleY <= maxY;
    }

    Map {
        id: map
        width: 1920
        height: 1080
        plugin: Plugin {
            name: "osm"
        }
        center: QtPositioning.coordinate(47.746723, 7.338994)
        zoomLevel: 15

        Repeater {
            model: cellulesModel.length

            Hexagone{
                Component.onCompleted: {
                    console.log("Index:", index, " -> ", cellulesModel[index] ? cellulesModel[index].toString() : "undefined");
                }

                x: cellulesModel[index].x
                y: cellulesModel[index].y

                fillColor: cellulesModel[index] ? cellulesModel[index].fillColor : "transparent"
                strokeColor: cellulesModel[index] ? cellulesModel[index].strokeColor : "black"
            }
        }




        /*MapPolyline {
                line.width: 20
                line.color: "blue"

                path: [
                    QtPositioning.coordinate(47.75396728515625, 7.353229522705078), // Coordonnées du véhicule principal
                    QtPositioning.coordinate(47.741127014160156, 7.338579177856445) // Coordonnées du véhicule connecté
                ]
            }*/
        MapItemView {
            model: vehicleModel
            delegate: MapQuickItem {
                coordinate: QtPositioning.coordinate(model.lat, model.lon)
                sourceItem: Item {
                    width: model.radius  // Définissez la largeur totale de l'élément
                    height: model.radius // Définissez la hauteur totale de l'élément
                    //Le Rectangle représentant la voiture
                    Rectangle {
                        width: 3
                        height: 3
                        color: "red"
                        anchors.centerIn: parent  // Centrez le rectangle dans l'élément parent
                    }
                    // L'Image représentant la couverture
                    Image {
                        id: vehicleImage
                        source: "coverage_circle.png"  // Chemin vers l'image de la couverture
                        width: parent.width   // Utilisez la largeur de l'élément parent
                        height: parent.height // Utilisez la hauteur de l'élément parent
                        anchors.centerIn: parent  // Centrez l'image dans l'élément parent
                    }
                    ColorOverlay {
                        anchors.fill: vehicleImage
                        source: vehicleImage
                        color: Qt.rgba(model.red, model.green, model.blue)
                    }
                }
                anchorPoint.x: sourceItem.width / 2
                anchorPoint.y: sourceItem.height / 2
            }
        }

        MapItemView {
                model: displayModel // connectedVehicles est une liste des véhicules connectés

                delegate: MapPolyline {
                    line.width: 2
                    line.color: "black"

                    path: [
                        QtPositioning.coordinate(mainVehicle.lat, mainVehicle.lon), // Coordonnées du véhicule principal
                        QtPositioning.coordinate(model.lat, model.lon) // Coordonnées du véhicule connecté
                    ]
                }
            }
    }
}


