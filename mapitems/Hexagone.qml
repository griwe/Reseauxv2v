import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Shapes 1.15

Item {
    id: hexagonItem
    width: 75  // Largeur de l'hexagone
    height: 75 // Hauteur de l'hexagone


    property color fillColor: "transparent"
    property color strokeColor: "black"

    Shape {
        id: shapeId
        anchors.fill: parent // Faire en sorte que la forme remplisse l'Item
        property color fillColor: "transparent"
        property color strokeColor: "black"

        ShapePath {
            fillColor: hexagonItem.fillColor
            strokeColor: hexagonItem.strokeColor
            strokeWidth: 1

            // Définition des points de l'hexagone
            startX: 37.5 // taille/2
            startY: 0

            PathLine { x: 37.5; y: 0 } // x = taille/2 & y = 0
            PathLine { x: 75; y: 18.75 } // x = taille & y = taille/4
            PathLine { x: 75; y: 56.25 } // x = taille & y = taille * 0.75
            PathLine { x: 37.5; y: 75 } // x = taille/2 & y = taille
            PathLine { x: 0; y: 56.25 } // x = 0 & y = taille * 0.75
            PathLine { x: 0; y: 18.75 } // x = 0 & y = taille/4
            PathLine { x: 37.5; y: 0 } // x = taille/2 & y = 0
        }
    }

    function containsPoint(x, y) {
        // Convertit le point dans le système de coordonnées de l'Item
        var localPoint = mapToItem(null, x, y);
        // Vérifie si le point est à l'intérieur des limites de l'Item
        return contains(localPoint.x, localPoint.y);
    }
}
