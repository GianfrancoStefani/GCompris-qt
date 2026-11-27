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
    property string imagesPrefix: "qrc:/gcompris/data/words-webp/birds/"
    property var levels: [
        {
            "type": "lesson",
            "name": qsTr("Birds"),
            "image": imagesPrefix + "bird.png",
            "content": [
                {
                    "instructions": qsTr("Place the BIRDS to the right and other objects to the left"),
                    "image": imagesPrefix + "bird.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["birds/parrot.png","birds/bird34.png","birds/bird23.png","birds/peacock.png","birds/rooster.png","birds/bird1.png"],
                    "bad": ["animals/baboon.png","animals/bosmutus.png","animals/camel.png","animals/cow.png","others/sharpnerandpencil.png","transport/cycle.png"]
                },
                {
                    "instructions": qsTr("Place the BIRDS to the right and other objects to the left"),
                    "image": imagesPrefix + "bird.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["birds/bird2.png","birds/bird3.png","birds/bird4.png","birds/bird5.png","birds/bird6.png","birds/bird7.png"],
                    "bad": ["animals/dog.png","animals/dolphin.png","animals/donkey.png","animals/elephant.png","others/mobile.png","transport/helicopter1.png"]
                },
                {
                    "instructions": qsTr("Place the BIRDS to the right and other objects to the left"),
                    "image": imagesPrefix + "bird.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 7,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["birds/bird8.png","birds/bird9.png","birds/bird10.png","birds/bird11.png","birds/bird12.png"],
                    "bad": ["plants/tree1.png","transport/train2.png","animals/giraffe.png","vegetables/spinaches.png","fruits/kiwi.png","insects/insect12.png","food/cereal.png"]
                },
                {
                    "instructions": qsTr("Place the BIRDS to the right and other objects to the left"),
                    "image": imagesPrefix + "bird.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["birds/bird13.png","birds/bird14.png","birds/bird15.png","birds/bird16.png","birds/bird17.png"],
                    "bad": ["animals/hare.png","animals/hedgehog.png","food/cheese.png","food/pizza.png"]
                },
                {
                    "instructions": qsTr("Place the BIRDS to the right and other objects to the left"),
                    "image": imagesPrefix + "bird.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["birds/bird18.png","birds/bird19.png","birds/bird20.png","birds/bird21.png","birds/bird22.png"],
                    "bad": ["animals/horse.png","animals/koala.png","food/milk.png","insects/insect5.png"]
                },
                {
                    "instructions": qsTr("Place the BIRDS to the right and other objects to the left"),
                    "image": imagesPrefix + "bird.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["birds/bird23.png","birds/bird24.png","birds/bird25.png","birds/bird26.png"],
                    "bad": ["animals/marsupialis.png","animals/mouse.png","others/pepsi.png","insects/insect16.png","food/milk.png"]
                },
                {
                    "instructions": qsTr("Place the BIRDS to the right and other objects to the left"),
                    "image": imagesPrefix + "bird.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["birds/bird27.png","birds/bird28.png","birds/bird29.png"],
                    "bad": ["animals/kodiak-bear.png","animals/krotiki.png","others/clock.png"]
                },
                {
                    "instructions": qsTr("Place the BIRDS to the right and other objects to the left"),
                    "image": imagesPrefix + "bird.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["birds/bird30.png","birds/bird31.png","birds/bird32.png"],
                    "bad": ["animals/mouse1.png","animals/pig.png","nature/nature5.png"]
                },
                {
                    "instructions": qsTr("Place the BIRDS to the right and other objects to the left"),
                    "image": imagesPrefix + "bird.png",
                    "maxNumberOfGood": 2,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["birds/bird33.png","birds/bird34.png"],
                    "bad": ["animals/lion.png","plants/tree3.png","transport/balloon.png","householdGoods/refrigerator.png"]
                }
            ]
        }
    ]
}
