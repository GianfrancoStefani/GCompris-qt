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
    property string imagesPrefix: "qrc:/gcompris/data/words-webp/nature/"
    property var levels: [
        {
            "type": "lesson",
            "name": qsTr("Nature"),
            "image": imagesPrefix + "nature7.png",
            "content": [
                {
                    "instructions": qsTr("Place the NATURE images to the right and other objects to the left"),
                    "image": imagesPrefix + "nature25.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["nature/nature1.png","nature/nature2.png","nature/nature3.png","nature/nature4.png","nature/nature5.png","nature/nature6.png"],
                    "bad": ["transport/train3.png","others/spoon.png","transport/bus.png","others/clock.png","food/hamburger.png","food/macroni.png"]
                },
                {
                    "instructions": qsTr("Place the NATURE images to the right and other objects to the left"),
                    "image": imagesPrefix + "nature28.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["nature/nature7.png","nature/nature8.png","nature/nature9.png","nature/nature10.png", "nature/nature11.png","nature/nature12.png"],
                    "bad": ["fruits/mango.png","vegetables/onion.png","transport/cycle.png","others/fork.png","food/riceBeans.png","others/pepsi.png"]
                },
                {
                    "instructions": qsTr("Place the NATURE images to the right and other objects to the left"),
                    "image": imagesPrefix + "nature22.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["nature/nature13.png","nature/nature14.png","nature/nature15.png","nature/nature16.png","nature/nature17.png"],
                    "bad": ["transport/autorickshaw.png","transport/bulletTrain.png","householdGoods/chest.png","others/broom1.png"]
                },
                {
                    "instructions": qsTr("Place the NATURE images to the right and other objects to the left"),
                    "image": imagesPrefix + "nature2.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["nature/nature18.png","nature/nature19.png","nature/nature20.png","nature/nature21.png","nature/nature22.png"],
                    "bad": ["transport/train5.png","transport/tram1.png","others/mobile.png","others/electricfan.png"]
                },
                {
                    "instructions": qsTr("Place the NATURE images to the right and other objects to the left"),
                    "image": imagesPrefix + "nature2.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["nature/nature29.png","nature/nature30.png","nature/nature31.png","nature/nature32.png","nature/nature33.png"],
                    "bad": ["fruits/pineapple.png","food/pitaBread.png","householdGoods/bed.png","others/house.png"]
                },
                {
                    "instructions": qsTr("Place the NATURE images to the right and other objects to the left"),
                    "image": imagesPrefix + "nature6.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["nature/nature23.png","nature/nature24.png","nature/nature25.png"],
                    "bad": ["vegetables/zucchini.png","others/street.png","householdGoods/couch.png"]
                },
                {
                    "instructions": qsTr("Place the NATURE images to the right and other objects to the left"),
                    "image": imagesPrefix + "nature14.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["nature/nature26.png","nature/nature27.png","nature/nature28.png"],
                    "bad": ["food/MaozVegetariano.png","transport/rickshaw2.png","others/electricfan.png"]
                },
                {
                    "instructions": qsTr("Place the NATURE images to the right and other objects to the left"),
                    "image": imagesPrefix + "nature14.png",
                    "maxNumberOfGood": 2,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["nature/nature34.png","nature/nature35.png"],
                    "bad": ["householdGoods/chair2.png","food/milk.png","others/weighingmachine.png","others/pencil.png"]
                }
            ]
        }
    ]
}
