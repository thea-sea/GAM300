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
    public override void Awake()
    {
        Audiofiles = new String[17];
        Subtitles = new String[17];
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
        Subtitles[6] = "Martin (Internal): Alright, looks like I\'m in.";

        Subtitles[7] = "Martin (Internal): No turning back now.";


        Subtitles[8] = "";
        Subtitles[9] = "";

        Subtitles[10] = "";
        Subtitles[11] = "";
        //to be continued, cause how to detect player is in bedroom? change of scene?
        Subtitles[12] = "Empty, but i could've sworn someone opened that door";
        Subtitles[13] = "";

        Subtitles[14] = "";

        Subtitles[15] = "";

        Subtitles[16] = "";

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

    }

    public override void Update()
    {
        
        UISpriteComponent Sprite = gameObject.GetComponent<UISpriteComponent>();
        AudioComponent audio = gameObject.GetComponent<AudioComponent>();

         if (counter == 0)
        {
            if (Input.GetKeyDown(Keycode.F))
                counter++;
            
        }if (counter == 1)
        {
            if (Input.GetKeyDown(Keycode.W)|| Input.GetKeyDown(Keycode.A) || Input.GetKeyDown(Keycode.S) || Input.GetKeyDown(Keycode.D))
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
        if (LockPick1.counter == 2) //only this works when LockPick1 script is no longer active, checking passed doesnt work
        {
            //this is already handled in lockpick script
            //counter = 7; 
            if (LockPick1.audio.finished(LockPick1.playerGuideVO[2])) //no turning back now
            {
                LockPick1.audio.stop(LockPick1.playerGuideVO[2]);
                counter = 5;
                LockPick1.counter = 6;//prevent audio repeat
                //play enter house bgm
                audio.play(BGMfile[0]);
            }

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