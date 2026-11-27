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
    property string imagesPrefix: "qrc:/gcompris/src/activities/categorization/resource/images/tools/"
    property var levels: [
        {
            "type": "lesson",
            "name": qsTr("Tools"),
            "image": imagesPrefix + "measuringTape6.png",
            "content": [
                {
                    "instructions": qsTr("Place the TOOLS to the right and other objects to the left"),
                    "image": imagesPrefix + "wrench.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["tools/hammer8.png", "tools/hammer9.png","tools/hammer6.png","tools/hammer7.png","tools/hammer1.png","tools/hammer2.png"],
                    "bad": ["monuments/arcDeTriomphe.png","others/buffetset.png","monuments/cathedral.png","others/pillow.png","renewable/windmill1.png", "renewable/geothermal.png"]
                },
                {
                    "instructions": qsTr("Place the TOOLS to the right and other objects to the left"),
                    "image": imagesPrefix + "sideCutter1.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["tools/measuringTape5.png","tools/measuringTape1.png","tools/plier3.png","tools/plier1.png","tools/measuringTape3.png","tools/measuringTape4.png"],
                    "bad": ["others/pan.png","others/blackslate.png","monuments/colosseum.png","fishes/fish10.png","renewable/dam1.png","monuments/monument3.png"]
                },
                {
                    "instructions": qsTr("Place the TOOLS to the right and other objects to the left"),
                    "image": imagesPrefix + "scissors4.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["tools/scissors1.png","tools/scissors2.png","tools/scissors3.png","tools/spray.png","tools/tweezer.png","tools/wrench1.png"],
                    "bad": ["others/buffetset.png","monuments/brandenburgGate.png","monuments/parthenon.png","fishes/fish12.png","fishes/fish16.png","others/plate.png"]
                },
                {
                    "instructions": qsTr("Place the TOOLS to the right and other objects to the left"),
                    "image": imagesPrefix + "spray.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["tools/spray.png","tools/screwDriver.png","tools/screwDriver1.png","tools/screwDriver2.png","tools/screwDriver3.png","tools/plier1.png"],
                    "bad": ["others/baseball.png","others/igloo.png","monuments/mountRushmore.png","renewable/dam3.png","monuments/greatWall.png","renewable/windmill7.png"]
                },
                {
                    "instructions": qsTr("Place the TOOLS to the right and other objects to the left"),
                    "image": imagesPrefix + "plier1.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["tools/multimeter1.png","tools/screwDriver.png","tools/hammer3.png","tools/measuringTape6.png","tools/plier3.png"],
                    "bad": ["fishes/fish25.png","fishes/fish18.png","monuments/monument1.png","monuments/monument2.png"]
                },
                {
                    "instructions": qsTr("Place the TOOLS to the right and other objects to the left"),
                    "image": imagesPrefix + "plier1.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["tools/wrench.png","tools/tweezer1.png","tools/sickle.png","tools/plier2.png","tools/sideCutter1.png"],
                    "bad": ["renewable/solar5.png","others/bulb.png","renewable/dam3.png","monuments/operaHouse.png"]
                },
                {
                    "instructions": qsTr("Place the TOOLS to the right and other objects to the left"),
                    "image": imagesPrefix + "multimeter2.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["tools/plier3.png","tools/scissors1.png","tools/plier2.png","tools/sideCutter1.png"],
                    "bad": ["fishes/fish3.png","others/chair.png","monuments/operaHouse.png","renewable/windmill12.png","monuments/IndiaGate.png"]
                },
                {
                    "instructions": qsTr("Place the TOOLS to the right and other objects to the left"),
                    "image": imagesPrefix + "nailCutter.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["tools/measuringTape2.png","tools/hammer5.png","tools/plier1.png"],
                    "bad": ["fishes/fish6.png","monuments/tajMahal.png","renewable/solar3.png"]
                },
                {
                    "instructions": qsTr("Place the TOOLS to the right and other objects to the left"),
                    "image": imagesPrefix + "screwDriver.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["tools/plier3.png","tools/multimeter3.png","tools/nailCutter.png"],
                    "bad": ["fishes/fish10.png","others/pan.png","others/igloo.png"]
                },
                {
                    "instructions": qsTr("Place the TOOLS to the right and other objects to the left"),
                    "image": imagesPrefix + "scissors2.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/src/activities/categorization/resource/images/",
                    "good": ["tools/hammer4.png","tools/screwDriver.png","tools/multimeter2.png"],
                    "bad": ["renewable/dam2.png","monuments/leaningTowerOfPisa.png", "others/pillow.png"]
                }
            ]
        }
    ]
}
