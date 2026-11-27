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
    property string imagesPrefix: "qrc:/gcompris/src/activities/categorization/resource/images/renewable/"
    property var levels: [
        {
            "type": "lesson",
            "name": qsTr("Renewable"),
            "image": imagesPrefix + "windmill12.png",
            "content": [
                {
                    "instructions": qsTr("Place the RENEWABLE energy sources to the right and other objects to the left"),
                    "image": imagesPrefix + "windmill12.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["renewable/windmill.png","renewable/windmill1.png","renewable/windmill2.png","renewable/windmill3.png","renewable/windmill4.png","renewable/windmill5.png"],
                    "bad": ["fishes/fish20.png","others/volleyball.png","monuments/burj.png","tools/plier1.png","others/pillow.png","fishes/fish10.png"]
                },
                {
                    "instructions": qsTr("Place the RENEWABLE energy sources to the right and other objects to the left"),
                    "image": imagesPrefix + "solar8.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["renewable/solar1.png","renewable/solar2.png","renewable/solar3.png","renewable/solar4.png","renewable/solar5.png","renewable/solar6.png"],
                    "bad": ["others/plate.png","fishes/fish25.png","tools/scissors1.png","monuments/christTheRedeemer.png","others/bucket.png","monuments/jucheTower.png"]
                },
                {
                    "instructions": qsTr("Place the RENEWABLE energy sources to the right and other objects to the left"),
                    "image": imagesPrefix + "dam2.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["renewable/dam1.png","renewable/dam2.png","renewable/dam3.png","renewable/dam4.png"],
                    "bad": ["fishes/fish14.png","tools/wrench.png","tools/plier3.png","others/baseball.png","monuments/arcDeTriomphe.png"]
                },
                {
                    "instructions": qsTr("Place the RENEWABLE energy sources to the right and other objects to the left"),
                    "image": imagesPrefix + "dam5.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["renewable/windmill2.png","renewable/windmill13.png","renewable/solar7.png","renewable/solar3.png"],
                    "bad": ["monuments/greatPyramid.png","others/buffetset.png","others/weighingMachine.png","tools/spray.png","monuments/arcDeTriomphe.png"]
                },
                {
                    "instructions": qsTr("Place the RENEWABLE energy sources to the right and other objects to the left"),
                    "image": imagesPrefix + "geothermal.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["renewable/dam5.png","renewable/geothermal.png","renewable/windmill6.png","renewable/windmill7.png","renewable/windmill8.png"],
                    "bad": ["fishes/fish6.png","fishes/fish3.png","others/pan.png","others/spoons.png"]
                },
                {
                    "instructions": qsTr("Place the RENEWABLE energy sources to the right and other objects to the left"),
                    "image": imagesPrefix + "dam3.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["renewable/windmill9.png","renewable/windmill12.png","renewable/solar7.png"],
                    "bad": ["others/blackslate.png","monuments/IndiaGate.png","tools/multimeter2.png"]
                },
                {
                    "instructions": qsTr("Place the RENEWABLE energy sources to the right and other objects to the left"),
                    "image": imagesPrefix + "solar2.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["renewable/solar8.png","renewable/dam5.png","renewable/windmill11.png"],
                    "bad": ["others/plate.png","others/chair.png","monuments/eiffelTower.png"]
                },
                {
                    "instructions": qsTr("Place the RENEWABLE energy sources to the right and other objects to the left"),
                    "image": imagesPrefix + "solar4.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["renewable/windmill14.png","renewable/windmill15.png","renewable/dam5.png"],
                    "bad": ["others/plate.png","fishes/fish18.png","monuments/leMusee.png"]
                }
            ]
        }
    ]
}
