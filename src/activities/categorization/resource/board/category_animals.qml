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
    property string imagesPrefix: "qrc:/gcompris/data/words-webp/animals/"
    property var levels:[
        {
            "type": "lesson",
            "name": qsTr("Animals"),
            "image": imagesPrefix + "animalsherd.png",
            "content": [
                {
                    "instructions": qsTr("Place the ANIMALS to the right and other objects to the left"),
                    "image": imagesPrefix + "tiger.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["animals/baboon.png","animals/bosmutus.png","animals/camel.png","animals/cow.png","animals/dog.png","birds/bird10.png"],
                    "bad": ["nature/nature5.png","householdGoods/chair.png","food/biryani.png","others/electricfan.png","others/clock.png","nature/nature10.png"]
                },
                {
                    "instructions": qsTr("Place the ANIMALS to the right and other objects to the left"),
                    "image": imagesPrefix + "tiger.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["animals/lion.png","birds/bird7.png","birds/bird8.png","animals/mouse1.png","animals/opossum.png","animals/pig.png"],
                    "bad": ["nature/nature3.png","food/milk.png","food/butter.png","fruits/apple.png","transport/rickshaw.png","transport/bus.png"]
                },
                {
                    "instructions": qsTr("Place the ANIMALS to the right and other objects to the left"),
                    "image": imagesPrefix + "opossum.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["animals/snowcat.png","animals/spidermonkey.png","animals/squirrel.png","animals/squirrel1.png","animals/tasmaniandevil.png","birds/bird22.png"],
                    "bad": ["transport/helicopter.png","nature/nature2.png","others/house.png","transport/plane2.png","plants/plant4.png","nature/nature5.png"]
                },
                {
                    "instructions": qsTr("Place the ANIMALS to the right and other objects to the left"),
                    "image": imagesPrefix + "tortoise.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["animals/redeyedfrog.png","animals/scorpian.png","animals/sealion.png","animals/sheep.png","animals/snake.png"],
                    "bad": ["fruits/grapes.png","others/broom1.png","transport/car1.png","food/icecream.png"]
                },
                {
                    "instructions": qsTr("Place the ANIMALS to the right and other objects to the left"),
                    "image": imagesPrefix + "koala.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["animals/donkey.png","animals/elephant.png","insects/insect20.png","animals/tiger.png","animals/zebra.png"],
                    "bad": ["vegetables/pumpkin.png","vegetables/carrots.png","transport/cycle.png","plants/tree3.png"]
                },
                {
                    "instructions": qsTr("Place the ANIMALS to the right and other objects to the left"),
                    "image": imagesPrefix + "hedgehog.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["animals/giraffe.png","birds/bird25.png","animals/hedgehog.png","insects/insect7.png"],
                    "bad": ["vegetables/potato.png","transport/balloon.png","food/cheese.png","others/mobile.png","transport/bus1.png"]
                },
                {
                    "instructions": qsTr("Place the ANIMALS to the right and other objects to the left"),
                    "image": imagesPrefix + "lion.png",
                    "maxNumberOfGood": 3 ,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["animals/koala.png","animals/kodiak-bear.png","birds/bird27.png"],
                    "bad": ["nature/nature6.png","transport/ferry.png","food/eggs.png","food/hamburger.png"]
                },
                {
                    "instructions": qsTr("Place the ANIMALS to the right and other objects to the left"),
                    "image": imagesPrefix + "pig.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["animals/tiger.png","animals/tortoise.png","animals/tortoise1.png"],
                    "bad": ["transport/car1.png","fruits/papaya.png","food/hotdog.png"]
                },
                {
                    "instructions": qsTr("Place the ANIMALS to the right and other objects to the left"),
                    "image": imagesPrefix + "snowcat.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["animals/porcupine.png","animals/cow.png","birds/bird29.png"],
                    "bad": ["nature/nature8.png","food/macroni.png","others/house.png"]
                }
            ]
        }
    ]
}
