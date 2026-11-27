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

QtObject{
    property bool isEmbedded: false
    property bool allowExpertMode: true
    property string imagesPrefix: "qrc:/gcompris/data/words-webp/insects/"
    property var levels: [
        {
            "type": "lesson",
            "name": qsTr("Insects"),
            "image": imagesPrefix + "insect6.png",
            "content": [
                {
                    "instructions": qsTr("Place the INSECTS to the right and other objects to the left"),
                    "image": imagesPrefix + "insect15.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["insects/insect1.png","insects/insect2.png","insects/insect3.png","insects/insect4.png","insects/insect5.png","insects/insect6.png"],
                    "bad": ["nature/nature4.png","plants/tree2.png","birds/bird8.png","birds/bird5.png","animals/giraffe.png","animals/horse.png"]
                },
                {
                    "instructions": qsTr("Place the INSECTS to the right and other objects to the left"),
                    "image": imagesPrefix + "insect25.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["insects/insect7.png","insects/insect8.png","insects/insect9.png","insects/insect10.png","insects/insect11.png","insects/insect12.png"],
                    "bad": ["food/eggs.png","birds/bird2.png","birds/bird3.png","animals/lion.png","animals/mouse.png","animals/elephant.png"]
                },
                {
                    "instructions": qsTr("Place the INSECTS to the right and other objects to the left"),
                    "image": imagesPrefix + "insect6.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["insects/insect13.png","insects/insect14.png","insects/insect15.png","insects/insect16.png","insects/insect17.png"],
                    "bad": ["transport/helicopter.png","householdGoods/oven.png","birds/bird8.png","birds/bird9.png"]
                },
                {
                    "instructions": qsTr("Place the INSECTS to the right and other objects to the left"),
                    "image": imagesPrefix + "insect8.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["insects/insect18.png","insects/insect19.png","insects/insect20.png","insects/insect21.png","insects/insect22.png"],
                    "bad": ["householdGoods/bed.png","others/mobile.png","birds/bird17.png","animals/hare.png"]
                },
                {
                    "instructions": qsTr("Place the INSECTS to the right and other objects to the left"),
                    "image": imagesPrefix + "insect19.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["insects/insect23.png","insects/insect24.png","insects/insect25.png"],
                    "bad": ["plants/tree3.png","birds/bird29.png","animals/pig.png"]
                },
                {
                    "instructions": qsTr("Place the INSECTS to the right and other objects to the left"),
                    "image": imagesPrefix + "insect12.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["insects/insect26.png","insects/insect27.png","insects/insect12.png"],
                    "bad": ["householdGoods/iron.png","birds/bird33.png","animals/sheep.png"]
                }
            ]
        }
    ]
}
