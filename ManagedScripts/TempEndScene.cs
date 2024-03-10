using ScriptAPI;
using System;
public class TempEndScene : Script
{
    private bool fadeIn = false;
    private bool fadeOut = true;
    private float incrementFading = Time.deltaTime / 4f;
    readonly string runningSFX = "end_grassrunning";
    readonly string monsterSFX = "mon_death";
    AudioComponent audio;
    public override void Awake()
    {
        //audio = new AudioComponent();
        audio = gameObject.GetComponent<AudioComponent>();
        audio.play(runningSFX);
        audio.play(monsterSFX);
        audio.FadeOut(3, monsterSFX);
        audio.FadeIn(7, runningSFX);
    }
    public override void Start()
    {

    }
    public override void Update()
    {

        if (fadeIn == true)
        {
            float alpha = gameObject.GetComponent<UISpriteComponent>().getColourAlpha();
            alpha -= incrementFading;
            alpha = Mathf.Clamp(alpha, 0, 1);
            gameObject.GetComponent<UISpriteComponent>().setColourAlpha(alpha);
            if (alpha <= 0)
            {
                fadeIn = false;
                SceneLoader.LoadEndGameCredits();
            }
        }
        if (fadeOut == true)
        {
            float alpha = gameObject.GetComponent<UISpriteComponent>().getColourAlpha();
            alpha += incrementFading;
            alpha = Mathf.Clamp(alpha, 0, 1);
            gameObject.GetComponent<UISpriteComponent>().setColourAlpha(alpha);
            if (alpha >= 1)
            {
                fadeOut = false;
                fadeIn = true;
            }
        }
    }
    public override void LateUpdate()
    {

    }

}