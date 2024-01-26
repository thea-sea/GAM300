using ScriptAPI;
using System;

public class PlayButton : Script
{
    public AudioComponent bgm;
    public string bgmName;

    bool withinArea(float mouse, float min, float max)
    {
        bool within = false;
        if (mouse > min && mouse < max)
            within = true;
        return within;
    }

    bool withinButton(GameObject obj)
    {
        Vector3 ObjectPos = obj.GetComponent<TransformComponent>().GetPosition();//objectpos in ndc
        Vector3 ObjectScale = obj.GetComponent<TransformComponent>().GetScale();//obj scale in ndc
        float mouseX = Input.GetLocalMousePosX();
        float mouseY = Input.GetLocalMousePosY();
        float minX = ObjectPos.X - ObjectScale.X / 2;
        float maxX = ObjectPos.X + ObjectScale.X / 2;
        float maxy = -ObjectPos.Y + ObjectScale.Y / 2;
        float miny = -ObjectPos.Y - ObjectScale.Y / 2;
        if (withinArea(mouseX, minX, maxX) && withinArea(mouseY, miny, maxy))
            return true;
        else
            return false;
    }
    public override void Awake()
    {
        GraphicsManagerWrapper.ToggleViewFrom2D(true);
        bgmName = "skyclad_sound_ambience_dark_loop_dynamic_tones_howling_moaning_mournful_eerie_105";
        bgm = gameObject.GetComponent<AudioComponent>();
    }

    public override void Update()
    {
        bgm.play(bgmName);
        if (Input.GetMouseButtonDown(Keycode.M1) && withinButton(gameObject))
        {
            //GraphicsManagerWrapper.ToggleViewFrom2D(false);
            bgm.stop(bgmName);
            SceneLoader.LoadStartingCutscene();
        }
    }
}
