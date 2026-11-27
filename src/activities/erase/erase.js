/* GCompris - erase.js
 *
 * SPDX-FileCopyrightText: 2014 Bruno Coudoin
 *
 * Authors:
 *   Bruno Coudoin <bruno.coudoin@gcompris.net> (GTK+ version)
 *   Bruno Coudoin <bruno.coudoin@gcompris.net> (Qt Quick port)
 *
 *   SPDX-License-Identifier: GPL-3.0-or-later
 */

.pragma library
.import core 1.0 as GCompris //for ApplicationInfo
.import "qrc:/gcompris/src/core/core.js" as Core
.import QtQuick as Quick

var url = "qrc:/gcompris/src/activities/erase/resource/"

//array of array, each with filename, horizontalAlignment and verticalAlignment
var backgroundImages = [
    ["alpaca.jpg", Quick.Image.AlignLeft, Quick.Image.AlignTop],
    ["bee.jpg", Quick.Image.AlignLeft, Quick.Image.AlignVCenter],
    ["butterfly.jpg", Quick.Image.AlignLeft, Quick.Image.AlignVCenter],
    ["calf.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignVCenter],
    ["camels.jpg", Quick.Image.AlignRight, Quick.Image.AlignVCenter],
    ["caterpillar.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignVCenter],
    ["chamaeleon.jpg", Quick.Image.AlignLeft, Quick.Image.AlignVCenter],
    ["cheetah.jpg", Quick.Image.AlignRight, Quick.Image.AlignTop],
    ["crab.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignVCenter],
    ["dolphin.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignTop],
    ["flying_fox.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignBottom],
    ["gibbon.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignVCenter],
    ["giraffe.jpg", Quick.Image.AlignRight, Quick.Image.AlignVCenter],
    ["goat.jpg", Quick.Image.AlignRight, Quick.Image.AlignTop],
    ["gorilla.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignVCenter],
    ["gosling.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignTop],
    ["heron.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignVCenter],
    ["horse.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignTop],
    ["kingfisher.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignTop],
    ["kitten.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignVCenter],
    ["long_nosed_monkey.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignTop],
    ["macaque.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignVCenter],
    ["meerkats.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignTop],
    ["northern_harrier.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignVCenter],
    ["nubian_ibex.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignTop],
    ["penguin.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignTop],
    ["pika.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignTop],
    ["red_panda.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignVCenter],
    ["rhinoceros.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignVCenter],
    ["spoonbills.jpg", Quick.Image.AlignLeft, Quick.Image.AlignTop],
    ["squirrel.jpg", Quick.Image.AlignHCenter, Quick.Image.AlignTop],
    ["swans.jpg", Quick.Image.AlignRight, Quick.Image.AlignTop],
    ["toucan.jpg", Quick.Image.AlignLeft, Quick.Image.AlignTop]
]

var blockImages = [
    url + "transparent_square.svg",
    url + "transparent_square_yellow.svg",
    url + "transparent_square_green.svg"
]

var currentImage
var items

// The counter of created blocks object
var createdBlocks
var killedBlocks

var blockImagesIndex

function start(items_) {
    items = items_
    items.currentLevel = Core.getInitialLevel(items.numberOfLevel)
    items_.currentSubLevel = 0
    currentImage = 0
    blockImagesIndex = 0
    backgroundImages = Core.shuffle(backgroundImages)
    initLevel()
}

function stop() {
}

function initLevel() {
    items.blocks.clear()
    blockImagesIndex++
    if(blockImagesIndex >= blockImages.length) {
        blockImagesIndex = 0
    }
    currentImage++
    if(currentImage >= backgroundImages.length) {
        currentImage = 0
    }
    items.activityBackground.source = url + backgroundImages[currentImage][0]
    items.activityBackground.alignBackground()
    createdBlocks = 0
    killedBlocks = 0
    var nbx = Math.min((items.currentLevel % 2 * 3) + 5, items.activityBackground.width / (10 * GCompris.ApplicationInfo.ratio));
    var nby = Math.min((items.currentLevel % 2 * 3) + 5, items.activityBackground.height / (10 * GCompris.ApplicationInfo.ratio));
    var data

    for(var x = 0;  x < nbx; ++x) {
        for(var y = 0;  y < nby; ++y) {
            data = {
                'nx': nbx, 'ny': nby, 'a': x, 'b': y,
                'img': blockImages[blockImagesIndex]
            }
            items.blocks.append(data)
        }
    }
    createdBlocks = nbx * nby;
}

function nextLevel() {
    items.score.stopWinAnimation();
    items.currentLevel = Core.getNextLevel(items.currentLevel, items.numberOfLevel);
    items.currentSubLevel = 0;
    initLevel();
}

function nextSubLevel() {
    if( items.currentSubLevel >= items.nbSubLevel) {
        items.bonus.good("flower")
        return;
    }
    initLevel();
}

function previousLevel() {
    items.score.stopWinAnimation();
    items.currentLevel = Core.getPreviousLevel(items.currentLevel, items.numberOfLevel);
    items.currentSubLevel = 0;
    initLevel();
}

function blockKilled() {
    if(++killedBlocks === createdBlocks) {
        items.currentSubLevel += 1
        items.winSound.play()
        items.score.playWinAnimation()
        if (items.telemetry) {
            var actName = items.activityName || "erase";
            items.telemetry.recordProblemCompleted(actName, items.currentLevel, true, {
                "sublevel": items.currentSubLevel,
                "total_sublevels": items.nbSubLevel,
                "blocks_cleared": killedBlocks
            });
        }
        if(items.mode === 2) {
            items.okButton.levelFinished = true
        }
    }
}
