/*!*************************************************************************
****
\file GameplaySubtitles.cs
\author Thea Sea
\par DP email: thea.sea@digipen.edu
\par Course: csd3450
\date 24-11-2023
\brief  Script for any subtitles that appear in main game
****************************************************************************
***/
using ScriptAPI;
using System;

public class GameplaySubtitles : Script
{
    String[] Audiofiles;
    String[] BGMfile;
    String[] Subtitles;
    [SerializeField]
    public static int counter;
    public static bool next = true;
    float timer;
    int pressCtrlTwice = 0;

    public override void Awake()
    {
        Audiofiles = new String[17];
        Subtitles = new String[45];
        GraphicsManagerWrapper.ToggleViewFrom2D(false);
        Subtitles[0] = "Press [F] for flashlight";
        Subtitles[1] = "Press [WASD] to move";
        Subtitles[2] = "Press [I] to open/close Inventory";
        Subtitles[3] = "Press [I] to open/close Inventory";
        Subtitles[4] = "Press [E] to interact with objects"; //do lockpicking
                                                             //note: 
                                                             // these subtitles: "Martin (Internal): Hopefully, I won\’t forget how to do this.";
                                                             //"Move [mouse] to adjust pick", "Press [E] to turn lock"
                                                             //are done in LockPick1.cs already, so this handles overall gameplay subtitles

        Subtitles[5] = "";
        Subtitles[6] = "Alright, looks like I\'m in.";

        Subtitles[7] = "No turning back now.";


        Subtitles[8] = "";
        Subtitles[9] = "Nothing inside,";

        Subtitles[10] = "But I could hide in here in case someone shows up";
        Subtitles[11] = "What the hell was that?";
        //to be continued, cause how to detect player is in bedroom? change of scene?
        Subtitles[12] = "Empty, but I could've sworn someone opened that door";
        Subtitles[13] = "One down. Hopefully this is worth a lot";

        Subtitles[14] = "According to this, there's a painting somewhere in here. But where?";

        Subtitles[15] = "I have to watch my back. I can't get seen by that thing";

        Subtitles[16] = "The shower's running... but I don't hear anyone in there.";
        Subtitles[17] = "Okay, uncle, if you're an artist,";
        Subtitles[18] = "that means you've got paintings for me to steal.";
        Subtitles[19] = "Let's see where you're keeping them.";
        Subtitles[20] = "Something's behind this painting.";
        Subtitles[21] = "Is someone here?";
        Subtitles[22] = "Someone's coming... I better hide.";
        Subtitles[23] = "Flashlight's running out of juice... better replace the battery.";
        Subtitles[24] = "Press [Ctrl] to toggle crouch";
        Subtitles[25] = "The shower’s running... but I don’t hear anyone in there.";
        Subtitles[26] = "The tub is still wet, but there's no one...";
        Subtitles[27] = "Something's different about this one. What's this symbol on the back?";
        Subtitles[28] = "Painting: You shouldn't be here";
        Subtitles[29] = "Martin: Huh?";
        Subtitles[30] = "Painting: You have our blood, but you're not one of us..";
        Subtitles[31] = "Painting: And yet you choose to come back... why?";
        Subtitles[32] = "Painting: LEAVE WHILE YOU STILL CAN!";
        Subtitles[33] = "More paintings.";
        Subtitles[34] = "Please don’t scream...";
        Subtitles[35] = "There’s something... off about this one.";
        Subtitles[36] = "No! It saw me!";
        Subtitles[37] = "I hope it didn’t see me come in here...";
        Subtitles[38] = "It saw me hide in here, I have to go!";
        Subtitles[39] = "There’s something... off about this one.";
        Subtitles[40] = "Looks like the receipt was right.";
        Subtitles[41] = "Looks like there's more in the basement of this house.";
        Subtitles[42] = "The switch to open it is somewhere in this room.";
        Subtitles[43] = "That might have opened the door. Worth taking a look.";
        Subtitles[44] = "Sounds like it opened something. But what?";

        Audiofiles[0] = ""; //wasd no audio
        Audiofiles[1] = ""; //no audio
        Audiofiles[2] = ""; //no
        Audiofiles[3] = "";
        Audiofiles[4] = "";
        Audiofiles[5] = "";
        Audiofiles[6] = "houseenter";
        Audiofiles[7] = "";
        Audiofiles[8] = "";
        Audiofiles[9] = "";
        Audiofiles[10] = "";
        Audiofiles[11] = "";
        Audiofiles[12] = "pc_enterbedroom"; //empty, but i could've sworn someone opened that door
        Audiofiles[13] = "pc_approachbedroom"; //is someone here?
        Audiofiles[14] = "intro8_2";
        Audiofiles[15] = "intro9_1";
        Audiofiles[16] = "intro9_2";

        BGMfile = new String[1];
        BGMfile[0] = "houseenter";

        counter = 0;
        next = true;
        timer = 3.0f;
    }

