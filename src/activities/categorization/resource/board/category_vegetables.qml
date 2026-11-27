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
    property string imagesPrefix: "qrc:/gcompris/data/words-webp/vegetables/"
    property var levels: [
        {
            "type": "lesson",
            "name": qsTr("Vegetables"),
            "image": imagesPrefix + "vegetablesmix.png",
            "content": [
                {
                    "instructions": qsTr("Place the VEGETABLES to the right and other objects to the left"),
                    "image": imagesPrefix + "vegetablesmix.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["vegetables/batatadoce.png","vegetables/bittergourd.png","vegetables/blackchillies.png", "vegetables/aubergine.png","vegetables/peas.png","vegetables/carrots.png"],
                    "bad": ["fruits/cranberries.png","fruits/feiji.png","fruits/strwaberry.png","fruits/pear.png","food/applepie.png","food/biryani.png","transport/bus.png","transport/truck.png"]
                },
                {
                    "instructions": qsTr("Place the VEGETABLES to the right and other objects to the left"),
                    "image": imagesPrefix + "vegetablesmix.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["vegetables/cauliflower.png","vegetables/chillies.png","vegetables/chineseradish.png","vegetables/cucumber.png","vegetables/dahuisi.png","vegetables/garlic.png"],
                    "bad": ["fruits/apple.png","fruits/apricot.png","fruits/banana.png","fruits/berries.png","transport/boat.png","plants/plant2.png","nature/nature6.png","householdGoods/quilt.png"]
                },
                {
                    "instructions": qsTr("Place the VEGETABLES to the right and other objects to the left"),
                    "image": imagesPrefix + "vegetablesmix.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["vegetables/potato.png","vegetables/radish.png","vegetables/redchillies.png","vegetables/spinach.png"],
                    "bad": ["fruits/billberries.png","fruits/cherries.png","food/cheese.png","birds/bird7.png","insects/insect2.png"]
                },
                {
                    "instructions": qsTr("Place the VEGETABLES to the right and other objects to the left"),
                    "image": imagesPrefix + "vegetablesmix.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["vegetables/spinaches.png","vegetables/taroroot.png","vegetables/cauliflower.png","vegetables/batatadoce.png"],
                    "bad": ["fruits/kiwi.png","householdGoods/vacuumCleaner.png","householdGoods/utensils.png","food/pizza.png","transport/tanker.png"]
                },
                {
                    "instructions": qsTr("Place the VEGETABLES to the right and other objects to the left"),
                    "image": imagesPrefix + "vegetablesmix.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["vegetables/mushroom.png","vegetables/pumpkin.png","vegetables/sweetpotato.png"],
                    "bad": ["fruits/mango.png","plants/plant6.png","others/electricfan.png"]
                },
                {
                    "instructions": qsTr("Place the VEGETABLES to the right and other objects to the left"),
                    "image": imagesPrefix + "vegetablesmix.png",
                    "maxNumberOfGood": 2,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["vegetables/zucchini.png","vegetables/vegetablesmix.png"],
                    "bad": ["fruits/papaya.png","food/sweetBread.png","others/clock.png","others/pepsi.png"]
                }
            ]
        }
    ]
}
