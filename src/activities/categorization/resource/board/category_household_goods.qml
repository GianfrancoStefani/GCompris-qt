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
    property string imagesPrefix: "qrc:/gcompris/data/words-webp/householdGoods/"
    property var levels: [
        {
            "type": "lesson",
            "name": qsTr("Household goods"),
            "image": imagesPrefix + "utensils.png",
            "content": [
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                    "image":imagesPrefix + "oven.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["householdGoods/bedsheet.png","householdGoods/blender.png","householdGoods/breadtoaster.png", "householdGoods/canOpener.png","householdGoods/bathtub.png","householdGoods/bed.png"],
                    "bad": ["food/eggs.png","food/milk.png","food/riceBeans.png","transport/metro.png","transport/plane.png","fruits/apple.png"]
                },
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                     "image": imagesPrefix + "dressingtable.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": [ "householdGoods/almirah.png","householdGoods/coffeeMaker.png","householdGoods/laptop.png","householdGoods/bookshelf.png","householdGoods/chair.png","householdGoods/chest.png"],
                    "bad": ["transport/ship1.png","nature/nature5.png","fruits/papaya.png","food/hamburger.png","transport/ship.png","others/street.png"]
                },
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                    "image":imagesPrefix + "radio.png",
                    "maxNumberOfGood": 7,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["householdGoods/curtains.png","householdGoods/heater.png","householdGoods/stool.png","householdGoods/couch.png","householdGoods/crib.png","householdGoods/oven.png","householdGoods/almirah.png"],
                    "bad": ["others/house.png","transport/rocket1.png","transport/train4.png","insects/insect16.png","birds/bird14.png"]
                },
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                    "image":imagesPrefix + "sewingMachine.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["householdGoods/quilt.png","householdGoods/iron.png","householdGoods/bed2.png","householdGoods/couch2.png","householdGoods/diningtable.png","householdGoods/coffeeMaker1.png"],
                    "bad": ["transport/ferry1.png","vegetables/mushroom.png","insects/insect22.png","birds/bird5.png","food/friedEggs.png","food/MaozVegetariano.png"]
                },
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                     "image": imagesPrefix + "bed.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["householdGoods/almirah1.png","householdGoods/radio.png","householdGoods/chair2.png","householdGoods/stool2.png","householdGoods/electricBlanket.png","householdGoods/refrigerator1.png"],
                    "bad": ["birds/bird2.png","birds/bird5.png","food/macroni.png","transport/exchanger.png","vegetables/potato.png","others/pepsi.png"]
                },
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                    "image":imagesPrefix + "heater.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["householdGoods/television.png","householdGoods/toaster.png","householdGoods/curtains1.png","householdGoods/coffeeMaker2.png","householdGoods/iron1.png"],
                    "bad": ["nature/nature14.png","food/pizza1.png","fruits/mango.png","vegetables/spinach.png"]
                },
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                    "image":imagesPrefix + "ac.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["householdGoods/heater1.png","householdGoods/oven1.png","householdGoods/radio1.png","householdGoods/electricBlanket.png","householdGoods/bathtub.png"],
                    "bad": ["vegetables/zucchini.png","others/street.png","transport/helicopter.png","plants/tree2.png"]
                },
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                    "image":imagesPrefix + "breadtoaster.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["householdGoods/ac.png","householdGoods/iron2.png","householdGoods/toaster.png", "householdGoods/sewingMachine1.png", "householdGoods/vacuumCleaner.png"],
                    "bad": ["food/hotdog.png","animals/cow.png","birds/bird25.png","insects/insect14.png"]
                },
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                     "image": imagesPrefix + "bathtub.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["householdGoods/bed1.png","householdGoods/lamp.png","householdGoods/chair3.png",
                            "householdGoods/refrigerator2.png","householdGoods/towels.png"],
                    "bad": ["transport/bus.png","animals/elephant.png","insects/insect5.png","others/street.png"]
                },
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                     "image": imagesPrefix + "stool.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["householdGoods/chest1.png","householdGoods/diningtable1.png","householdGoods/laptop.png","householdGoods/sewingMachine.png"],
                    "bad": ["animals/kodiak-bear.png","animals/sealion.png","transport/plane.png","food/frenchFries.png","others/house.png"]
                },
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                     "image": imagesPrefix + "almirah.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["householdGoods/couch2.png","householdGoods/refrigerator.png","householdGoods/ac1.png"],
                    "bad": ["insects/insect18.png","transport/car1.png","animals/dog.png"]
                },
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                    "image":imagesPrefix + "coffeeMaker2.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["householdGoods/heater2.png","householdGoods/dressingtable.png","householdGoods/stool2.png"],
                    "bad": ["animals/lion.png","vegetables/aubergine.png","nature/nature12.png"]
                },
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                     "image": imagesPrefix + "chest.png",
                    "maxNumberOfGood": 2,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["householdGoods/chair5.png","householdGoods/lamp1.png"],
                    "bad": ["vegetables/fid.png","animals/koala.png","transport/ferry.png","others/broom1.png"]
                },
                {
                    "instructions": qsTr("Place the HOUSEHOLD GOODS to the right and other objects to the left"),
                    "image":imagesPrefix + "heater1.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["householdGoods/ac2.png","householdGoods/vacuumCleaner.png","householdGoods/sewingMachine2.png"],
                    "bad": ["insects/insect15.png","vegetables/potato.png","nature/nature13.png"]
                }
            ]
        }
    ]
}
