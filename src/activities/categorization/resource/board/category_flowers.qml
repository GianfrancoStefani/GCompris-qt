/* GCompris
 *
 * SPDX-FileCopyrightText: 2018 Aman Kumar Gupta <gupta2140@gmail.com>
 *
 * Authors:
 *   Aman Kumar Gupta <gupta2140@gmail.com>
 *
 *   SPDX-License-Identifier: GPL-3.0-or-later
 */
import QtQuick

QtObject {
    property bool isEmbedded: false
    property bool allowExpertMode: true
    property string imagesPrefix: "qrc:/gcompris/data/words-webp/flowers/"
    property var levels: [
        {
            "name": qsTr("Flowers"),
            "image": imagesPrefix + "bunchOfFlowers.png",
            "content": [
                {
                    "instructions": qsTr("Place the FLOWERS to the right and other objects to the left"),
                    "image": imagesPrefix + "bunchOfFlowers.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["flowers/allamanda.png", "flowers/angiosperms.png", "flowers/angiosperms2.png", "flowers/aster.png", "flowers/aster2.png", "flowers/bauhiniaAcuminata.png"],
                    "bad": ["plants/plant2.png", "birds/bird1.png", "insects/insect10.png", "transport/cycle.png", "animals/camel.png", "householdGoods/iron2.png"]
                },
                {
                    "instructions": qsTr("Place the FLOWERS to the right and other objects to the left"),
                    "image": imagesPrefix + "bunchOfFlowers.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["flowers/bossier.png", "flowers/bunchOfFlowers.png", "flowers/calendulaSuffruticosa.png", "flowers/camellia.png", "flowers/capullo.png", "flowers/crocus.png"],
                    "bad": ["nature/nature5.png", "food/biryani.png", "food/milk.png", "fruits/apple.png", "others/house.png", "vegetables/carrots.png"]
                },
                {
                    "instructions": qsTr("Place the FLOWERS to the right and other objects to the left"),
                    "image": imagesPrefix + "bunchOfFlowers.png",
                    "maxNumberOfGood": 6,
                    "maxNumberOfBad": 6,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["flowers/dahlia.png", "flowers/daisy.png", "flowers/floora.png", "flowers/floreInsectos.png", "flowers/galanthusNivalis.png", "flowers/hibiscus.png"],
                    "bad": ["animals/cow.png", "transport/helicopter1.png", "food/cereal.png", "animals/elephant.png", "others/pepsi.png", "plants/tree3.png"]
                },
                {
                    "instructions": qsTr("Place the FLOWERS to the right and other objects to the left"),
                    "image": imagesPrefix + "bunchOfFlowers.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["flowers/lily.png", "flowers/marigold.png", "flowers/orchid.png", "flowers/pollen.png", "flowers/rose.png"],
                    "bad": ["lobster.png", "potato.png", "pumpkin.png", "radio.png"]
                },
                {
                    "instructions": qsTr("Place the FLOWERS to the right and other objects to the left"),
                    "image": imagesPrefix + "bunchOfFlowers.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 5,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["flowers/sunflower.png", "flowers/tulip.png", "flowers/victoriaAmazonica.png", "flowers/wildPetunia.png"],
                    "bad": ["food/hotdog.png", "vegetables/onion.png", "food/cheese.png", "food/water.png", "others/street.png"]
                },
                {
                    "instructions": qsTr("Place the FLOWERS to the right and other objects to the left"),
                    "image": imagesPrefix + "bunchOfFlowers.png",
                    "maxNumberOfGood": 5,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["flowers/lily.png", "flowers/marigold.png", "flowers/orchid.png", "flowers/pollen.png", "flowers/rose.png"],
                    "bad": ["food/eggs.png", "animals/lion.png", "birds/bird17.png", "householdGoods/iron.png"]
                },
                {
                    "instructions": qsTr("Place the FLOWERS to the right and other objects to the left"),
                    "image": imagesPrefix + "bunchOfFlowers.png",
                    "maxNumberOfGood": 3,
                    "maxNumberOfBad": 3,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["flowers/lotus.png", "flowers/jasmine.png", "flowers/rhododendron.png"],
                    "bad": ["food/grilledSandwich.png", "transport/metro.png", "food/frenchFries.png"]
                },
                {
                    "instructions": qsTr("Place the FLOWERS to the right and other objects to the left"),
                    "image": imagesPrefix + "bunchOfFlowers.png",
                    "maxNumberOfGood": 2,
                    "maxNumberOfBad": 4,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["flowers/ashoka.png", "flowers/acidanthera.png"],
                    "bad": ["others/fork.png", "fruits/apple.png", "food/skimmedMilk.png", "fruits/grapes.png"]
                },
                {
                    "instructions": qsTr("Place the FLOWERS to the right and other objects to the left"),
                    "image": imagesPrefix + "bunchOfFlowers.png",
                    "maxNumberOfGood": 4,
                    "maxNumberOfBad": 2,
                    "prefix": "qrc:/gcompris/data/words-webp/",
                    "good": ["flowers/allamanda.png", "flowers/camellia.png", "flowers/jasmine.png", "flowers/marigold.png"],
                    "bad": ["plants/plant6.png", "fruits/papaya.png"]
                }
            ]
        }
    ]
}
