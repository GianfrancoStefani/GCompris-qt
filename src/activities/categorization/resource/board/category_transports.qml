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
    property string imagesPrefix: "qrc:/gcompris/data/words-webp/transport/"
    property var levels: [
        {
            "type": "lesson",
            "name": qsTr("Transport"),
            "image": imagesPrefix + "balloon.png",
            "content": [
                {
                    "instructions": qsTr("Place the MEANS OF TRANSPORTATION to the right and other objects to the left"),
                    "image":imagesPrefix + "tucker.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["transport/autorickshaw.png","transport/balloon.png","transport/bicycle2.png","transport/boat.png","transport/bulletTrain.png","transport/bullockcart.png"],
                    "bad": ["householdGoods/ac.png","householdGoods/heater.png","householdGoods/quilt.png","food/kathiRoll.png","food/icecream.png","vegetables/carrots.png"]
                },
                {
                    "instructions": qsTr("Place the MEANS OF TRANSPORTATION to the right and other objects to the left"),
                    "image":imagesPrefix + "train1.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["transport/bus.png","transport/camel.png","transport/car1.png","transport/exchanger.png","transport/ferry.png","transport/flight.png"],
                    "bad": ["householdGoods/coffeeMaker.png","householdGoods/curtains.png","nature/nature6.png","insects/insect10.png","householdGoods/lamp.png","others/fork.png"]
                },
                {
                    "instructions": qsTr("Place the MEANS OF TRANSPORTATION to the right and other objects to the left"),
                    "image":imagesPrefix + "ship.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["transport/helicopter.png","transport/metro.png","transport/plane.png","transport/train5.png","transport/rickshaw.png","transport/rocket.png"],
                    "bad": ["nature/nature5.png","nature/nature8.png","food/biryani.png","food/skimmedMilk.png","fruits/grapes.png","fruits/apple.png"]
                },
                {
                    "instructions": qsTr("Place the MEANS OF TRANSPORTATION to the right and other objects to the left"),
                    "image":imagesPrefix + "plane.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["transport/bus2.png","transport/car2.png","transport/ferry1.png","transport/ship.png","transport/rotorShip.png","transport/cycle.png"],
                    "bad": ["vegetables/pumpkin.png","householdGoods/oven.png","householdGoods/radio1.png","food/riceBeans.png","fruits/guava.png","plants/plant2.png","nature/nature14.png"]
                },
                {
                    "tags": ["transport"],
                    "instructions": qsTr("Place the MEANS OF TRANSPORTATION to the right and other objects to the left"),
                    "image":imagesPrefix + "train2.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["transport/tram.png","transport/tucker.png","transport/bus1.png","transport/car1.png", "transport/ferry1.png"],
                    "bad": ["insects/insect15.png","birds/bird18.png","animals/giraffe.png","others/street.png"]
                },
                {
                    "instructions": qsTr("Place the MEANS OF TRANSPORTATION to the right and other objects to the left"),
                    "image":imagesPrefix + "roadTrain.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["transport/cycle.png","transport/helicopter1.png","transport/plane1.png","transport/train2.png","animals/horse.png"],
                    "bad": ["food/hamburger.png","householdGoods/electricBlanket.png","householdGoods/bed.png","vegetables/spinach.png"]
                },
                {
                    "instructions": qsTr("Place the MEANS OF TRANSPORTATION to the right and other objects to the left"),
                    "image":imagesPrefix + "car2.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["transport/plane2.png","transport/roadTrain.png","transport/rocket1.png","transport/train4.png"],
                    "bad": ["food/pizza.png","householdGoods/toaster.png","animals/koala.png","birds/bird21.png","insects/insect12.png"]
                },
                {
                    "instructions": qsTr("Place the MEANS OF TRANSPORTATION to the right and other objects to the left"),
                    "image":imagesPrefix + "ferry.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["transport/car3.png","transport/train3.png","transport/ship1.png"],
                    "bad": ["householdGoods/almirah.png","plants/tree3.png","nature/nature17.png"]
                },
                {
                    "instructions": qsTr("Place the MEANS OF TRANSPORTATION to the right and other objects to the left"),
                    "image":imagesPrefix + "rocket1.png",
                    "maxNumberOfGood": 2,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["transport/car5.png","transport/tanker.png"],
                    "bad": ["food/milk.png","householdGoods/vacuumCleaner.png","insects/insect9.png","birds/bird23.png"]
                },
                {
                    "instructions": qsTr("Place the MEANS OF TRANSPORTATION to the right and other objects to the left"),
                    "image":imagesPrefix + "cycle.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["transport/plane3.png","transport/rickshaw2.png","transport/train1.png"],
                    "bad": ["insects/insect18.png","vegetables/cauliflower.png","nature/nature24.png"]
                }
            ]
        }
    ]
}
