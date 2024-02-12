using ScriptAPI;
using System;

public class GameplaySubtitles : Script
{
    String[] Audiofiles;
    String[] Subtitles;
    [SerializeField]
    public static int counter;
    public static bool next = true;
    public override void Awake()
    {
        Audiofiles = new String[17];
        Subtitles = new String[17];
        GraphicsManagerWrapper.ToggleViewFrom2D(true);
        Subtitles[0] = "Press [WASD] to move";
        Subtitles[1] = "Press [I] to open/close Inventory";
        Subtitles[2] = "Press [E] to interact with door"; //do lockpicking
        //note: 
        // these subtitles: "Martin (Internal): Hopefully, I won\’t forget how to do this.";
        //"Move [mouse] to adjust pick", "Press [E] to turn lock"
        //are done in LockPick1.cs already, so this handles overall gameplay subtitles
        
        Subtitles[3] = "";
        Subtitles[4] = "";

        Subtitles[5] = "";

        Subtitles[6] = "";
        Subtitles[7] = "";

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
        Audiofiles[6] = "";
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

        counter = 0;
        next = true;

    }

    public override void Update()
    {
        UISpriteComponent Sprite = gameObject.GetComponent<UISpriteComponent>();
        AudioComponent audio = gameObject.GetComponent<AudioComponent>();

        if (counter == 0)
        {
            if (Input.GetKeyDown(Keycode.W)|| Input.GetKeyDown(Keycode.A) || Input.GetKeyDown(Keycode.S) || Input.GetKeyDown(Keycode.D))
            {
                //go next line
                counter++;
            }
        }
        if (counter == 1)
        {
            if (Input.GetKeyDown(Keycode.I))
                counter++;
            
        }
        if (counter == 2)
        {
            if (Input.GetKeyDown(Keycode.E))
                counter++;
          
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