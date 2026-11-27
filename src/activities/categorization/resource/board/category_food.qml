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
    property string imagesPrefix: "qrc:/gcompris/data/words-webp/food/"
    property var levels: [
        {
            "type": "lesson",
            "name": qsTr("Food"),
            "image": imagesPrefix + "pizza.png",
            "content": [
                {
                    "instructions": qsTr("Place the FOOD ITEMS to the right and other objects to the left"),
                    "image": imagesPrefix + "sweetBread.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["food/applepie.png","food/bananaNutBread.png","food/biryani.png","food/cereal.png","food/cheese.png","food/eggs.png"],
                    "bad": ["nature/nature28.png","plants/plant2.png","nature/nature27.png","birds/bird1.png","nature/nature17.png","insects/insect10.png"]
                },
                {
                    "instructions": qsTr("Place the FOOD ITEMS to the right and other objects to the left"),
                    "image": imagesPrefix + "pizza1.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["food/fish.png","food/frenchFries.png","food/friedEggs.png","food/grilledSandwich.png","food/hamburger.png","food/hotdog.png"],
                    "bad": ["nature/nature26.png","birds/bird25.png","transport/cycle.png","insects/insect1.png","nature/nature24.png","others/fork.png"]
                },
                {
                    "instructions": qsTr("Place the FOOD ITEMS to the right and other objects to the left"),
                    "image": imagesPrefix + "hotdog.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["food/icecream.png","food/cereal1.png","food/cheese1.png","food/kathiRoll.png","food/macroni.png"],
                    "bad": ["animals/camel.png","animals/hare.png","insects/insect4.png","birds/bird5.png"]
                },
                {
                    "instructions": qsTr("Place the FOOD ITEMS to the right and other objects to the left"),
                    "image": imagesPrefix + "frenchFries.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["food/MaozVegetariano.png","food/milk.png","food/bananaNutBread.png","food/pitaBread.png","food/pizza.png"],
                    "bad": ["birds/bird6.png","others/street.png","householdGoods/iron2.png","insects/insect8.png"]
                },
                {
                    "instructions": qsTr("Place the FOOD ITEMS to the right and other objects to the left"),
                    "image": imagesPrefix + "hamburger.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["food/icecream1.png","food/riceBeans.png","food/scrambledEggsVeggies.png"],
                    "bad": ["nature/nature21.png","plants/tree2.png","householdGoods/bed.png"]
                },
                {
                    "instructions": qsTr("Place the FOOD ITEMS to the right and other objects to the left"),
                    "image": imagesPrefix + "milk.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["food/skimmedMilk.png","food/steak.png","food/sweetBread.png"],
                    "bad": ["birds/bird4.png","animals/koala.png","transport/ferry.png"]
                }
            ]
        }
    ]
}