    public override void Update()
    {

        UISpriteComponent Sprite = gameObject.GetComponent<UISpriteComponent>();
        AudioComponent audio = gameObject.GetComponent<AudioComponent>();

        if (counter == 0)
        {
            if (Input.GetKeyDown(Keycode.F))
                counter++;

        } if (counter == 1)
        {
            if (Input.GetKeyDown(Keycode.W) || Input.GetKeyDown(Keycode.A) || Input.GetKeyDown(Keycode.S) || Input.GetKeyDown(Keycode.D))
            {
                //go next line
                counter++;
            }
        }
        if (counter == 2 || counter == 3)
        {
            if (Input.GetKeyDown(Keycode.I))
                counter++;

        }
        if (counter == 4)
        {
            if (Input.GetKeyDown(Keycode.E))
                counter++;

        }
        if (counter == 7 )
        {
            if (audio.finished("pc_lockpicksuccess2"))
            {
                counter = 24;
            }
        }
        if (counter == 24)
        {
            if (pressCtrlTwice == 2)
            {
                counter = 17;
                audio.play("pc_okuncle");

            }
            if (Input.GetKeyDown(Keycode.CTRL))
            {
                pressCtrlTwice++;
            }


        }
        if (LockPick1.counter == 2) //only this works when LockPick1 script is no longer active, checking passed doesnt work
        {
            //this is already handled in lockpick script
            //counter = 7; 
            if (LockPick1.audio.finished(LockPick1.playerGuideVO[2])) //no turning back now
            {
                LockPick1.audio.stop(LockPick1.playerGuideVO[2]);
                //counter = 7;
                LockPick1.counter = 6;//prevent audio repeat
                //play enter house bgm
                audio.play(BGMfile[0]);
            }

        }
        if (counter == 9 || counter == 10)
        {
            if (audio.finished("pc_hideinclosetfirst"))
            {
                audio.stop("pc_hideinclosetfirst");
                GameplaySubtitles.counter = 5;
            }
        }
        
        if (counter == 15)
        {
            if (audio.finished("pc_monstergoesaway2"))
            {
                audio.stop("pc_monstergoesaway2");
                GameplaySubtitles.counter = 5;
            }
        }
        if (counter == 17)
        { 
            timer -= Time.deltaTime;
            if (timer<=0.0f)
            {
                counter = 18;
            }
            
        }
        if (counter == 18)
        {
            if (audio.finished("pc_okuncle")) 
            {
               
                counter = 19;
                audio.play("pc_letsseewhere");
            }
        }
        if (counter == 19)
        {
            if (audio.finished("pc_letsseewhere"))
            {
                audio.stop("pc_letsseewhere");
                counter = 5;
            }
        }
        
        if (counter == 21)
        {
            if (audio.finished("pc_approachbedroom"))
            {
                audio.stop("pc_approachbedroom");
                counter = 5;
            }
        }
        
        if (counter == 23)
        {
            if (audio.finished("pc_runningoutofjuice"))
            {
                audio.stop("pc_runningoutofjuice");
                GameplaySubtitles.counter = 5;

            }
        }
        
        // Notes stuff
        if (counter == 14) // Bedroom Receipt
        {
            if (audio.finished("pc_checkreceipt"))
            {
                audio.stop("pc_checkreceipt");
                counter = 5;
            }
        }
        if (counter == 41) // Gallery Letter
        {
            if (audio.finished("pc_moreinthebasement"))
            {
                audio.stop("pc_moreinthebasement");
                audio.play("pc_switchtoopen");
                counter = 42;
            }
        }
        if (counter == 42)
        {
            if (audio.finished("pc_switchtoopen"))
            {
                audio.stop("pc_switchtoopen");
                counter = 5; // No subtitles
            }
        }


        // Paintings Stuff
        if (counter == 20) // Bedroom painting before receipt
        {
            if (audio.finished("pc_shinelightbeforereceipt"))
            {
                audio.stop("pc_shinelightbeforereceipt");
                counter = 5;
            }
        }
        if (counter == 40) // Bedroom painting after receipt
        {
            if (audio.finished("pc_shinelightafterreceipt"))
            {
                audio.stop("pc_shinelightbeforereceipt");
                counter = 5;
            }
        }
        if (counter == 13) // Bedroom steal painting
        {
            if (audio.finished("pc_stealpainting1"))
            {
                audio.stop("pc_stealpainting1");
                counter = 8;

            }
        }
        if (counter == 8) // Gallery pick up painting
        {
            if (audio.finished("gallery_movepainting")) // Creaking sound ends
            {
                audio.stop("gallery_movepainting");
                audio.play("pc_monsterrattledoor"); // Someone's coming, better hide
                counter = 22; //commented this out as u dont hide after every painting u pick up
            }
        }
        if (counter == 22)
        {
            if (audio.finished("pc_monsterrattledoor"))
            {
                audio.stop("pc_monsterrattledoor");
                GameplaySubtitles.counter = 5;
            }
        }

        // Gallery Switch
        if(counter == 43)
        {
            if (audio.finished("pc_mighthaveopened"))
            {
                audio.stop("pc_mighthaveopened");
                GameplaySubtitles.counter = 8;
            }
        }
        if (counter == 27)
        {
            if (audio.finished("pc_somethingdiff"))
            {
                audio.play("misc_shouldntbehere");
                GameplaySubtitles.counter = 28;
            }
        }
        if (counter == 28)
        {
            if (audio.finished("misc_shouldntbehere"))
            {
                audio.play("pc_huh");
                GameplaySubtitles.counter = 29;
            }
        }
        if (counter == 29)
        {
            if (audio.finished("pc_huh"))
            {
                audio.play("misc_haveourblood");
                GameplaySubtitles.counter = 30;
            }

        }
        if (counter == 30)
        {
            if (audio.finished("misc_haveourblood"))
            {
                audio.play("misc_comeback");
                counter = 31;
            }
        }
        if (counter == 31)
        {
            if (audio.finished("misc_comeback"))
            {
                audio.play("misc_leave");
                counter = 32;
            }
        }
        if (counter == 32)
        {
            if (audio.finished("misc_leave"))
            {
                audio.play("pc_scream");
                audio.play("painting_dropin"); //scream and drop painting into fire same time 
                GameplaySubtitles.counter = 8;
            }
            audio.play("painting_burning");
        }

       
        if (counter == 44)
        {
            if (audio.finished("pc_openedsomething"))
            {
                audio.stop("pc_openedsomething");
                GameplaySubtitles.counter = 8;
            }
        }
        
        if (Painting_Script.isPaintingCollected)
        {
            //if (audio.finished("pc_stealpainting1"))
            //{
            //    audio.stop("pc_stealpainting1");
            //    GameplaySubtitles.counter = 8;
            //}
            //Painting_Script.isPaintingCollected = false; //reset for other paintings
        }
        if (counter == 8)
        {
            Note_Script.isNotePicked = false;
            Painting_Script.isPaintingCollected = false; //reset for other paintings
            
        }

        // if (Input.GetKeyDown(Keycode.SPACE))
        // {
        //     audio.stop(Audiofiles[counter]);
        //     GraphicsManagerWrapper.ToggleViewFrom2D(false);
        //     SceneLoader.LoadMainGame();
        // }
        // else
        // {
        //     audio.playQueue();

        //     if (counter > 16)//cutscene over
        //     {
        //         GraphicsManagerWrapper.ToggleViewFrom2D(false);
        //         SceneLoader.LoadMainGame();
        //     }
        //     else
        //     {
        //         if (next)
        //         {
        Sprite.SetFontMessage(Subtitles[counter]);
        //             audio.play(Audiofiles[counter]);
        //             next = false;
        //         }
        //         else if (audio.finished(Audiofiles[counter]))
        //         {
        //             next = true;
        //             ++counter;
        //         }
        //     }
        // }
        
    }
}