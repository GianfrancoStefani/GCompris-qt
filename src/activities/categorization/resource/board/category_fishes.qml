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
    property string imagesPrefix: "qrc:/gcompris/src/activities/categorization/resource/images/fishes/"
    property var levels: [
        {
            "type": "lesson",
            "name": qsTr("Fishes"),
            "image": imagesPrefix + "fish20.png",
            "content": [
                {
                    "instructions": qsTr("Place the FISHES to the right and other objects to the left"),
                    "image": imagesPrefix + "fish10.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["fishes/fish1.png","fishes/fish2.png","fishes/fish3.png","fishes/fish4.png","fishes/fish5.png","fishes/fish6.png"],
                    "bad": ["monuments/brandenburgGate.png","monuments/burj.png","others/bulb.png","tools/cuttingTool.png","others/knife.png","tools/multimeter2.png"]
                },
                {
                    "instructions": qsTr("Place the FISHES to the right and other objects to the left"),
                    "image": imagesPrefix + "fish13.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["fishes/fish7.png","fishes/fish8.png","fishes/fish9.png","fishes/fish10.png","fishes/fish11.png","fishes/fish12.png"],
                    "bad": ["renewable/dam2.png","renewable/solar5.png","others/pillow.png","tools/plier1.png","tools/nailCutter.png","monuments/colosseum.png"]
                },
                {
                    "instructions": qsTr("Place the FISHES to the right and other objects to the left"),
                    "image": imagesPrefix + "fish3.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["fishes/fish13.png","fishes/fish14.png","fishes/fish15.png","fishes/fish16.png","fishes/fish17.png"],
                    "bad": ["others/bulb.png","others/chair.png","renewable/windmill.png","monuments/monument2.png"]
                },
                {
                    "instructions": qsTr("Place the FISHES to the right and other objects to the left"),
                    "image": imagesPrefix + "fish16.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["fishes/fish18.png","fishes/fish19.png","fishes/fish20.png","fishes/fish21.png",
                            "fishes/fish22.png"],
                    "bad": ["others/plate.png","monuments/operaHouse.png","monuments/zimniPalace.png","renewable/geothermal.png"]
                },
                {
                    "instructions": qsTr("Place the FISHES to the right and other objects to the left"),
                    "image": imagesPrefix + "fish20.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["fishes/fish23.png","fishes/fish24.png", "fishes/fish25.png"],
                    "bad": ["monuments/monument3.png","others/pan.png","others/pencil.png"]
                },
                {
                    "instructions": qsTr("Place the FISHES to the right and other objects to the left"),
                    "image": imagesPrefix + "fish25.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["fishes/fish26.png","fishes/fish27.png","fishes/fish28.png"],
                    "bad": ["renewable/windmill13.png","renewable/geothermal.png","tools/scissors1.png"]
                }
            ]
        }
    ]
}
