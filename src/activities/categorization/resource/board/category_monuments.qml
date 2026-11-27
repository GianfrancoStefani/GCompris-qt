/* GCompris
 *
 * SPDX-FileCopyrightText: 2016 Divyam Madaan <divyam3897@gmail.com>
 *
 * Authors:
 *   Divyam Madaan <divyam3897@gmail.com>
 *
 *   SPDX-License-Identifier: GPL-3.0-or-later
 */
import QtQuick

QtObject {
    property bool isEmbedded: true
    property bool allowExpertMode: true
    property string imagesPrefix: "qrc:/gcompris/src/activities/categorization/resource/images/monuments/"
    property var levels: [
        {
            "type": "lesson",
            "name": qsTr("Monuments"),
            "image": imagesPrefix + "colosseum.png",
            "content": [
                {
                    "instructions": qsTr("Place the MONUMENTS to the right and other objects to the left"),
                    "image": imagesPrefix + "victoriaMemorial.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["monuments/bayterek.png","monuments/burj.png","monuments/cathedral.png","monuments/colosseum.png","monuments/brandenburgGate.png","monuments/arcDeTriomphe.png"],
                    "bad": ["renewable/windmill5.png","renewable/dam2.png","tools/tweezer.png","others/bulb.png","others/pan.png","others/knife.png"]
                },
                {
                    "instructions": qsTr("Place the MONUMENTS to the right and other objects to the left"),
                    "image": imagesPrefix + "parthenon.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["monuments/christTheRedeemer.png","monuments/eiffelTower.png","monuments/empireState.png" ,"monuments/greatPyramid.png","monuments/greatWall.png","monuments/IndiaGate.png"],
                    "bad": ["fishes/fish3.png","fishes/fish5.png","renewable/dam2.png","others/spoons.png","others/igloo.png","tools/measuringTape5.png"]
                },
                {
                    "instructions": qsTr("Place the MONUMENTS to the right and other objects to the left"),
                    "image": imagesPrefix + "monument2.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["monuments/jucheTower.png","monuments/kutubMinar.png","monuments/leaningTowerOfPisa.png" ,"monuments/operaHouse.png","monuments/monument1.png"],
                    "bad": ["fishes/fish7.png","fishes/fish16.png","tools/hammer4.png","tools/spray.png"]
                },
                {
                    "instructions": qsTr("Place the MONUMENTS to the right and other objects to the left"),
                    "image": imagesPrefix + "greatPyramid.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["monuments/mountRushmore.png","monuments/operaHouse.png","monuments/parthenon.png","monuments/statueOfLiberty.png","monuments/tajMahal.png"],
                    "bad": ["renewable/solar2.png","renewable/geothermal.png","tools/scissors2.png","tools/plier4.png"]
                },
                {
                    "instructions": qsTr("Place the MONUMENTS to the right and other objects to the left"),
                    "image": imagesPrefix + "brandenburgGate.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["monuments/usCapitol.png","monuments/victoriaMemorial.png","monuments/zimniPalace.png"],
                    "bad": ["tools/plier1.png","others/pillow.png","renewable/solar5.png"]
                },
                {
                    "instructions": qsTr("Place the MONUMENTS to the right and other objects to the left"),
                    "image": imagesPrefix + "IndiaGate.png",
                    "maxNumberOfGood": 2,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["monuments/monument2.png","monuments/monument3.png"],
                    "bad": ["others/volleyball.png","tools/sickle.png","others/chair.png","renewable/dam4.png"]
                }
            ]
        }
    ]
}
