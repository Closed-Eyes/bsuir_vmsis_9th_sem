/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2011      Zynga Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
var TAG_TILE_MAP = 1;
var TAG_SPRITE_BATCH_NODE = 1;
var TAG_NODE = 2;
var TAG_ANIMATION1 = 1;
var TAG_SPRITE_LEFT = 2;
var TAG_SPRITE_RIGHT = 3;

var TAG_SPRITE1 = 0;
var TAG_SPRITE2 = 1;
var TAG_SPRITE3 = 2;
var TAG_SPRITE4 = 3;
var TAG_SPRITE5 = 4;
var TAG_SPRITE6 = 5;
var TAG_SPRITE7 = 6;
var TAG_SPRITE8 = 7;

var IDC_NEXT = 100;
var IDC_BACK = 101;
var IDC_RESTART = 102;

var spriteTestIdx = -1;

var spriteFrameCache = cc.SpriteFrameCache.getInstance();

//------------------------------------------------------------------
//
// SpriteTestDemo
//
//------------------------------------------------------------------
var SpriteTestDemo = BaseTestLayer.extend({
    _title:"",
    _subtitle:"",

    ctor:function () {
        if (arguments.length === 0) {
            this._super(cc.c4b(0, 0, 0, 255), cc.c4b(98, 99, 117, 255));
        } else {
            this._super.apply(this, arguments);
        }
    },

    onRestartCallback:function (sender) {
        var s = new SpriteTestScene();
        s.addChild(restartSpriteTest());
        director.replaceScene(s);
    },

    onNextCallback:function (sender) {
        var s = new SpriteTestScene();
        s.addChild(nextSpriteTest());
        director.replaceScene(s);
    },

    onBackCallback:function (sender) {
        var s = new SpriteTestScene();
        s.addChild(previousSpriteTest());
        director.replaceScene(s);
    },

    // automation
    numberOfPendingTests:function () {
        return ( (arrayOfSpriteTest.length - 1) - spriteTestIdx );
    },

    getTestNumber:function () {
        return spriteTestIdx;
    }
});

//------------------------------------------------------------------
//
// SpriteFrameTest
//
//------------------------------------------------------------------
var SpriteFrameTest = SpriteTestDemo.extend({
    _sprite1:null,
    _sprite2:null,
    _counter:0,
    _title:"Sprite vs. SpriteBatchNode animation",
    _subtitle:"Testing issue #792",

    onEnter:function () {
        this._super();
        // IMPORTANT:
        // The sprite frames will be cached AND RETAINED, and they won't be released unless you call
        //     cc.SpriteFrameCache.getInstance().removeUnusedSpriteFrames);
		cc.SpriteFrameCache.getInstance().removeUnusedSpriteFrames
        spriteFrameCache.addSpriteFrames(s_grossiniPlist);
        

        //
        // Animation using Sprite BatchNode
        //
        this._sprite1 = cc.Sprite.createWithSpriteFrameName("grossini_dance_01.png");
        this._sprite1.setPosition(cc.p(winSize.width / 2 - 80, winSize.height / 2));

		this.addChild(this._sprite1, 0, TAG_SPRITE1);
		
        var animFrames = [];
        var str = "";
        var frame;
        for (var i = 1; i < 5; i++) {
            str = "grossini_dance_" + (i < 10 ? ("0" + i) : i) + ".png";
            frame = spriteFrameCache.getSpriteFrame(str);
            animFrames.push(frame);
        }

        var animation = cc.Animation.create(animFrames, 0.5);
        this._sprite1.runAction(cc.RepeatForever.create(cc.Animate.create(animation)));

        // to test issue #732, uncomment the following line
        
    },
    onExit:function () {
        this._super();
        spriteFrameCache.removeSpriteFramesFromFile(s_grossiniPlist);
    },
    onStartIn05Secs:function () {
        //this.unschedule(this.onStartIn05Secs);
        //this.schedule(this.onFlipSprites, 1.0);
    },
    //
    // Automation
    //
    testDuration:3.1,
    pixel1:{"0":255, "1":204, "2":153, "3":255},
    pixel2:{"0":255, "1":204, "2":153, "3":255},
    getExpectedResult:function () {
        var ret = {"pixel1":"yes", "pixel2":"yes"};
        return JSON.stringify(ret);
    },
    getCurrentResult:function () {
        var ret1 = this.readPixels(winSize.width / 2 - 50, winSize.height / 2 + 8, 5, 5);
        var ret2 = this.readPixels(winSize.width / 2 - 80, winSize.height / 2 - 42, 5, 5);
        var ret = {"pixel1":this.containsPixel(ret1, this.pixel1) ? "yes" : "no",
            "pixel2":this.containsPixel(ret2, this.pixel2) ? "yes" : "no"};
        return JSON.stringify(ret);
    }
});

var SpriteTestScene = TestScene.extend({
    runThisTest:function () {
        spriteTestIdx = -1;
        var layer = nextSpriteTest();
        this.addChild(layer);

        director.replaceScene(this);
    }
});

//
// Flow control
//
var arrayOfSpriteTest = [
    SpriteFrameTest
];

var nextSpriteTest = function () {
    spriteTestIdx++;
    spriteTestIdx = spriteTestIdx % arrayOfSpriteTest.length;

    return new arrayOfSpriteTest[spriteTestIdx ]();
};
var previousSpriteTest = function () {
    spriteTestIdx--;
    if (spriteTestIdx < 0)
        spriteTestIdx += arrayOfSpriteTest.length;

    return new arrayOfSpriteTest[spriteTestIdx ]();
};
var restartSpriteTest = function () {
    return new arrayOfSpriteTest[spriteTestIdx ]();
};

