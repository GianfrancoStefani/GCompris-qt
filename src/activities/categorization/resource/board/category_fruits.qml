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
    property string imagesPrefix: "qrc:/gcompris/src/activities/lang/resource/words_sample/"
    property var levels: [
        {
            "type": "lesson",
            "name": qsTr("Fruits"),
            "image": "qrc:/gcompris/data/words-webp/fruits/fruitsalad.png",
            "content": [
                {
                    "instructions": qsTr("Place the FRUITS to the right and other objects to the left"),
                    "image": imagesPrefix + "fruit.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["fruits/apple.png","fruits/apricot.png","fruits/banana.png","fruits/berries.png","fruits/billberries.png","fruits/cherries.png"],
                    "bad": ["vegetables/batatadoce.png","vegetables/bittergourd.png","vegetables/blackchillies.png","vegetables/aubergine.png","others/house.png","others/street.png"],
                },
                {
                    "instructions": qsTr("Place the FRUITS to the right and other objects to the left"),
                    "image": imagesPrefix + "fruit.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["fruits/cranberries.png","fruits/feiji.png","fruits/kiwi.png","fruits/lemon.png","fruits/litchi.png","fruits/mango.png"],
                    "bad": ["vegetables/garlic.png","vegetables/carrots.png","vegetables/cauliflower.png","vegetables/chillies.png","others/spoon.png","others/fork.png"]
                },
                {
                    "instructions": qsTr("Place the FRUITS to the right and other objects to the left"),
                    "image": imagesPrefix + "fruit.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 7,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["fruits/mirabellen.png","fruits/nectarine.png","fruits/papaya.png","fruits/peach.png","fruits/pineapple.png"],
                    "bad": ["vegetables/chineseradish.png","vegetables/cucumber.png","food/cereal.png","food/macroni.png","food/hamburger.png","transport/cycle.png","transport/car1.png"]
                },
                {
                    "instructions": qsTr("Place the FRUITS to the right and other objects to the left"),
                    "image": imagesPrefix + "fruit.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["fruits/plum.png","fruits/stackelberry.png","fruits/tyttberries.png","fruits/strwaberry.png"],
                    "bad": ["vegetables/dahuisi.png","others/mobile.png","vegetables/fid.png","food/eggs.png","food/icecream.png"]
                },
                {
                    "instructions": qsTr("Place the FRUITS to the right and other objects to the left"),
                    "image": imagesPrefix + "fruit.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["fruits/fruitsalad.png","fruits/guava.png","fruits/grapes.png","fruits/litchi.png"],
                    "bad": ["vegetables/pumpkin.png","transport/bus.png","food/milk.png","food/cheese.png","food/butter.png"]
                },
                {
                    "instructions": qsTr("Place the FRUITS to the right and other objects to the left"),
                    "image": imagesPrefix + "fruit.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["fruits/pear.png","fruits/pyrusmalus.png","fruits/pineapple.png"],
                    "bad": ["vegetables/radish.png","food/popcorn.png","others/street.png"]
                },
                {
                    "instructions": qsTr("Place the FRUITS to the right and other objects to the left"),
                    "image": imagesPrefix + "fruit.png",
                    "maxNumberOfGood": 3 ,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["fruits/peach.png","fruits/tyttberries.png","fruits/papaya.png"],
                    "bad": ["vegetables/spinach.png","food/hotdog.png", "food/milk.png"]
                },
                {
                    "instructions": qsTr("Place the FRUITS to the right and other objects to the left"),
                    "image": imagesPrefix + "fruit.png",
                    "maxNumberOfGood": 2,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["fruits/mango.png","fruits/strwaberry.png"],
                    "bad": ["others/pencil.png","vegetables/onion.png","food/cheese.png","food/water.png"]
                }
            ]
        }
    ]
}
