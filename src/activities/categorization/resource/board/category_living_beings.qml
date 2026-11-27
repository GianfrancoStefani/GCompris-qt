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
    property string imagesPrefix: "qrc:/gcompris/data/words-webp/"
    property var levels: [
        {
            "type": "lesson",
            "name": qsTr("Living"),
            "image": imagesPrefix + "plants" + "/" + "plant1.png",
            "content": [
                {
                    "instructions": qsTr("Place the living beings to the right and other objects to the left"),
                    "image":imagesPrefix + "plants" + "/" + "plant5.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["plants/plant2.png","plants/tree1.png","animals/donkey.png","animals/horse.png","birds/bird12.png","birds/bird13.png"],
                    "bad": ["transport/balloon.png","transport/bus.png","others/house.png","others/clock.png","householdGoods/ac.png","others/pencil.png"]
                },
                {
                    "instructions": qsTr("Place the living beings to the right and other objects to the left"),
                    "image":imagesPrefix + "insects" + "/" + "insect4.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["insects/insect16.png","birds/bird17.png","insects/insect22.png","insects/insect19.png","insects/insect23.png","birds/bird21.png"],
                    "bad": ["transport/plane.png","transport/metro.png","householdGoods/refrigerator.png","householdGoods/chest1.png","food/grilledSandwich.png","others/mobile.png"]
                },
                {
                    "instructions": qsTr("Place the living beings to the right and other objects to the left"),
                    "image":imagesPrefix + "birds" + "/" + "bird5.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good":["plants/plant1.png","plants/tree2.png","insects/insect3.png","insects/insect4.png","birds/bird1.png"],
                    "bad": ["transport/rickshaw.png","householdGoods/bed.png","others/pepsi.png","householdGoods/canOpener.png"]
                },
                {
                    "instructions": qsTr("Place the living beings to the right and other objects to the left"),
                    "image":imagesPrefix + "plants" +"/" + "tree2.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["plants/plant3.png","plants/tree3.png","animals/snowcat.png","animals/pig.png"],
                    "bad": ["transport/plane.png","food/frenchFries.png","food/water.png","others/street.png","householdGoods/bed.png"]
                },
                {
                    "instructions": qsTr("Place the living beings to the right and other objects to the left"),
                    "image":imagesPrefix + "animals" + "/" + "opossum.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["plants/tree4.png","animals/giraffe.png","insects/insect7.png","birds/bird12.png","birds/bird15.png"],
                    "bad": ["householdGoods/heater.png","householdGoods/couch.png","transport/ship.png","others/fork.png","food/eggs.png"]
                },
                {
                    "instructions": qsTr("Place the living beings to the right and other objects to the left"),
                    "image":imagesPrefix + "animals" + "/" +  "hare.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["plants/plant4.png","animals/lion.png","birds/bird24.png"],
                    "bad": ["transport/helicopter.png","householdGoods/iron.png","others/mobile.png"]
                },
                {
                    "instructions": qsTr("Place the living beings to the right and other objects to the left"),
                    "image":imagesPrefix + "insects" + "/" +  "insect5.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["plants/plant5.png","plants/plant6.png","animals/sheep.png"],
                    "bad": ["transport/rocket.png","others/pepsi.png","food/hotdog.png"]
                }
            ]
        }
    ]
}
